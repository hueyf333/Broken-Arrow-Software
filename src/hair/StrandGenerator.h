#pragma once
#include "GuideCurve.h"
#include "HairStrand.h"
#include "Random.h"
#include <vector>
#include <memory>

namespace BrokenArrow {
namespace Hair {

struct StrandGeneratorParams {
    int density = 10000;
    float length = 10.0f;
    float lengthVariation = 0.1f;
    int segments = 16;
    float widthRoot = 0.02f;
    float widthTip = 0.001f;
    uint32_t seed = 12345;
};

class StrandGenerator {
public:
    StrandGenerator() = default;

    std::vector<HairStrand> Generate(const std::vector<GuideCurve>& guides, 
                                     const StrandGeneratorParams& params) {
        std::vector<HairStrand> strands;
        if (guides.empty()) return strands;

        Math::Random rng(params.seed);
        strands.reserve(params.density);

        for (int i = 0; i < params.density; ++i) {
            HairStrand strand = GenerateStrand(guides, params, rng);
            strands.push_back(strand);
        }

        return strands;
    }

private:
    HairStrand GenerateStrand(const std::vector<GuideCurve>& guides,
                              const StrandGeneratorParams& params,
                              Math::Random& rng) {
        
        int guideIndex = rng.RangeInt(0, static_cast<int>(guides.size()) - 1);
        const GuideCurve& guide = guides[guideIndex];

        float lengthMultiplier = 1.0f + rng.Range(-params.lengthVariation, params.lengthVariation);
        float strandLength = params.length * lengthMultiplier;

        HairStrand strand;
        strand.SetWidth((params.widthRoot + params.widthTip) * 0.5f);

        Math::Vector3 rootOffset = Math::Vector3(
            rng.Range(-0.5f, 0.5f),
            rng.Range(-0.5f, 0.5f),
            rng.Range(-0.5f, 0.5f)
        ) * 0.1f;

        for (int seg = 0; seg <= params.segments; ++seg) {
            float t = static_cast<float>(seg) / static_cast<float>(params.segments);
            
            Math::Vector3 guidePos = guide.Evaluate(t);
            
            Math::Vector3 noise = Math::Vector3(
                rng.Range(-1.0f, 1.0f),
                rng.Range(-1.0f, 1.0f),
                rng.Range(-1.0f, 1.0f)
            ) * 0.05f * t;

            Math::Vector3 point = guidePos + rootOffset * (1.0f - t) + noise;
            strand.AddPoint(point);
        }

        return strand;
    }
};

} // namespace Hair
} // namespace BrokenArrow
