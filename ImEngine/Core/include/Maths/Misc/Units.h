#ifndef IMENGINE_CORE_MATHS_MISC_UNITS_H
#define IMENGINE_CORE_MATHS_MISC_UNITS_H

namespace ImEngine::Core::Maths::Units {

// Angle units
struct Degree;
struct uDegree;
struct Radian;
struct uRadian;

// Transform types for Mat4 * Vec3

// w=1: affected by translation
struct Point {}; 

// w=0: ignores translation
struct Direction {};  

}  // namespace ImEngine::Core::Maths::Units

#endif  // IMENGINE_CORE_MATHS_MISC_UNITS_H