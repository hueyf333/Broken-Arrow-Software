#pragma once
#include "HairStrand.h"
#include <vector>
#include <string>
#include <memory>

namespace BrokenArrow {
namespace Hair {

class Modifier {
public:
    virtual ~Modifier() = default;

    virtual std::string GetName() const = 0;
    virtual void Apply(std::vector<HairStrand>& strands) = 0;

    void SetEnabled(bool enabled) { enabled_ = enabled; }
    bool IsEnabled() const { return enabled_; }

protected:
    bool enabled_ = true;
};

class LengthModifier : public Modifier {
public:
    LengthModifier() : lengthMultiplier_(1.0f) {}

    std::string GetName() const override { return "Length"; }
    
    void Apply(std::vector<HairStrand>& strands) override {
        if (!enabled_) return;

        for (auto& strand : strands) {
            auto& points = strand.GetPoints();
            if (points.empty()) continue;

            Math::Vector3 root = points[0];
            for (size_t i = 1; i < points.size(); ++i) {
                Math::Vector3 dir = points[i] - root;
                points[i] = root + dir * lengthMultiplier_;
            }
        }
    }

    void SetLengthMultiplier(float multiplier) { lengthMultiplier_ = multiplier; }
    float GetLengthMultiplier() const { return lengthMultiplier_; }

private:
    float lengthMultiplier_;
};

class ClumpModifier : public Modifier {
public:
    ClumpModifier() : clumpStrength_(0.5f) {}

    std::string GetName() const override { return "Clump"; }

    void Apply(std::vector<HairStrand>& strands) override {
        if (!enabled_ || strands.empty()) return;

        size_t clumpSize = std::max(size_t(1), strands.size() / 10);

        for (size_t i = 0; i < strands.size(); i += clumpSize) {
            size_t clumpEnd = std::min(i + clumpSize, strands.size());
            ApplyClump(strands, i, clumpEnd);
        }
    }

    void SetClumpStrength(float strength) { clumpStrength_ = strength; }
    float GetClumpStrength() const { return clumpStrength_; }

private:
    void ApplyClump(std::vector<HairStrand>& strands, size_t start, size_t end) {
        if (start >= end) return;

        size_t centerIdx = start + (end - start) / 2;
        const auto& centerStrand = strands[centerIdx];
        size_t maxPoints = centerStrand.GetPointCount();

        for (size_t i = start; i < end; ++i) {
            if (i == centerIdx) continue;

            auto& strand = strands[i];
            auto& points = strand.GetPoints();
            size_t pointCount = std::min(points.size(), maxPoints);

            for (size_t j = 0; j < pointCount; ++j) {
                float t = static_cast<float>(j) / static_cast<float>(pointCount - 1);
                Math::Vector3 centerPoint = centerStrand.GetPoint(j);
                points[j] = Math::Vector3::Lerp(points[j], centerPoint, clumpStrength_ * t * t);
            }
        }
    }

    float clumpStrength_;
};

class NoiseModifier : public Modifier {
public:
    NoiseModifier() : noiseStrength_(0.3f), noiseScale_(5.0f) {}

    std::string GetName() const override { return "Noise"; }

    void Apply(std::vector<HairStrand>& strands) override {
        if (!enabled_) return;

        for (auto& strand : strands) {
            auto& points = strand.GetPoints();
            for (size_t i = 1; i < points.size(); ++i) {
                float t = static_cast<float>(i) / static_cast<float>(points.size() - 1);
                Math::Vector3 noiseOffset = Math::Vector3(
                    Math::Noise::Perlin(points[i] * noiseScale_),
                    Math::Noise::Perlin(points[i] * noiseScale_ + Math::Vector3(100, 0, 0)),
                    Math::Noise::Perlin(points[i] * noiseScale_ + Math::Vector3(0, 100, 0))
                ) * 2.0f - Math::Vector3::One();

                points[i] += noiseOffset * noiseStrength_ * t;
            }
        }
    }

    void SetNoiseStrength(float strength) { noiseStrength_ = strength; }
    float GetNoiseStrength() const { return noiseStrength_; }

    void SetNoiseScale(float scale) { noiseScale_ = scale; }
    float GetNoiseScale() const { return noiseScale_; }

private:
    float noiseStrength_;
    float noiseScale_;
};

class GravityModifier : public Modifier {
public:
    GravityModifier() : gravityStrength_(0.5f), direction_(Math::Vector3(0, -1, 0)) {}

    std::string GetName() const override { return "Gravity"; }

    void Apply(std::vector<HairStrand>& strands) override {
        if (!enabled_) return;

        for (auto& strand : strands) {
            auto& points = strand.GetPoints();
            for (size_t i = 1; i < points.size(); ++i) {
                float t = static_cast<float>(i) / static_cast<float>(points.size() - 1);
                points[i] += direction_ * gravityStrength_ * t * t;
            }
        }
    }

    void SetGravityStrength(float strength) { gravityStrength_ = strength; }
    float GetGravityStrength() const { return gravityStrength_; }

    void SetDirection(const Math::Vector3& dir) { direction_ = dir.Normalized(); }
    const Math::Vector3& GetDirection() const { return direction_; }

private:
    float gravityStrength_;
    Math::Vector3 direction_;
};

} // namespace Hair
} // namespace BrokenArrow
