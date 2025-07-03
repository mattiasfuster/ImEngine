// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// This software utilizes code from the following GitHub repositories, which are also licensed under the MIT License:
//
// - [ImGui](https://github.com/ocornut/imgui)
// - [GLFW](https://github.com/glfw/glfw)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

template <typename A>
concept MemoryAllocator = requires(A a, std::size_t s, std::size_t al, void* p) {
	{ a.impl_allocate(s, al) } -> std::same_as<void*>;
	{ a.impl_deallocate(p) } -> std::same_as<void>;
	{ std::as_const(a).impl_get_used_bytes() } -> std::same_as<std::size_t>;
};

template <typename DerivedAllocator>
struct MemoryAllocatorBase
{
	void* allocate(const size_t size, const size_t alignment = alignof(std::max_align_t))
	{
		return static_cast<DerivedAllocator*>(this)->impl_allocate(size, alignment);
	}

	void deallocate(void* ptr)
	{
		static_cast<DerivedAllocator*>(this)->impl_deallocate(ptr);
	}

	[[nodiscard]] size_t get_used_bytes() const
	{
		return static_cast<const DerivedAllocator*>(this)->impl_get_used_bytes();
	}
};


struct LinearAllocator : MemoryAllocatorBase<LinearAllocator>
{
	LinearAllocator(void* memory, const size_t size)
		: m_start(static_cast<uint8_t*>(memory)), m_capacity(size) {}

	void* impl_allocate(size_t size, size_t alignment)
	{
		assert((alignment & (alignment - 1)) == 0 && "Alignment must be a power of two");

		constexpr int max_attempts = 1000;
		int attempts = 0;

		while (attempts < max_attempts)
		{
			size_t current = m_offset.load(std::memory_order_relaxed);
			const size_t aligned = (current + alignment - 1) & ~(alignment - 1);
			const size_t newOff  = aligned + size;

			if (newOff > m_capacity)
				return nullptr;

			if (m_offset.compare_exchange_weak(current, newOff,
											   std::memory_order_acquire,
											   std::memory_order_relaxed))
			{
				return m_start + aligned;
			}

			++attempts;
		}

		return nullptr; // fail after too many retries
	}

	void impl_deallocate(void*) {}

	[[nodiscard]] size_t impl_get_used_bytes() const
	{
		return m_offset.load(std::memory_order_relaxed);
	}

private:
	uint8_t*                 m_start;
	const size_t             m_capacity;
	std::atomic_size_t       m_offset{0};
};


template <size_t BlockSize, size_t BlockCount>
struct PoolAllocator : MemoryAllocatorBase<PoolAllocator<BlockSize, BlockCount>>
{
	PoolAllocator()
	{
		m_pool = {};
		for (size_t i = 0; i < BlockCount - 1; ++i)
			*reinterpret_cast<void**>(&m_pool[i * BlockSize]) = &m_pool[(i + 1) * BlockSize];
		*reinterpret_cast<void**>(&m_pool[(BlockCount - 1) * BlockSize]) = nullptr;
		m_freeList = m_pool;
	}

	void* impl_allocate(size_t size, size_t)
	{
		if (size > BlockSize || !m_freeList) return nullptr;
		void* block = m_freeList;
		m_freeList  = *reinterpret_cast<void**>(m_freeList);
		m_used.fetch_add(1, std::memory_order_relaxed);
		return block;
	}

	void impl_deallocate(void* ptr)
	{
		*reinterpret_cast<void**>(ptr) = m_freeList;
		m_freeList = ptr;
		m_used.fetch_sub(1, std::memory_order_relaxed);
	}

	[[nodiscard]] size_t impl_get_used_bytes() const
	{
		return m_used.load(std::memory_order_relaxed) * BlockSize;
	}

private:
	alignas(BlockSize) uint8_t m_pool[BlockSize * BlockCount] = {};
	void*             m_freeList = nullptr;
	std::atomic_size_t m_used{0};
};