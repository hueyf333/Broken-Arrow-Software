#pragma once
#include "MathCommon.h"
#include <cmath>

namespace BrokenArrow {
namespace Math {

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(float value) : x(value), y(value), z(value) {}

    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
    Vector3 operator-() const { return Vector3(-x, -y, -z); }
    
    Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vector3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3& operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    bool operator==(const Vector3& other) const {
        return NearEqual(x, other.x) && NearEqual(y, other.y) && NearEqual(z, other.z);
    }
    bool operator!=(const Vector3& other) const { return !(*this == other); }

    float Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    
    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    float LengthSquared() const { return x * x + y * y + z * z; }
    
    Vector3 Normalized() const {
        float len = Length();
        return NearZero(len) ? Vector3(0, 0, 0) : Vector3(x / len, y / len, z / len);
    }

    void Normalize() {
        float len = Length();
        if (!NearZero(len)) {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
        return a + (b - a) * t;
    }

    static float Distance(const Vector3& a, const Vector3& b) {
        return (b - a).Length();
    }

    static float DistanceSquared(const Vector3& a, const Vector3& b) {
        return (b - a).LengthSquared();
    }

    static Vector3 Reflect(const Vector3& incident, const Vector3& normal) {
        return incident - normal * (2.0f * incident.Dot(normal));
    }

    static Vector3 Project(const Vector3& vector, const Vector3& onNormal) {
        float sqrLen = onNormal.LengthSquared();
        if (NearZero(sqrLen)) return Vector3(0, 0, 0);
        return onNormal * (vector.Dot(onNormal) / sqrLen);
    }

    // Common constants
    static Vector3 Zero() { return Vector3(0, 0, 0); }
    static Vector3 One() { return Vector3(1, 1, 1); }
    static Vector3 UnitX() { return Vector3(1, 0, 0); }
    static Vector3 UnitY() { return Vector3(0, 1, 0); }
    static Vector3 UnitZ() { return Vector3(0, 0, 1); }
    static Vector3 Up() { return Vector3(0, 1, 0); }
    static Vector3 Down() { return Vector3(0, -1, 0); }
    static Vector3 Right() { return Vector3(1, 0, 0); }
    static Vector3 Left() { return Vector3(-1, 0, 0); }
    static Vector3 Forward() { return Vector3(0, 0, 1); }
    static Vector3 Back() { return Vector3(0, 0, -1); }
};

inline Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
}

} // namespace Math
} // namespace BrokenArrow
