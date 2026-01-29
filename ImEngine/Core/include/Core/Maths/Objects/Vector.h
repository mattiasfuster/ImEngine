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

#ifndef IMENGINE_MATHS_OBJECTS_VECTOR_H_
#define IMENGINE_MATHS_OBJECTS_VECTOR_H_

#include <cmath>
#include <numbers>
#include <type_traits>

#include "Core/Maths/Concepts.h"
#include "Core/Maths/Constants.h"
#include "Core/Maths/Objects/Matrice.h"

using namespace ImEngine::Core::Maths::Constants;
using namespace ImEngine::Core::Maths::Units;

namespace ImEngine::Core::Maths::Objects {

using ImEngine::Core::Maths::Concepts::AngleUnit;
using ImEngine::Core::Maths::Concepts::Arithmetic;

// 2D vector for geometric operations.
template <Arithmetic T>
struct Vector2 {
  T x{};
  T y{};

  // Constructors
  constexpr Vector2() noexcept = default;
  constexpr Vector2(T x, T y) noexcept : x(x), y(y) {}

  constexpr Vector2(const Vector2&) noexcept = default;
  constexpr Vector2& operator=(const Vector2&) noexcept = default;
  constexpr Vector2(Vector2&&) noexcept = default;
  constexpr Vector2& operator=(Vector2&&) noexcept = default;

  constexpr ~Vector2() noexcept = default;

  // Unary Operators
  [[nodiscard]] constexpr Vector2 operator-() const noexcept {
    return Vector2(-x, -y);
  }

  // Binary Operators
  [[nodiscard]] constexpr Vector2 operator+(
      const Vector2& other) const noexcept {
    return Vector2(x + other.x, y + other.y);
  }
  [[nodiscard]] constexpr Vector2 operator-(
      const Vector2& other) const noexcept {
    return Vector2(x - other.x, y - other.y);
  }
  [[nodiscard]] constexpr Vector2 operator*(
      const Vector2& other) const noexcept {
    return Vector2(x * other.x, y * other.y);
  }
  [[nodiscard]] constexpr Vector2 operator/(
      const Vector2& other) const noexcept {
    return Vector2(x / other.x, y / other.y);
  }

  [[nodiscard]] constexpr Vector2 operator*(T scalar) const noexcept {
    return Vector2(x * scalar, y * scalar);
  }
  [[nodiscard]] constexpr Vector2 operator/(T scalar) const noexcept {
    return Vector2(x / scalar, y / scalar);
  }

  // Compound Assignment Operators
  constexpr void operator+=(const Vector2& other) noexcept {
    x += other.x;
    y += other.y;
  }
  constexpr void operator-=(const Vector2& other) noexcept {
    x -= other.x;
    y -= other.y;
  }
  constexpr void operator*=(const Vector2& other) noexcept {
    x *= other.x;
    y *= other.y;
  }
  constexpr void operator/=(const Vector2& other) noexcept {
    x /= other.x;
    y /= other.y;
  }

  constexpr void operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
  }
  constexpr void operator/=(T scalar) noexcept {
    x /= scalar;
    y /= scalar;
  }

  // Math Functions
  [[nodiscard]] constexpr double CrossProduct(
      const Vector2& other) const noexcept {
    return static_cast<double>(x * other.y - y * other.x);
  }
  [[nodiscard]] constexpr double DotProduct(
      const Vector2& other) const noexcept {
    return static_cast<double>(x * other.x + y * other.y);
  }

  [[nodiscard]] constexpr double SquaredLength() const noexcept {
    return static_cast<double>(x * x + y * y);
  }
  [[nodiscard]] double Length() const noexcept {
    return std::sqrt(SquaredLength());
  }

  [[nodiscard]] double Distance(const Vector2& other) const noexcept {
    return (*this - other).Length();
  }
  [[nodiscard]] constexpr double SquaredDistance(
      const Vector2& other) const noexcept {
    return (*this - other).SquaredLength();
  }

  [[nodiscard]] constexpr Vector2 Normal() const noexcept {
    return Vector2(-y, x);  // Perpendicular
  }

  void Normalize() noexcept { *this /= static_cast<T>(Length()); }

  [[nodiscard]] Vector2 Normalized() const noexcept {
    return *this / static_cast<T>(Length());
  }

  template <AngleUnit AngleType = Degree>
  [[nodiscard]] double Angle(const Vector2& other) const {
    double angle = std::atan2(CrossProduct(other), DotProduct(other));

    if constexpr (std::is_same_v<AngleType, Degree>) {
      return angle * RadToDeg<double>;
    } else if constexpr (std::is_same_v<AngleType, uDegree>) {
      angle *= RadToDeg<double>;
      return (angle < 0) ? angle + 360.0 : angle;
    } else if constexpr (std::is_same_v<AngleType, uRadian>) {
      return (angle < 0) ? angle + (2.0 * std::numbers::pi) : angle;
    } else {
      return angle;
    }
  }
};

// 3D vector for geometric operations.
template <Arithmetic T>
struct Vector3 {
  T x{};
  T y{};
  T z{};

  // Constructors
  constexpr Vector3() noexcept = default;
  constexpr Vector3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
  constexpr Vector3(const Vector2<T>& xy, T z) noexcept
      : x(xy.x), y(xy.y), z(z) {}
  constexpr Vector3(const Matrice<3, 1, T> mat) noexcept
      : x(mat.data[0][0]), y(mat.data[0][1]), z(mat.data[0][2]) {}

  constexpr Vector3(const Vector3&) noexcept = default;
  constexpr Vector3& operator=(const Vector3&) noexcept = default;
  constexpr Vector3(Vector3&&) noexcept = default;
  constexpr Vector3& operator=(Vector3&&) noexcept = default;

  constexpr ~Vector3() noexcept = default;

  // Unary Operators
  [[nodiscard]] constexpr Vector3 operator-() const noexcept {
    return Vector3(-x, -y, -z);
  }

  // Binary Operators
  [[nodiscard]] constexpr Vector3 operator+(
      const Vector3& other) const noexcept {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }
  [[nodiscard]] constexpr Vector3 operator-(
      const Vector3& other) const noexcept {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }
  [[nodiscard]] constexpr Vector3 operator*(
      const Vector3& other) const noexcept {
    return Vector3(x * other.x, y * other.y, z * other.z);
  }
  [[nodiscard]] constexpr Vector3 operator/(
      const Vector3& other) const noexcept {
    return Vector3(x / other.x, y / other.y, z / other.z);
  }

  [[nodiscard]] constexpr Vector3 operator*(T scalar) const noexcept {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }
  [[nodiscard]] constexpr Vector3 operator/(T scalar) const noexcept {
    return Vector3(x / scalar, y / scalar, z / scalar);
  }

  // Compound Assignment Operators
  constexpr void operator+=(const Vector3& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
  }
  constexpr void operator-=(const Vector3& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
  }
  constexpr void operator*=(const Vector3& other) noexcept {
    x *= other.x;
    y *= other.y;
    z *= other.z;
  }
  constexpr void operator/=(const Vector3& other) noexcept {
    x /= other.x;
    y /= other.y;
    z /= other.z;
  }

  constexpr void operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
  }
  constexpr void operator/=(T scalar) noexcept {
    x /= scalar;
    y /= scalar;
    z /= scalar;
  }

  // Math Functions
  [[nodiscard]] constexpr Vector3 CrossProduct(
      const Vector3& other) const noexcept {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
  }

  [[nodiscard]] constexpr double DotProduct(
      const Vector3& other) const noexcept {
    return static_cast<double>(x * other.x + y * other.y + z * other.z);
  }

  [[nodiscard]] constexpr double SquaredLength() const noexcept {
    return static_cast<double>(x * x + y * y + z * z);
  }
  [[nodiscard]] double Length() const noexcept {
    return std::sqrt(SquaredLength());
  }

  [[nodiscard]] double Distance(const Vector3& other) const noexcept {
    return (*this - other).Length();
  }
  [[nodiscard]] constexpr double SquaredDistance(
      const Vector3& other) const noexcept {
    return (*this - other).SquaredLength();
  }

  void Normalize() noexcept { *this /= static_cast<T>(Length()); }
  [[nodiscard]] Vector3 Normalized() const noexcept {
    return *this / static_cast<T>(Length());
  }

  // Angle between two vectors.
  template <AngleUnit AngleType = Degree>
  [[nodiscard]] double Angle(const Vector3& other) const {
    double dot = DotProduct(other);
    double len_sq1 = SquaredLength();
    double len_sq2 = other.SquaredLength();
    double angle = std::acos(dot / std::sqrt(len_sq1 * len_sq2));

    if constexpr (std::is_same_v<AngleType, Degree>) {
      return angle * RadToDeg<double>;
    } else if constexpr (std::is_same_v<AngleType, uDegree>) {
      return angle * RadToDeg<double>;
    } else {
      return angle;
    }
  }
};

// 4D vector for homogeneous coordinates.
template <Arithmetic T>
struct Vector4 {
  T w{};
  T x{};
  T y{};
  T z{};

  // Constructors
  constexpr Vector4() noexcept = default;
  constexpr Vector4(T w, T x, T y, T z) noexcept : w(w), x(x), y(y), z(z) {}
  constexpr Vector4(T w, const Vector3<T>& xyz) noexcept
      : w(w), x(xyz.x), y(xyz.y), z(xyz.z) {}
  constexpr Vector4(T w, const Vector2<T>& xy, T z) noexcept
      : w(w), x(xy.x), y(xy.y), z(z) {}
  constexpr Vector4(const Matrice<4, 1, T> mat) noexcept
      : w(mat.data[3][0]),
        x(mat.data[0][0]),
        y(mat.data[1][0]),
        z(mat.data[2][0]) {}

  constexpr Vector4(const Vector4&) noexcept = default;
  constexpr Vector4& operator=(const Vector4&) noexcept = default;
  constexpr Vector4(Vector4&&) noexcept = default;
  constexpr Vector4& operator=(Vector4&&) noexcept = default;

  constexpr ~Vector4() noexcept = default;

  // Unary Operators
  [[nodiscard]] constexpr Vector4 operator-() const noexcept {
    return Vector4(-w, -x, -y, -z);
  }

  // Binary Operators
  [[nodiscard]] constexpr Vector4 operator+(
      const Vector4& other) const noexcept {
    return Vector4(w + other.w, x + other.x, y + other.y, z + other.z);
  }
  [[nodiscard]] constexpr Vector4 operator-(
      const Vector4& other) const noexcept {
    return Vector4(w - other.w, x - other.x, y - other.y, z - other.z);
  }
  [[nodiscard]] constexpr Vector4 operator*(
      const Vector4& other) const noexcept {
    return Vector4(w * other.w, x * other.x, y * other.y, z * other.z);
  }
  [[nodiscard]] constexpr Vector4 operator/(
      const Vector4& other) const noexcept {
    return Vector4(w / other.w, x / other.x, y / other.y, z / other.z);
  }

  [[nodiscard]] constexpr Vector4 operator*(T scalar) const noexcept {
    return Vector4(w * scalar, x * scalar, y * scalar, z * scalar);
  }
  [[nodiscard]] constexpr Vector4 operator/(T scalar) const noexcept {
    return Vector4(w / scalar, x / scalar, y / scalar, z / scalar);
  }

  // Compound Assignment Operators
  constexpr void operator+=(const Vector4& other) noexcept {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
  }
  constexpr void operator-=(const Vector4& other) noexcept {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
  }
  constexpr void operator*=(const Vector4& other) noexcept {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
  }
  constexpr void operator/=(const Vector4& other) noexcept {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
  }

  constexpr void operator*=(T scalar) noexcept {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
  }
  constexpr void operator/=(T scalar) noexcept {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
  }

  // Math Functions
  [[nodiscard]] constexpr double DotProduct(
      const Vector4& other) const noexcept {
    return static_cast<double>(x * other.x + y * other.y + z * other.z +
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
  [[nodiscard]] constexpr double SquaredDistance(
      const Vector4& other) const noexcept {
    return (*this - other).SquaredLength();
  }

  void Normalize() noexcept { *this /= static_cast<T>(Length()); }
  [[nodiscard]] Vector4 Normalized() const noexcept {
    return *this / static_cast<T>(Length());
  }
};

}  // namespace ImEngine::Core::Maths::Objects

// Type aliases (GLM style).
template <typename T = float>
using Vector2 = ImEngine::Core::Maths::Objects::Vector2<T>;

template <typename T = float>
using Vector3 = ImEngine::Core::Maths::Objects::Vector3<T>;

template <typename T = float>
using Vector4 = ImEngine::Core::Maths::Objects::Vector4<T>;

#endif  // IMENGINE_MATHS_OBJECTS_VECTOR_H_
