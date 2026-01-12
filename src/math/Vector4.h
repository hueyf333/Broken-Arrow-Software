#pragma once
#include "MathCommon.h"
#include <cmath>

namespace BrokenArrow {
namespace Math {

class Vector4 {
public:
    float x, y, z, w;

    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(float value) : x(value), y(value), z(value), w(value) {}

    Vector4 operator+(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vector4 operator-(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vector4 operator*(float scalar) const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vector4 operator/(float scalar) const { return Vector4(x / scalar, y / scalar, z / scalar, w / scalar); }

    float Dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
    float Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    float LengthSquared() const { return x * x + y * y + z * z + w * w; }

    Vector4 Normalized() const {
        float len = Length();
        return NearZero(len) ? Vector4(0, 0, 0, 0) : Vector4(x / len, y / len, z / len, w / len);
    }
};

} // namespace Math
} // namespace BrokenArrow
