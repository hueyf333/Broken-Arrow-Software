#pragma once
#include "Vector3.h"
#include "HairStrand.h"
#include <vector>

namespace BrokenArrow {
namespace Hair {

enum class BrushType {
    Comb,
    Smooth,
    Inflate,
    Cut,
    Grow,
    Clump
};

struct BrushParams {
    BrushType type = BrushType::Comb;
    float radius = 5.0f;
    float strength = 0.3f;
    float falloff = 1.0f;
    bool xSymmetry = false;
};

class Brush {
public:
    Brush() = default;

    void ApplyStroke(std::vector<HairStrand>& strands, 
                     const Math::Vector3& position,
                     const Math::Vector3& direction,
                     const BrushParams& params) {
        
        for (auto& strand : strands) {
            ApplyToStrand(strand, position, direction, params);
        }
    }

private:
    void ApplyToStrand(HairStrand& strand,
                       const Math::Vector3& position,
                       const Math::Vector3& direction,
                       const BrushParams& params) {
        
        auto& points = strand.GetPoints();
        
        for (size_t i = 0; i < points.size(); ++i) {
            float distance = Math::Vector3::Distance(points[i], position);
            
            if (distance > params.radius) continue;

            float influence = 1.0f - (distance / params.radius);
            influence = std::pow(influence, params.falloff);
            influence *= params.strength;

            switch (params.type) {
                case BrushType::Comb:
                    points[i] += direction * influence;
                    break;

                case BrushType::Smooth: {
                    if (i > 0 && i < points.size() - 1) {
                        Math::Vector3 avg = (points[i - 1] + points[i + 1]) * 0.5f;
                        points[i] = Math::Vector3::Lerp(points[i], avg, influence);
                    }
                    break;
                }

                case BrushType::Inflate: {
                    Math::Vector3 normal = (points[i] - position).Normalized();
                    points[i] += normal * influence;
                    break;
                }

                case BrushType::Cut: {
                    float targetLength = distance;
                    float currentLength = strand.GetLength();
                    if (currentLength > targetLength && i > static_cast<size_t>(targetLength / currentLength * points.size())) {
                        points.resize(i);
                        break;
                    }
                    break;
                }

                case BrushType::Grow:
                    points[i] += direction * influence * 0.1f;
                    break;

                case BrushType::Clump:
                    points[i] = Math::Vector3::Lerp(points[i], position, influence * 0.5f);
                    break;
            }
        }
    }
};

} // namespace Hair
} // namespace BrokenArrow
