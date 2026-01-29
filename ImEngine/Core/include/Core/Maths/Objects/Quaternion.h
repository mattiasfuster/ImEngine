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

template <std::floating_point T = float>
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

  constexpr Quaternion(T w, T x, T y, T z) noexcept : w(w), x(x), y(y), z(z) {}

  // Explicit conversion from Vector4
  explicit constexpr Quaternion(const Vector4<T>& vec) noexcept
      : w(vec.w), x(vec.x), y(vec.y), z(vec.z) {}

  constexpr ~Quaternion() noexcept = default;

  // Static factory methods
  [[nodiscard]] static constexpr Quaternion Identity() noexcept {
    return Quaternion{};
  }

  [[nodiscard]] static constexpr Quaternion FromVector4(
      const Vector4<T>& vec) noexcept {
    return Quaternion{vec.w, vec.x, vec.y, vec.z};
  }

  [[nodiscard]] static Quaternion FromRotationMatrix(
      const Matrice<3, 3, T>& mat) noexcept {
    T trace = mat.data[0][0] + mat.data[1][1] + mat.data[2][2];

    if (trace > T{0}) {
      T s = std::sqrt(trace + T{1}) * T{2};
      return Quaternion{s / T{4}, (mat.data[2][1] - mat.data[1][2]) / s,
                        (mat.data[0][2] - mat.data[2][0]) / s,
                        (mat.data[1][0] - mat.data[0][1]) / s};
    } else if (mat.data[0][0] > mat.data[1][1] &&
               mat.data[0][0] > mat.data[2][2]) {
      T s = std::sqrt(T{1} + mat.data[0][0] - mat.data[1][1] - mat.data[2][2]) *
            T{2};
      return Quaternion{(mat.data[2][1] - mat.data[1][2]) / s, s / T{4},
                        (mat.data[0][1] + mat.data[1][0]) / s,
                        (mat.data[0][2] + mat.data[2][0]) / s};
    } else if (mat.data[1][1] > mat.data[2][2]) {
      T s = std::sqrt(T{1} + mat.data[1][1] - mat.data[0][0] - mat.data[2][2]) *
            T{2};
      return Quaternion{(mat.data[0][2] - mat.data[2][0]) / s,
                        (mat.data[0][1] + mat.data[1][0]) / s, s / T{4},
                        (mat.data[1][2] + mat.data[2][1]) / s};
    } else {
      T s = std::sqrt(T{1} + mat.data[2][2] - mat.data[0][0] - mat.data[1][1]) *
            T{2};
      return Quaternion{(mat.data[1][0] - mat.data[0][1]) / s,
                        (mat.data[0][2] + mat.data[2][0]) / s,
                        (mat.data[1][2] + mat.data[2][1]) / s, s / T{4}};
    }
  }

  // Conversion from rotation matrix (4x4) - extracts upper-left 3x3
  [[nodiscard]] static Quaternion FromRotationMatrix(
      const Matrice<4, 4, T>& mat) noexcept {
    Matrice<3, 3, T> rot3x3;
    for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        rot3x3.data[i][j] = mat.data[i][j];
      }
    }
    return FromRotationMatrix(rot3x3);
  }

  [[nodiscard]] constexpr Vector4<T> ToVector4() const noexcept {
    return Vector4<T>{w, x, y, z};
  }

  [[nodiscard]] constexpr Matrice<3, 3, T> ToRotationMatrix3() const noexcept {
    T xx = x * x, yy = y * y, zz = z * z;
    T xy = x * y, xz = x * z, yz = y * z;
    T wx = w * x, wy = w * y, wz = w * z;

    return Matrice<3, 3, T>{
        {T{1} - T{2} * (yy + zz), T{2} * (xy - wz), T{2} * (xz + wy)},
        {T{2} * (xy + wz), T{1} - T{2} * (xx + zz), T{2} * (yz - wx)},
        {T{2} * (xz - wy), T{2} * (yz + wx), T{1} - T{2} * (xx + yy)}};
  }

  [[nodiscard]] constexpr Matrice<4, 4, T> ToRotationMatrix4() const noexcept {
    T xx = x * x, yy = y * y, zz = z * z;
    T xy = x * y, xz = x * z, yz = y * z;
    T wx = w * x, wy = w * y, wz = w * z;

    return Matrice<4, 4, T>{
        {T{1} - T{2} * (yy + zz), T{2} * (xy - wz), T{2} * (xz + wy), T{0}},
        {T{2} * (xy + wz), T{1} - T{2} * (xx + zz), T{2} * (yz - wx), T{0}},
        {T{2} * (xz - wy), T{2} * (yz + wx), T{1} - T{2} * (xx + yy), T{0}},
        {T{0}, T{0}, T{0}, T{1}}};
  }

  [[nodiscard]] constexpr Quaternion Conjugate() const noexcept {
    return Quaternion{w, -x, -y, -z};
  }

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
};

}  // namespace ImEngine::Core::Maths::Objects

#endif  // IMENGINE_MATHS_OBJECTS_QUATERNION_H_