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
