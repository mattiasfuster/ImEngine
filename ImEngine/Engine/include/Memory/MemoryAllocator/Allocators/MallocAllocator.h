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
#include <new>

struct MallocAllocator : MemoryAllocatorBase<MallocAllocator>
{
	void* impl_allocate(const size_t size, const size_t alignment = alignof(std::max_align_t))
	{
		if (size == 0) return nullptr;
		const size_t totalSize = size + sizeof(size_t);
		void* rawPtr = nullptr;

#if defined(_MSC_VER)
		rawPtr = _aligned_malloc(totalSize, alignment);
		if (!rawPtr) throw std::bad_alloc();
#else
		if (posix_memalign(&rawPtr, alignment, totalSize) != 0)
			throw std::bad_alloc();
#endif

		*static_cast<size_t*>(rawPtr) = size;
		m_usedBytes.fetch_add(size, std::memory_order_relaxed);
		return static_cast<uint8_t*>(rawPtr) + sizeof(size_t);
	}

	void impl_deallocate(void* ptr)
	{
		if (!ptr) return;
		void*        rawPtr = static_cast<uint8_t*>(ptr) - sizeof(size_t);
		const size_t size = *static_cast<size_t*>(rawPtr);
		m_usedBytes.fetch_sub(size, std::memory_order_relaxed);
#if defined(_MSC_VER)
		_aligned_free(rawPtr);
#else
		std::free(rawPtr);
#endif
	}

	[[nodiscard]] size_t impl_get_used_bytes() const
	{
		return m_usedBytes.load(std::memory_order_relaxed);
	}

private:
	std::atomic_size_t m_usedBytes{0};
};