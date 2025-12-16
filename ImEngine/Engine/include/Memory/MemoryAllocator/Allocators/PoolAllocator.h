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
#include "Memory/MemoryAllocator/MemoryAllocatorBase.h"
#include <atomic>

template <size_t BlockSize, size_t BlockCount>
struct PoolAllocator : MemoryAllocatorBase<PoolAllocator<BlockSize, BlockCount>>
{
	PoolAllocator()
	{
		for (size_t i = 0; i < BlockCount - 1; ++i)
			*reinterpret_cast<void**>(&m_pool[i * BlockSize]) = &m_pool[(i + 1) * BlockSize];
		*reinterpret_cast<void**>(&m_pool[(BlockCount - 1) * BlockSize]) = nullptr;
		m_freeList = m_pool;
	}

	void* impl_allocate(const size_t size, const size_t alignment = alignof(std::max_align_t))
	{
		if (size > BlockSize || !m_freeList) return nullptr;
		void* block = m_freeList;
		m_freeList  = *static_cast<void**>(m_freeList);
		m_used.fetch_add(1, std::memory_order_relaxed);
		return block;
	}

	void impl_deallocate(void* ptr)
	{
		*static_cast<void**>(ptr) = m_freeList;
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