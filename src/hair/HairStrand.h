#pragma once
#include "Vector3.h"
#include <vector>

namespace BrokenArrow {
namespace Hair {

class HairStrand {
public:
    HairStrand() : width_(0.01f) {}

    void AddPoint(const Math::Vector3& point) {
        points_.push_back(point);
    }

    void SetPoint(size_t index, const Math::Vector3& point) {
        if (index < points_.size()) {
            points_[index] = point;
        }
    }

    const Math::Vector3& GetPoint(size_t index) const {
        return points_[index];
    }

    size_t GetPointCount() const {
        return points_.size();
    }

    void SetWidth(float width) { width_ = width; }
    float GetWidth() const { return width_; }

    void Clear() { points_.clear(); }

    const std::vector<Math::Vector3>& GetPoints() const { return points_; }
    std::vector<Math::Vector3>& GetPoints() { return points_; }

    Math::Vector3 Evaluate(float t) const {
        if (points_.empty()) return Math::Vector3::Zero();
        if (points_.size() == 1) return points_[0];

        float scaledT = t * (points_.size() - 1);
        size_t index = static_cast<size_t>(scaledT);
        if (index >= points_.size() - 1) return points_.back();

        float localT = scaledT - index;
        return Math::Vector3::Lerp(points_[index], points_[index + 1], localT);
    }

    float GetLength() const {
        float length = 0.0f;
        for (size_t i = 1; i < points_.size(); ++i) {
            length += Math::Vector3::Distance(points_[i - 1], points_[i]);
        }
        return length;
    }

private:
    std::vector<Math::Vector3> points_;
    float width_;
};

} // namespace Hair
} // namespace BrokenArrow
