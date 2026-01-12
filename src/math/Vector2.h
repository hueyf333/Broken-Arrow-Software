#pragma once
#include "MathCommon.h"
#include <cmath>

namespace BrokenArrow {
namespace Math {

class Vector2 {
public:
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }
    
    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
    Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    float Dot(const Vector2& other) const { return x * other.x + y * other.y; }
    float Length() const { return std::sqrt(x * x + y * y); }
    float LengthSquared() const { return x * x + y * y; }
    
    Vector2 Normalized() const {
        float len = Length();
        return NearZero(len) ? Vector2(0, 0) : Vector2(x / len, y / len);
    }

    void Normalize() {
        float len = Length();
        if (!NearZero(len)) {
            x /= len;
            y /= len;
        }
    }

    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
        return a + (b - a) * t;
    }

    static float Distance(const Vector2& a, const Vector2& b) {
        return (b - a).Length();
    }
};

} // namespace Math
} // namespace BrokenArrow
