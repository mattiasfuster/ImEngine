#ifndef IMENGINE_MATHS_OBJECTS_MATRICE_H
#define IMENGINE_MATHS_OBJECTS_MATRICE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <initializer_list>

template<size_t M, size_t N, class T>
struct Matrice
{
    T Data[M][N]{};
    static constexpr size_t Col = N;
    static constexpr size_t Row = M;

    //Constructors
    constexpr Matrice() noexcept = default;

    constexpr Matrice(T Scalar) noexcept
    {
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                Data[i][j] = Scalar;
    }

    constexpr Matrice(std::initializer_list<std::initializer_list<T>> List) noexcept
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

#endif  // IMENGINE_MATHS_OBJECTS_MATRICE_H