#pragma once
#include "Modifier.h"
#include <vector>
#include <memory>
#include <algorithm>

namespace BrokenArrow {
namespace Hair {

class ModifierStack {
public:
    ModifierStack() = default;

    void AddModifier(std::unique_ptr<Modifier> modifier) {
        modifiers_.push_back(std::move(modifier));
    }

    void RemoveModifier(size_t index) {
        if (index < modifiers_.size()) {
            modifiers_.erase(modifiers_.begin() + index);
        }
    }

    void MoveModifier(size_t from, size_t to) {
        if (from >= modifiers_.size() || to >= modifiers_.size()) return;
        
        auto modifier = std::move(modifiers_[from]);
        modifiers_.erase(modifiers_.begin() + from);
        modifiers_.insert(modifiers_.begin() + to, std::move(modifier));
    }

    void Apply(std::vector<HairStrand>& strands) {
        for (auto& modifier : modifiers_) {
            if (modifier && modifier->IsEnabled()) {
                modifier->Apply(strands);
            }
        }
    }

    size_t GetModifierCount() const {
        return modifiers_.size();
    }

    Modifier* GetModifier(size_t index) {
        if (index < modifiers_.size()) {
            return modifiers_[index].get();
        }
        return nullptr;
    }

    void Clear() {
        modifiers_.clear();
    }

private:
    std::vector<std::unique_ptr<Modifier>> modifiers_;
};

} // namespace Hair
} // namespace BrokenArrow
