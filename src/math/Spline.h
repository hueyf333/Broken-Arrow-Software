#pragma once
#include "Vector3.h"
#include <vector>
#include <cmath>

namespace BrokenArrow {
namespace Math {

class Spline {
public:
    enum class Type {
        CatmullRom,
        CubicBezier
    };

    Spline(Type type = Type::CatmullRom) : type_(type) {}

    void AddControlPoint(const Vector3& point) {
        controlPoints_.push_back(point);
        arcLengthsDirty_ = true;
    }

    void SetControlPoint(size_t index, const Vector3& point) {
        if (index < controlPoints_.size()) {
            controlPoints_[index] = point;
            arcLengthsDirty_ = true;
        }
    }

    const Vector3& GetControlPoint(size_t index) const {
        return controlPoints_[index];
    }

    size_t GetControlPointCount() const {
        return controlPoints_.size();
    }

    void ClearControlPoints() {
        controlPoints_.clear();
        arcLengthsDirty_ = true;
    }

    Vector3 Evaluate(float t) const {
        if (controlPoints_.size() < 2) return Vector3::Zero();
        
        t = Clamp(t, 0.0f, 1.0f);

        if (type_ == Type::CatmullRom) {
            return EvaluateCatmullRom(t);
        } else {
            return EvaluateBezier(t);
        }
    }

    Vector3 EvaluateByArcLength(float distance) const {
        if (controlPoints_.size() < 2) return Vector3::Zero();
        
        UpdateArcLengths();
        
        if (arcLengths_.empty() || distance <= 0.0f) {
            return controlPoints_[0];
        }
        
        float totalLength = arcLengths_.back();
        if (distance >= totalLength) {
            return controlPoints_.back();
        }

        for (size_t i = 0; i < arcLengths_.size() - 1; ++i) {
            if (distance >= arcLengths_[i] && distance < arcLengths_[i + 1]) {
                float segmentLength = arcLengths_[i + 1] - arcLengths_[i];
                float localT = (distance - arcLengths_[i]) / segmentLength;
                float globalT = (float(i) + localT) / float(arcLengths_.size() - 1);
                return Evaluate(globalT);
            }
        }

        return Evaluate(1.0f);
    }

    Vector3 GetTangent(float t) const {
        if (controlPoints_.size() < 2) return Vector3::UnitZ();
        
        float delta = 0.001f;
        float t1 = std::max(0.0f, t - delta);
        float t2 = std::min(1.0f, t + delta);
        
        Vector3 p1 = Evaluate(t1);
        Vector3 p2 = Evaluate(t2);
        
        return (p2 - p1).Normalized();
    }

    float GetTotalLength() const {
        UpdateArcLengths();
        return arcLengths_.empty() ? 0.0f : arcLengths_.back();
    }

private:
    Vector3 EvaluateCatmullRom(float t) const {
        size_t n = controlPoints_.size();
        if (n == 1) return controlPoints_[0];
        if (n == 2) return Vector3::Lerp(controlPoints_[0], controlPoints_[1], t);

        float scaledT = t * float(n - 1);
        size_t segment = static_cast<size_t>(scaledT);
        segment = std::min(segment, n - 2);
        float localT = scaledT - float(segment);

        size_t p0 = (segment == 0) ? segment : segment - 1;
        size_t p1 = segment;
        size_t p2 = segment + 1;
        size_t p3 = (segment + 2 >= n) ? n - 1 : segment + 2;

        float tt = localT * localT;
        float ttt = tt * localT;

        Vector3 q0 = controlPoints_[p0];
        Vector3 q1 = controlPoints_[p1];
        Vector3 q2 = controlPoints_[p2];
        Vector3 q3 = controlPoints_[p3];

        return q1 * (2.0f * tt - ttt - localT) * 0.5f +
               q2 * (3.0f * ttt - 5.0f * tt + 2.0f) * 0.5f +
               q0 * (ttt - tt) * -0.5f +
               q3 * (ttt - tt) * 0.5f +
               q1;
    }

    Vector3 EvaluateBezier(float t) const {
        size_t n = controlPoints_.size();
        if (n == 0) return Vector3::Zero();
        if (n == 1) return controlPoints_[0];
        
        std::vector<Vector3> points = controlPoints_;
        
        while (points.size() > 1) {
            std::vector<Vector3> nextLevel;
            for (size_t i = 0; i < points.size() - 1; ++i) {
                nextLevel.push_back(Vector3::Lerp(points[i], points[i + 1], t));
            }
            points = nextLevel;
        }
        
        return points[0];
    }

    void UpdateArcLengths() const {
        if (!arcLengthsDirty_) return;
        
        arcLengths_.clear();
        if (controlPoints_.size() < 2) {
            arcLengthsDirty_ = false;
            return;
        }

        int samples = 100;
        float totalLength = 0.0f;
        arcLengths_.push_back(0.0f);

        Vector3 prevPoint = controlPoints_[0];
        for (int i = 1; i <= samples; ++i) {
            float t = float(i) / float(samples);
            Vector3 currentPoint = Evaluate(t);
            totalLength += Vector3::Distance(prevPoint, currentPoint);
            arcLengths_.push_back(totalLength);
            prevPoint = currentPoint;
        }

        arcLengthsDirty_ = false;
    }

    Type type_;
    std::vector<Vector3> controlPoints_;
    mutable std::vector<float> arcLengths_;
    mutable bool arcLengthsDirty_ = true;
};

} // namespace Math
} // namespace BrokenArrow
