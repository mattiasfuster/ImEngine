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

struct LinearAllocator : MemoryAllocatorBase<LinearAllocator>
{
	LinearAllocator(void* memory, const size_t size)
		: m_start(static_cast<uint8_t*>(memory)), m_capacity(size) {}

	void* impl_allocate(const size_t size, const size_t alignment = alignof(std::max_align_t))
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

		return nullptr;
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