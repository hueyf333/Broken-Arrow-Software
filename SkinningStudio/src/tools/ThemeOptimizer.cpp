#include "tools/ThemeOptimizer.h"
#include <unordered_set>

OptimizationResult ThemeOptimizer::OptimizeTheme(
    std::shared_ptr<Theme> theme,
    const std::vector<std::shared_ptr<UIElement>>& rootElements
) {
    OptimizationResult result;
    
    if (!theme) return result;
    
    // Collect all tokens used in widget defaults
    std::unordered_set<std::string> usedTokens;
    
    for (const auto& [widgetType, style] : theme->widgetDefaults) {
        for (const auto& [prop, value] : style.properties) {
            if (!value.empty() && value[0] == '$') {
                std::string tokenName = value.substr(1);
                usedTokens.insert(tokenName);
            }
        }
    }
    
    // Collect tokens used in elements
    CollectUsedTokens(rootElements, usedTokens);
    
    // Find unused tokens
    for (const auto& [tokenName, token] : theme->tokens) {
        if (usedTokens.find(tokenName) == usedTokens.end()) {
            result.unusedTokens.push_back(tokenName);
            result.tokensSaved++;
        }
    }
    
    return result;
}

void ThemeOptimizer::CollectUsedTokens(
    const std::vector<std::shared_ptr<UIElement>>& elements,
    std::unordered_set<std::string>& usedTokens
) {
    for (const auto& elem : elements) {
        for (const auto& [state, overrides] : elem->stateOverrides) {
            for (const auto& [prop, value] : overrides) {
                if (!value.empty() && value[0] == '$') {
                    std::string tokenName = value.substr(1);
                    usedTokens.insert(tokenName);
                }
            }
        }
        
        CollectUsedTokens(elem->children, usedTokens);
    }
}
