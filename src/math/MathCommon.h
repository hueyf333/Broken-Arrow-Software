#pragma once
#include <cmath>
#include <algorithm>

namespace BrokenArrow {
namespace Math {

constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 2.0f * PI;
constexpr float HALF_PI = 0.5f * PI;
constexpr float EPSILON = 1e-6f;

inline float Clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float Deg2Rad(float degrees) {
    return degrees * PI / 180.0f;
}

inline float Rad2Deg(float radians) {
    return radians * 180.0f / PI;
}

inline float Smoothstep(float edge0, float edge1, float x) {
    float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

inline bool NearZero(float value, float epsilon = EPSILON) {
    return std::abs(value) < epsilon;
}

inline bool NearEqual(float a, float b, float epsilon = EPSILON) {
    return std::abs(a - b) < epsilon;
}

} // namespace Math
} // namespace BrokenArrow
