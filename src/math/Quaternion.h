#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

namespace BrokenArrow {
namespace Math {

class Quaternion {
public:
    float x, y, z, w;

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    static Quaternion FromAxisAngle(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        Vector3 normalizedAxis = axis.Normalized();
        return Quaternion(
            normalizedAxis.x * s,
            normalizedAxis.y * s,
            normalizedAxis.z * s,
            std::cos(halfAngle)
        );
    }

    static Quaternion FromEuler(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        return Quaternion(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y + y * other.w + z * other.x - x * other.z,
            w * other.z + z * other.w + x * other.y - y * other.x,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Quaternion Normalized() const {
        float len = Length();
        if (NearZero(len)) return Quaternion();
        return Quaternion(x / len, y / len, z / len, w / len);
    }

    void Normalize() {
        float len = Length();
        if (!NearZero(len)) {
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    }

    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    Vector3 RotateVector(const Vector3& v) const {
        Quaternion qv(v.x, v.y, v.z, 0.0f);
        Quaternion result = (*this) * qv * Conjugate();
        return Vector3(result.x, result.y, result.z);
    }

    Matrix4x4 ToMatrix() const {
        Matrix4x4 result;
        
        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        result.m[0][0] = 1.0f - 2.0f * (yy + zz);
        result.m[0][1] = 2.0f * (xy - wz);
        result.m[0][2] = 2.0f * (xz + wy);
        result.m[0][3] = 0.0f;

        result.m[1][0] = 2.0f * (xy + wz);
        result.m[1][1] = 1.0f - 2.0f * (xx + zz);
        result.m[1][2] = 2.0f * (yz - wx);
        result.m[1][3] = 0.0f;

        result.m[2][0] = 2.0f * (xz - wy);
        result.m[2][1] = 2.0f * (yz + wx);
        result.m[2][2] = 1.0f - 2.0f * (xx + yy);
        result.m[2][3] = 0.0f;

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        
        Quaternion b2 = b;
        if (dot < 0.0f) {
            b2 = Quaternion(-b.x, -b.y, -b.z, -b.w);
            dot = -dot;
        }

        if (dot > 0.9995f) {
            return Quaternion(
                Lerp(a.x, b2.x, t),
                Lerp(a.y, b2.y, t),
                Lerp(a.z, b2.z, t),
                Lerp(a.w, b2.w, t)
            ).Normalized();
        }

        float theta = std::acos(dot);
        float sinTheta = std::sin(theta);
        float wa = std::sin((1.0f - t) * theta) / sinTheta;
        float wb = std::sin(t * theta) / sinTheta;

        return Quaternion(
            a.x * wa + b2.x * wb,
            a.y * wa + b2.y * wb,
            a.z * wa + b2.z * wb,
            a.w * wa + b2.w * wb
        );
    }

    static Quaternion Identity() {
        return Quaternion(0, 0, 0, 1);
    }
};

} // namespace Math
} // namespace BrokenArrow
