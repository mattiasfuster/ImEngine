// MIT License
//
// Copyright (c) 2024 FUSTER Mattias
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef IMENGINE_MATHS_OBJECTS_QUATERNION_H_
#define IMENGINE_MATHS_OBJECTS_QUATERNION_H_


namespace ImEngine::Core::Maths::Objects {

template<std::floating_point T = float>
struct Quaternion {
  T x{};
  T y{};
  T z{};
  T w{};

  // Constructors
  constexpr Quaternion() noexcept = default;
  constexpr Quaternion(T x, T y, T z, T w) noexcept
    : x(x), y(y), z(z), w(w) {}
  constexpr Quaternion(const Vector& vec) :

  constexpr Quaternion(const Quaternion&) noexcept = default;
  constexpr Quaternion& operator=(const Quaternion&) noexcept = default;
  constexpr Quaternion(Quaternion&&) noexcept = default;
  constexpr Quaternion& operator=(Quaternion&&) noexcept = default;

  constexpr ~Quaternion() noexcept = default;
}

}

#endif  // IMENGINE_MATHS_OBJECTS_QUATERNION_H_