#pragma once

#include "core/StyleSystem.h"
#include "core/UIElement.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>

struct OptimizationResult {
    std::vector<std::string> unusedTokens;
    std::vector<std::string> redundantOverrides;
    int tokensSaved = 0;
};

class ThemeOptimizer {
public:
    OptimizationResult OptimizeTheme(
        std::shared_ptr<Theme> theme,
        const std::vector<std::shared_ptr<UIElement>>& rootElements
    );
    
private:
    void CollectUsedTokens(
        const std::vector<std::shared_ptr<UIElement>>& elements,
        std::unordered_set<std::string>& usedTokens
    );
};
