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

#ifndef IMENGINE_MATHS_OBJECTS_MATRICE_H_
#define IMENGINE_MATHS_OBJECTS_MATRICE_H_

#include <iomanip>
#include <initializer_list>
#include <sstream>
#include <string>

namespace ImEngine::Core::Maths::Objects {

// Generic matrix with M rows and N columns.
template <size_t M, size_t N, class T>
struct Matrice {
  T data[M][N]{};
  static constexpr size_t kRows = M;
  static constexpr size_t kCols = N;

  // Constructors
  constexpr Matrice() noexcept = default;

  explicit constexpr Matrice(T scalar) noexcept {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        data[i][j] = scalar;
      }
    }
  }

  constexpr Matrice(std::initializer_list<std::initializer_list<T>> list)
      noexcept {
    size_t i = 0;
    for (const auto& row_list : list) {
      if (i >= M) break;
      size_t j = 0;
      for (const auto& val : row_list) {
        if (j >= N) break;
        data[i][j] = val;
        ++j;
      }
      ++i;
    }
  }

  constexpr Matrice(const Matrice&) noexcept = default;
  constexpr Matrice& operator=(const Matrice&) noexcept = default;
  constexpr Matrice(Matrice&&) noexcept = default;
  constexpr Matrice& operator=(Matrice&&) noexcept = default;

  constexpr ~Matrice() noexcept = default;

  [[nodiscard]] Matrice<M, N, T> operator+(const Matrice<M, N, T>& other) const
      noexcept {
    Matrice<M, N, T> result;
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        result.data[i][j] = data[i][j] + other.data[i][j];
      }
    }
    return result;
  }

  [[nodiscard]] Matrice<M, N, T> operator-(const Matrice<M, N, T>& other) const
      noexcept {
    Matrice<M, N, T> result;
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        result.data[i][j] = data[i][j] - other.data[i][j];
      }
    }
    return result;
  }

  template <size_t O>
  [[nodiscard]] Matrice<M, O, T> operator*(const Matrice<N, O, T>& other) const
      noexcept {
    Matrice<M, O, T> result;
    for (size_t i = 0; i < O; ++i) {
      for (size_t j = 0; j < M; ++j) {
        for (size_t k = 0; k < N; ++k) {
          result.data[j][i] += data[j][k] * other.data[k][i];
        }
      }
    }
    return result;
  }

  [[nodiscard]] Matrice<M, N, T> operator*(double scalar) const noexcept {
    Matrice<M, N, T> result;
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        result.data[i][j] = data[i][j] * scalar;
      }
    }
    return result;
  }

  [[nodiscard]] constexpr Matrice<M, N, T> operator/(double scalar) const
      noexcept {
    if (scalar == 0) {
      return Matrice();
    }
    Matrice<M, N, T> result;
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        result.data[i][j] = data[i][j] / scalar;
      }
    }
    return result;
  }

  [[nodiscard]] constexpr T Determinant() const
    requires(M == N)
  {
    if constexpr (M == 1) {
      return data[0][0];
    } else if constexpr (M == 2) {
      return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    } else if constexpr (M == 3) {
      return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
             data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
             data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
    } else {
      // TODO: Implement Gaussian elimination.
      return T();
    }
  }

  [[nodiscard]] std::string Print() const {
    std::stringstream ss;
    for (size_t i = 0; i < M; ++i) {
      if (M > 1) {
        if (i == 0) {
          ss << "/ ";
        } else if (i == M - 1) {
          ss << "\\ ";
        } else {
          ss << "| ";
        }
      } else {
        ss << "[ ";
      }

      for (size_t j = 0; j < N; ++j) {
        ss << std::fixed << std::setprecision(2) << std::setw(8) << data[i][j]
           << " ";
      }

      if (M > 1) {
        if (i == 0) {
          ss << "\\";
        } else if (i == M - 1) {
          ss << "/";
        } else {
          ss << "|";
        }
      } else {
        ss << "]";
      }
      ss << "\n";
    }
    return ss.str();
  }
};

}  // namespace ImEngine::Core::Maths::Objects

// Type aliases (GLM style).
template <typename T = float>
using Mat3 = ImEngine::Core::Maths::Objects::Matrice<3, 3, T>;

template <typename T = float>
using Mat4 = ImEngine::Core::Maths::Objects::Matrice<4, 4, T>;

#endif  // IMENGINE_MATHS_OBJECTS_MATRICE_H_