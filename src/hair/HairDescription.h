#pragma once
#include "GuideCurve.h"
#include "HairStrand.h"
#include "ModifierStack.h"
#include "StrandGenerator.h"
#include "MeshImporter.h"
#include <vector>
#include <memory>
#include <string>

namespace BrokenArrow {
namespace Hair {

class HairDescription {
public:
    HairDescription() : name_("Hair"), scalpMeshName_("") {}

    void SetName(const std::string& name) { name_ = name; }
    const std::string& GetName() const { return name_; }

    void SetScalpMesh(const std::string& meshName) { scalpMeshName_ = meshName; }
    const std::string& GetScalpMesh() const { return scalpMeshName_; }

    void AddGuide(const GuideCurve& guide) {
        guides_.push_back(guide);
    }

    void RemoveGuide(size_t index) {
        if (index < guides_.size()) {
            guides_.erase(guides_.begin() + index);
        }
    }

    GuideCurve& GetGuide(size_t index) {
        return guides_[index];
    }

    const GuideCurve& GetGuide(size_t index) const {
        return guides_[index];
    }

    size_t GetGuideCount() const {
        return guides_.size();
    }

    const std::vector<GuideCurve>& GetGuides() const {
        return guides_;
    }

    void GenerateStrands(const StrandGeneratorParams& params) {
        if (guides_.empty()) return;

        StrandGenerator generator;
        strands_ = generator.Generate(guides_, params);
        
        modifierStack_.Apply(strands_);
    }

    const std::vector<HairStrand>& GetStrands() const {
        return strands_;
    }

    std::vector<HairStrand>& GetStrands() {
        return strands_;
    }

    ModifierStack& GetModifierStack() {
        return modifierStack_;
    }

    const ModifierStack& GetModifierStack() const {
        return modifierStack_;
    }

    void ApplyModifiers() {
        modifierStack_.Apply(strands_);
    }

    void ClearGuides() {
        guides_.clear();
    }

    void ClearStrands() {
        strands_.clear();
    }

private:
    std::string name_;
    std::string scalpMeshName_;
    std::vector<GuideCurve> guides_;
    std::vector<HairStrand> strands_;
    ModifierStack modifierStack_;
};

} // namespace Hair
} // namespace BrokenArrow
