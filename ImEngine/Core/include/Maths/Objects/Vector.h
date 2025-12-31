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

    //Operator Overloads
    auto operator<=>(const Vector2&) const noexcept = default;

    Vector2 operator-() const noexcept { return Vector2(-x, -y); }

    Vector2 operator+(const Vector2& Other) const noexcept
    {
        return Vector2(x + Other.x, y + Other.y);
    }
    Vector2 operator-(const Vector2& Other) const noexcept
    {
        return Vector2(x - Other.x, y - Other.y);
    }
    Vector2 operator*(const Vector2& Other) const noexcept
    {
        return Vector2(x * Other.x, y * Other.y);
    }
    Vector2 operator/(const Vector2& Other) const noexcept
    {
        return Vector2(x / Other.x, y / Other.y);
    }
    Vector2 operator*(const T Scalar) const noexcept
    {
        return Vector2(x * Scalar, y * Scalar);
    }
    Vector2 operator/(const T Scalar) const noexcept
    {
        return Vector2(x / Scalar, y / Scalar);
    }
    void operator+=(const Vector2& Other) noexcept
    {
        x += Other.x;
        y += Other.y;
    }
    void operator-=(const Vector2& Other) noexcept
    {
        x -= Other.x;
        y -= Other.y;
    }
    void operator*=(const Vector2& Other) noexcept
    {
        x *= Other.x;
        y *= Other.y;
    }
    void operator/=(const Vector2& Other) noexcept
    {
        x /= Other.x;
        y /= Other.y;
    }
    void operator*=(const T Scalar) noexcept
    {
        x *= Scalar;
        y *= Scalar;
    }
    void operator/=(const T Scalar) noexcept
    {
        x /= Scalar;
        y /= Scalar;
    }

    inline double CrossProduct(const Vector2& Other) const
    {
        return x * Other.y - y * Other.x;
    }

    inline double DotProduct(const Vector2& Other) const
    {
        return x * Other.x + y * Other.y;
    }

    double SquaredLength()
    { 
        return x * x + y * y;
    }

    double Length()
    {
        return std::sqrt(SquaredLength());
    }

    T Distance(const Vector2& Other)
    {
        return (*this - Other).Length();
    }

    T SquaredDistance(const Vector2& Other)
    {
        return (*this - Other).SquaredLength();
    }

    Vector2 Normal()
    {
        return Vector2(-y, x);
    }

    void Normalize()
    {
        *this /= Length();
    }

    double Nomalized()
    {
        return *this / Length();
    }

    template<AngleUnit AngleType = Degree>
    double Angle(const Vector2& Other) const
    {
        double angle = std::atan2(CrossProduct(Other), DotProduct(Other));

        if constexpr (std::is_same_v<AngleType, Degree>)
        {
            return angle * RadToDeg;
        }
        else if constexpr (std::is_same_v<AngleType, uDegree>)
        {
            angle *= RadToDeg;
            return (angle < 0) ? angle + 360.0 : angle;
        }
        else if constexpr (std::is_same_v<AngleType, uRadian>)
        {
            return (angle < 0) ? angle + (2.0 * std::numbers::pi) : angle;
        }
        else
        {
            return angle;
        }
    }
};

}
#endif  // IMENGINE_MATHS_OBJECTS_VECTOR_H
