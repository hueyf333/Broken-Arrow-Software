#pragma once

#include "UIElement.h"
#include <map>
#include <string>
#include <nlohmann/json.hpp>

namespace SkinningStudio {

struct ColorToken {
    std::string name;
    Color value;
};

struct SpacingToken {
    std::string name;
    float value;
};

struct FontToken {
    std::string name;
    std::string fontPath;
    float fontSize;
};

class Theme {
public:
    Theme();
    ~Theme();

    // Color tokens
    void AddColorToken(const std::string& name, const Color& color);
    Color GetColorToken(const std::string& name, const Color& defaultValue = Color()) const;
    const std::map<std::string, Color>& GetColorTokens() const { return m_colorTokens; }

    // Spacing tokens
    void AddSpacingToken(const std::string& name, float value);
    float GetSpacingToken(const std::string& name, float defaultValue = 0.0f) const;
    const std::map<std::string, float>& GetSpacingTokens() const { return m_spacingTokens; }

    // Font tokens
    void AddFontToken(const std::string& name, const std::string& path, float size);
    FontToken GetFontToken(const std::string& name) const;
    const std::map<std::string, FontToken>& GetFontTokens() const { return m_fontTokens; }

    // Widget defaults
    void SetWidgetDefault(WidgetType type, const std::string& property, const std::string& value);
    std::string GetWidgetDefault(WidgetType type, const std::string& property) const;

    // Style resolution
    std::string ResolveStyle(WidgetType type, const std::string& property, 
                            const std::map<std::string, std::string>& overrides = {},
                            WidgetState state = WidgetState::Normal,
                            const std::map<WidgetState, std::map<std::string, std::string>>& stateOverrides = {}) const;

    // Serialization
    nlohmann::json Serialize() const;
    void Deserialize(const nlohmann::json& json);

    // Optimization
    struct OptimizationReport {
        std::vector<std::string> unusedTokens;
        std::vector<std::string> unusedOverrides;
        int tokensRemoved;
    };
    OptimizationReport Optimize();

    // Create default theme
    static std::unique_ptr<Theme> CreateDefaultDarkTheme();

private:
    std::map<std::string, Color> m_colorTokens;
    std::map<std::string, float> m_spacingTokens;
    std::map<std::string, FontToken> m_fontTokens;
    std::map<WidgetType, std::map<std::string, std::string>> m_widgetDefaults;
};

} // namespace SkinningStudio
