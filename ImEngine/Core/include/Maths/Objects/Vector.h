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

#ifndef IMENGINE_MATHS_OBJECTS_VECTOR_H
#define IMENGINE_MATHS_OBJECTS_VECTOR_H

#include "Maths/Misc/Constants.h"
#include "Maths/Misc/Units.h"

#include <type_traits>
#include <iostream>
#include <numbers>
#include <cmath>

using namespace ImEngine::Core::Maths::Constants;
using namespace ImEngine::Core::Maths::Units;

namespace ImEngine::Core::Maths::Objects
{

template <typename T>
concept AngleUnit = std::is_same_v<T, Degree> ||
                    std::is_same_v<T, uDegree> ||
                    std::is_same_v<T, Radian> ||
                    std::is_same_v<T, uRadian>;

template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
struct Vector2
{
    T x{}, y{};

    //Constructors
    constexpr Vector2() noexcept = default;
    constexpr Vector2(T x, T y) noexcept : x(x), y(y) {}

    constexpr Vector2(const Vector2&) noexcept = default;
    constexpr Vector2& operator=(const Vector2&) noexcept = default;
    constexpr Vector2(Vector2&&) noexcept = default;
    constexpr Vector2& operator=(Vector2&&) noexcept = default;

    //Destructors
    constexpr ~Vector2() noexcept = default;

    // Unary Operators
    constexpr Vector2 operator-() const noexcept { return Vector2(-x, -y); }

    // Binary Operators
    constexpr Vector2 operator+(const Vector2& Other) const noexcept { return Vector2(x + Other.x, y + Other.y); }
    constexpr Vector2 operator-(const Vector2& Other) const noexcept { return Vector2(x - Other.x, y - Other.y); }
    constexpr Vector2 operator*(const Vector2& Other) const noexcept { return Vector2(x * Other.x, y * Other.y); }
    constexpr Vector2 operator/(const Vector2& Other) const noexcept { return Vector2(x / Other.x, y / Other.y); }
    
    constexpr Vector2 operator*(const T Scalar) const noexcept { return Vector2(x * Scalar, y * Scalar); }
    constexpr Vector2 operator/(const T Scalar) const noexcept { return Vector2(x / Scalar, y / Scalar); }

    // Compound Assignment Operators
    constexpr void operator+=(const Vector2& Other) noexcept { x += Other.x; y += Other.y; }
    constexpr void operator-=(const Vector2& Other) noexcept { x -= Other.x; y -= Other.y; }
    constexpr void operator*=(const Vector2& Other) noexcept { x *= Other.x; y *= Other.y; }
    constexpr void operator/=(const Vector2& Other) noexcept { x /= Other.x; y /= Other.y; }
    
    constexpr void operator*=(const T Scalar) noexcept { x *= Scalar; y *= Scalar; }
    constexpr void operator/=(const T Scalar) noexcept { x /= Scalar; y /= Scalar; }

    // Math Functions
    constexpr double CrossProduct(const Vector2& Other) const noexcept { return static_cast<double>(x * Other.y - y * Other.x); }
    constexpr double DotProduct(const Vector2& Other) const noexcept { return static_cast<double>(x * Other.x + y * Other.y); }

    constexpr double SquaredLength() const noexcept { return static_cast<double>(x * x + y * y); }
    inline double Length() const noexcept { return std::sqrt(SquaredLength()); }

    inline double Distance(const Vector2& Other) const noexcept { return (*this - Other).Length(); }
    constexpr double SquaredDistance(const Vector2& Other) const noexcept { return (*this - Other).SquaredLength(); }

    constexpr Vector2 Normal() const noexcept { return Vector2(-y, x); } // Perpendicular

    inline void Normalize() noexcept { *this /= static_cast<T>(Length()); }
    inline Vector2 Normalized() const noexcept { return *this / static_cast<T>(Length()); }

    template<AngleUnit AngleType = Degree>
    double Angle(const Vector2& Other) const
    {
        double angle = std::atan2(CrossProduct(Other), DotProduct(Other));

        if constexpr (std::is_same_v<AngleType, Degree>)
            return angle * RadToDeg;
        else if constexpr (std::is_same_v<AngleType, uDegree>)
        {
            angle *= RadToDeg;
            return (angle < 0) ? angle + 360.0 : angle;
        }
        else if constexpr (std::is_same_v<AngleType, uRadian>)
            return (angle < 0) ? angle + (2.0 * std::numbers::pi) : angle;
        else
            return angle;
    }
};

template <Arithmetic T>
struct Vector3
{
    T x{}, y{}, z{};

    //Constructors
    constexpr Vector3() noexcept = default;
    constexpr Vector3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
    constexpr Vector3(const Vector2<T>& xy, T z) noexcept : x(xy.x), y(xy.y), z(z) {}

    constexpr Vector3(const Vector3&) noexcept = default;
    constexpr Vector3& operator=(const Vector3&) noexcept = default;
    constexpr Vector3(Vector3&&) noexcept = default;
    constexpr Vector3& operator=(Vector3&&) noexcept = default;

    //Destructors
    constexpr ~Vector3() noexcept = default;

    // Unary Operators
    constexpr Vector3 operator-() const noexcept { return Vector3(-x, -y, -z); }

    // Binary Operators
    constexpr Vector3 operator+(const Vector3& Other) const noexcept { return Vector3(x + Other.x, y + Other.y, z + Other.z); }
    constexpr Vector3 operator-(const Vector3& Other) const noexcept { return Vector3(x - Other.x, y - Other.y, z - Other.z); }
    constexpr Vector3 operator*(const Vector3& Other) const noexcept { return Vector3(x * Other.x, y * Other.y, z * Other.z); }
    constexpr Vector3 operator/(const Vector3& Other) const noexcept { return Vector3(x / Other.x, y / Other.y, z / Other.z); }
    
    constexpr Vector3 operator*(const T Scalar) const noexcept { return Vector3(x * Scalar, y * Scalar, z * Scalar); }
    constexpr Vector3 operator/(const T Scalar) const noexcept { return Vector3(x / Scalar, y / Scalar, z / Scalar); }

    // Compound Assignment Operators
    constexpr void operator+=(const Vector3& Other) noexcept { x += Other.x; y += Other.y; z += Other.z; }
    constexpr void operator-=(const Vector3& Other) noexcept { x -= Other.x; y -= Other.y; z -= Other.z; }
    constexpr void operator*=(const Vector3& Other) noexcept { x *= Other.x; y *= Other.y; z *= Other.z; }
    constexpr void operator/=(const Vector3& Other) noexcept { x /= Other.x; y /= Other.y; z /= Other.z; }
    
    constexpr void operator*=(const T Scalar) noexcept { x *= Scalar; y *= Scalar; z *= Scalar; }
    constexpr void operator/=(const T Scalar) noexcept { x /= Scalar; y /= Scalar; z /= Scalar; }

    // Math Functions
    constexpr Vector3 CrossProduct(const Vector3& Other) const noexcept 
    { 
        return Vector3(
            y * Other.z - z * Other.y,
            z * Other.x - x * Other.z,
            x * Other.y - y * Other.x
        ); 
    }
    
    constexpr double DotProduct(const Vector3& Other) const noexcept { return static_cast<double>(x * Other.x + y * Other.y + z * Other.z); }

    constexpr double SquaredLength() const noexcept { return static_cast<double>(x * x + y * y + z * z); }
    inline double Length() const noexcept { return std::sqrt(SquaredLength()); }

    inline double Distance(const Vector3& Other) const noexcept { return (*this - Other).Length(); }
    constexpr double SquaredDistance(const Vector3& Other) const noexcept { return (*this - Other).SquaredLength(); }

    inline void Normalize() noexcept { *this /= static_cast<T>(Length()); }
    inline Vector3 Normalized() const noexcept { return *this / static_cast<T>(Length()); }

    // Angle between two vectors
    template<AngleUnit AngleType = Degree>
    double Angle(const Vector3& Other) const
    {
        double dot = DotProduct(Other);
        double lenSq1 = SquaredLength();
        double lenSq2 = Other.SquaredLength();
        double angle = std::acos(dot / std::sqrt(lenSq1 * lenSq2));

        if constexpr (std::is_same_v<AngleType, Degree>)
            return angle * RadToDeg;
        else if constexpr (std::is_same_v<AngleType, uDegree>)
            return angle * RadToDeg;
        else
            return angle;
    }
};

template <Arithmetic T>
struct Vector4
{
    T x{}, y{}, z{}, w{};

    //Constructors
    constexpr Vector4() noexcept = default;
    constexpr Vector4(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}
    constexpr Vector4(const Vector3<T>& xyz, T w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    constexpr Vector4(const Vector2<T>& xy, T z, T w) noexcept : x(xy.x), y(xy.y), z(z), w(w) {}

    constexpr Vector4(const Vector4&) noexcept = default;
    constexpr Vector4& operator=(const Vector4&) noexcept = default;
    constexpr Vector4(Vector4&&) noexcept = default;
    constexpr Vector4& operator=(Vector4&&) noexcept = default;

    //Destructors
    constexpr ~Vector4() noexcept = default;

    // Unary Operators
    constexpr Vector4 operator-() const noexcept { return Vector4(-x, -y, -z, -w); }

    // Binary Operators
    constexpr Vector4 operator+(const Vector4& Other) const noexcept { return Vector4(x + Other.x, y + Other.y, z + Other.z, w + Other.w); }
    constexpr Vector4 operator-(const Vector4& Other) const noexcept { return Vector4(x - Other.x, y - Other.y, z - Other.z, w - Other.w); }
    constexpr Vector4 operator*(const Vector4& Other) const noexcept { return Vector4(x * Other.x, y * Other.y, z * Other.z, w * Other.w); }
    constexpr Vector4 operator/(const Vector4& Other) const noexcept { return Vector4(x / Other.x, y / Other.y, z / Other.z, w / Other.w); }
    
    constexpr Vector4 operator*(const T Scalar) const noexcept { return Vector4(x * Scalar, y * Scalar, z * Scalar, w * Scalar); }
    constexpr Vector4 operator/(const T Scalar) const noexcept { return Vector4(x / Scalar, y / Scalar, z / Scalar, w / Scalar); }

    // Compound Assignment Operators
    constexpr void operator+=(const Vector4& Other) noexcept { x += Other.x; y += Other.y; z += Other.z; w += Other.w; }
    constexpr void operator-=(const Vector4& Other) noexcept { x -= Other.x; y -= Other.y; z -= Other.z; w -= Other.w; }
    constexpr void operator*=(const Vector4& Other) noexcept { x *= Other.x; y *= Other.y; z *= Other.z; w *= Other.w; }
    constexpr void operator/=(const Vector4& Other) noexcept { x /= Other.x; y /= Other.y; z /= Other.z; w /= Other.w; }
    
    constexpr void operator*=(const T Scalar) noexcept { x *= Scalar; y *= Scalar; z *= Scalar; w *= Scalar; }
    constexpr void operator/=(const T Scalar) noexcept { x /= Scalar; y /= Scalar; z /= Scalar; w /= Scalar; }

    // Math Functions
    constexpr double DotProduct(const Vector4& Other) const noexcept { return static_cast<double>(x * Other.x + y * Other.y + z * Other.z + w * Other.w); }

    constexpr double SquaredLength() const noexcept { return static_cast<double>(x * x + y * y + z * z + w * w); }
    inline double Length() const noexcept { return std::sqrt(SquaredLength()); }

    inline double Distance(const Vector4& Other) const noexcept { return (*this - Other).Length(); }
    constexpr double SquaredDistance(const Vector4& Other) const noexcept { return (*this - Other).SquaredLength(); }

    inline void Normalize() noexcept { *this /= static_cast<T>(Length()); }
    inline Vector4 Normalized() const noexcept { return *this / static_cast<T>(Length()); }
};

//Type GLM style
using Vec2 = Vector2<float>;
using Vec3 = Vector3<float>;
using Vec4 = Vector4<float>;
}
#endif  // IMENGINE_MATHS_OBJECTS_VECTOR_H
