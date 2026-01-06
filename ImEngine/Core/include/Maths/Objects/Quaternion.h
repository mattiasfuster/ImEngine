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
  T w{1};
  T x{};
  T y{};
  T z{};

  constexpr Quaternion() noexcept = default;
  constexpr Quaternion(const Quaternion&) noexcept = default;
  constexpr Quaternion& operator=(const Quaternion&) noexcept = default;
  constexpr Quaternion(Quaternion&&) noexcept = default;
  constexpr Quaternion& operator=(Quaternion&&) noexcept = default;

  constexpr Quaternion(T w, T x, T y, T z) noexcept
      : w(w), x(x), y(y), z(z) {}

  // Explicit conversion from Vector4
  explicit constexpr Quaternion(const Vector4<T>& vec) noexcept
      : w(vec.w), x(vec.x), y(vec.y), z(vec.z) {}

  constexpr ~Quaternion() noexcept = default;

  // Static factory methods
  [[nodiscard]] static constexpr Quaternion Identity() noexcept {
    return Quaternion{};
  }

  [[nodiscard]] static constexpr Quaternion FromVector4(const Vector4<T>& vec)
      noexcept {
    return Quaternion{vec.w, vec.x, vec.y, vec.z};
  }

  // Explicit conversion to Vector4
  [[nodiscard]] constexpr Vector4<T> ToVector4() const noexcept {
    return Vector4<T>{w, x, y, z};
  }

  [[nodiscard]] constexpr Quaternion Conjugate() const noexcept {
    return Quaternion{w, -x, -y, -z};
  }

  [[nodiscard]] constexpr double DotProduct(const Vector4& other) const
    noexcept {
    return static_cast<double>(
    x * other.x +
    y * other.y +
    z * other.z +
    w * other.w);
  }

  [[nodiscard]] constexpr double SquaredLength() const noexcept {
    return static_cast<double>(x * x + y * y + z * z + w * w);
  }

  [[nodiscard]] double Length() const noexcept {
    return std::sqrt(SquaredLength());
  }

  [[nodiscard]] double Distance(const Vector4& other) const noexcept {
    return (*this - other).Length();
  }

  [[nodiscard]] constexpr double SquaredDistance(const Vector4& other) const
      noexcept {
    return (*this - other).SquaredLength();
  }

};

}

#endif  // IMENGINE_MATHS_OBJECTS_QUATERNION_H_