#pragma once
#include "Spline.h"
#include "Vector3.h"
#include <vector>

namespace BrokenArrow {
namespace Hair {

class GuideCurve {
public:
    GuideCurve() : rootPosition_(Math::Vector3::Zero()), rootNormal_(Math::Vector3::UnitY()) {
        spline_ = Math::Spline(Math::Spline::Type::CatmullRom);
    }

    void SetRootPosition(const Math::Vector3& pos) { rootPosition_ = pos; }
    void SetRootNormal(const Math::Vector3& normal) { rootNormal_ = normal; }
    
    const Math::Vector3& GetRootPosition() const { return rootPosition_; }
    const Math::Vector3& GetRootNormal() const { return rootNormal_; }

    void AddControlPoint(const Math::Vector3& point) {
        spline_.AddControlPoint(point);
    }

    void SetControlPoint(size_t index, const Math::Vector3& point) {
        spline_.SetControlPoint(index, point);
    }

    const Math::Vector3& GetControlPoint(size_t index) const {
        return spline_.GetControlPoint(index);
    }

    size_t GetControlPointCount() const {
        return spline_.GetControlPointCount();
    }

    Math::Vector3 Evaluate(float t) const {
        return spline_.Evaluate(t);
    }

    Math::Vector3 EvaluateByArcLength(float distance) const {
        return spline_.EvaluateByArcLength(distance);
    }

    Math::Vector3 GetTangent(float t) const {
        return spline_.GetTangent(t);
    }

    float GetTotalLength() const {
        return spline_.GetTotalLength();
    }

    void ClearControlPoints() {
        spline_.ClearControlPoints();
    }

    Math::Spline& GetSpline() { return spline_; }
    const Math::Spline& GetSpline() const { return spline_; }

private:
    Math::Spline spline_;
    Math::Vector3 rootPosition_;
    Math::Vector3 rootNormal_;
};

} // namespace Hair
} // namespace BrokenArrow
