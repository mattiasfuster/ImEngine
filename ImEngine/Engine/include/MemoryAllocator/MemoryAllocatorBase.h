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

//
// Created by damma on 17/05/2025.
//

#ifndef MEMORYALLOCATORBASE_H
#define MEMORYALLOCATORBASE_H

template <typename DerivedAllocator>
class MemoryAllocatorBase
{
public:
	void* allocate(size_t size, size_t alignment = alignof(std::max_align_t))
	{
		return static_cast<DerivedAllocator*>(this)->impl_allocate(size, alignment);
	}

	void deallocate(void* ptr)
	{
		static_cast<DerivedAllocator*>(this)->impl_deallocate(ptr);
	}

	size_t get_used_bytes()
	{
		return static_cast<DerivedAllocator*>(this)->impl_get_used_bytes();
	}
};




#endif //MEMORYALLOCATORBASE_H
