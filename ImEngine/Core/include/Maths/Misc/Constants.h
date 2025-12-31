#ifndef IMENGINE_CORE_MATHS_MISC_CONSTANTS_H
#define IMENGINE_CORE_MATHS_MISC_CONSTANTS_H

#include <numbers>

namespace ImEngine::Core::Maths::Constants
{
    inline constexpr double PI = std::numbers::pi;
    inline constexpr double RadToDeg = 180.0 / PI;
    inline constexpr double DegToRad = PI / 180.0;

}

#endif  // IMENGINE_CORE_MATHS_MISC_CONSTANTS_H