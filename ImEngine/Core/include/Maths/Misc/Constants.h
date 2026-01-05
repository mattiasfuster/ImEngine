#ifndef IMENGINE_CORE_MATHS_MISC_CONSTANTS_H
#define IMENGINE_CORE_MATHS_MISC_CONSTANTS_H

#include <numbers>
#include <concepts>

namespace ImEngine::Core::Maths::Constants
{
    template<std::floating_point T = float>
    inline constexpr T PI = std::numbers::pi_v<T>;

    template<std::floating_point T = float>
    inline constexpr T RadToDeg = static_cast<T>(180.0) / PI<T>;

    template<std::floating_point T = float>
    inline constexpr T DegToRad = PI<T> / static_cast<T>(180.0);
}

#endif  // IMENGINE_CORE_MATHS_MISC_CONSTANTS_H