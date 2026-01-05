#ifndef IMENGINE_MATHS_OBJECTS_MATRICE_H
#define IMENGINE_MATHS_OBJECTS_MATRICE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <initializer_list>

// M Row N Col Ty Type
template<size_t M, size_t N, class Ty>
struct Matrice
{
    Ty Data[M][N]{};
    static constexpr size_t Col = N;
    static constexpr size_t Row = M;

    //Constructors
    constexpr Matrice() noexcept = default;

    explicit constexpr Matrice(Ty Scalar) noexcept
    {
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                Data[i][j] = Scalar;
            }
        }
    }

    constexpr Matrice(std::initializer_list<std::initializer_list<Ty>> List) noexcept
    {
        size_t i = 0;
        for (auto& RowList : List)
        {
            if (i >= M) break;
            size_t j = 0;
            for (auto& Val : RowList)
            {
                if (j >= N) break;
                Data[i][j] = Val;
                ++j;
            }
            ++i;
        }
    }

    constexpr Matrice(const Matrice&) noexcept = default;
    constexpr Matrice& operator=(const Matrice&) noexcept = default;
    constexpr Matrice(Matrice&&) noexcept = default;
    constexpr Matrice& operator=(Matrice&&) noexcept = default;

    //Destructors
    constexpr ~Matrice() noexcept = default;

    Matrice<M, N, Ty> operator+(const Matrice<M, N, Ty>& Other) const noexcept
    {
        Matrice<M, N, Ty> Result;
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                Result.Data[i][j] = Data[i][j] + Other.Data[i][j];
            }
        }
        return Result;
    }

    Matrice<M, N, Ty> operator-(const Matrice<M, N, Ty>& Other) const noexcept
    {
        Matrice<M, N, Ty> Result;
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                Result.Data[i][j] = Data[i][j] - Other.Data[i][j];
            }
        }
        return Result;
    }

    template <size_t O>
    Matrice<M, O, Ty> operator*(const Matrice<N, O, Ty>& Other) const noexcept
    {
        Matrice<M, O, Ty> Result;
        for (size_t i = 0; i < O; ++i)
        {
            for (size_t j = 0; j < M; ++j)
            {
                for (size_t k = 0; k < N; k++)
                {
                    Result.Data[j][i] += Data[j][k] * Other.Data[k][i];
                }
            }
        }
        return Result;
    }

    Matrice<M, N, Ty> operator*(const double Scalar) const noexcept
    {
        Matrice<M, N, Ty> Result;
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                Result.Data[i][j] = Data[i][j] * Scalar;
            }
        }
        return Result;
    }

    constexpr Matrice<M, N, Ty> operator/(const double Scalar) const noexcept
    {
        if (Scalar == 0)
        {
            std::cerr << "Can't divide by 0" << std::endl;
            return Matrice();
        }
        Matrice<M, N, Ty> Result;
        for (size_t i = 0; i < M; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                Result.Data[i][j] = Data[i][j] / Scalar;
            }
        }
        return Result;
    }

    constexpr Ty Determinant() const requires (M == N)
    {
        if constexpr (M == 1)
        {
            return Data[0][0];
        }
        else if constexpr (M == 2)
        {
            return Data[0][0] * Data[1][1] - Data[0][1] * Data[1][0];
        }
        else if constexpr (M == 3)
        {
            return Data[0][0] * (Data[1][1] * Data[2][2] - Data[1][2] * Data[2][1]) -
                   Data[0][1] * (Data[1][0] * Data[2][2] - Data[1][2] * Data[2][0]) +
                   Data[0][2] * (Data[1][0] * Data[2][1] - Data[1][1] * Data[2][0]);
        }
        else
        {
            // TODO : Pivot de Gauss
            return Ty(); 
        }
    }

    std::string Print() const
    {
        std::stringstream ss;
        for (size_t i = 0; i < M; ++i)
        {
            if (M > 1)
            {
                if (i == 0) ss << "/ ";
                else if (i == M - 1) ss << "\\ ";
                else ss << "| ";
            }
            else
            {
                ss << "[ ";
            }

            for (size_t j = 0; j < N; ++j)
            {
                ss << std::fixed << std::setprecision(2) << std::setw(8) << Data[i][j] << " ";
            }

            if (M > 1)
            {
                if (i == 0) ss << "\\";
                else if (i == M - 1) ss << "/";
                else ss << "|";
            }
            else
            {
                ss << "]";
            }
            ss << "\n";
        }
        return ss.str();
    }
};

using Mat3 = Matrice<3, 3, float>;
using Mat4 = Matrice<4, 4, float>;

#endif  // IMENGINE_MATHS_OBJECTS_MATRICE_H