#include "Theme.h"

namespace SkinningStudio {

Theme::Theme() {
}

Theme::~Theme() = default;

void Theme::AddColorToken(const std::string& name, const Color& color) {
    m_colorTokens[name] = color;
}

Color Theme::GetColorToken(const std::string& name, const Color& defaultValue) const {
    auto it = m_colorTokens.find(name);
    if (it != m_colorTokens.end()) {
        return it->second;
    }
    return defaultValue;
}

void Theme::AddSpacingToken(const std::string& name, float value) {
    m_spacingTokens[name] = value;
}

float Theme::GetSpacingToken(const std::string& name, float defaultValue) const {
    auto it = m_spacingTokens.find(name);
    if (it != m_spacingTokens.end()) {
        return it->second;
    }
    return defaultValue;
}

void Theme::AddFontToken(const std::string& name, const std::string& path, float size) {
    FontToken token;
    token.name = name;
    token.fontPath = path;
    token.fontSize = size;
    m_fontTokens[name] = token;
}

FontToken Theme::GetFontToken(const std::string& name) const {
    auto it = m_fontTokens.find(name);
    if (it != m_fontTokens.end()) {
        return it->second;
    }
    return FontToken();
}

void Theme::SetWidgetDefault(WidgetType type, const std::string& property, const std::string& value) {
    m_widgetDefaults[type][property] = value;
}

std::string Theme::GetWidgetDefault(WidgetType type, const std::string& property) const {
    auto typeIt = m_widgetDefaults.find(type);
    if (typeIt != m_widgetDefaults.end()) {
        auto propIt = typeIt->second.find(property);
        if (propIt != typeIt->second.end()) {
            return propIt->second;
        }
    }
    return "";
}

std::string Theme::ResolveStyle(WidgetType type, const std::string& property,
                               const std::map<std::string, std::string>& overrides,
                               WidgetState state,
                               const std::map<WidgetState, std::map<std::string, std::string>>& stateOverrides) const {
    // Priority: state override > element override > widget default > theme token
    
    // Check state override
    if (state != WidgetState::Normal) {
        auto stateIt = stateOverrides.find(state);
        if (stateIt != stateOverrides.end()) {
            auto propIt = stateIt->second.find(property);
            if (propIt != stateIt->second.end()) {
                return propIt->second;
            }
        }
    }
    
    // Check element override
    auto overrideIt = overrides.find(property);
    if (overrideIt != overrides.end()) {
        return overrideIt->second;
    }
    
    // Check widget default
    std::string widgetDefault = GetWidgetDefault(type, property);
    if (!widgetDefault.empty()) {
        return widgetDefault;
    }
    
    return "";
}

nlohmann::json Theme::Serialize() const {
    nlohmann::json json;
    
    // Color tokens
    nlohmann::json colors = nlohmann::json::object();
    for (const auto& [name, color] : m_colorTokens) {
        colors[name] = color.ToHex();
    }
    json["colors"] = colors;
    
    // Spacing tokens
    json["spacing"] = m_spacingTokens;
    
    // Font tokens
    nlohmann::json fonts = nlohmann::json::object();
    for (const auto& [name, token] : m_fontTokens) {
        fonts[name] = {
            {"path", token.fontPath},
            {"size", token.fontSize}
        };
    }
    json["fonts"] = fonts;
    
    // Widget defaults
    nlohmann::json widgets = nlohmann::json::object();
    for (const auto& [type, props] : m_widgetDefaults) {
        std::string typeName = UIElement::TypeToString(type);
        widgets[typeName] = props;
    }
    json["widgetDefaults"] = widgets;
    
    return json;
}

void Theme::Deserialize(const nlohmann::json& json) {
    // Color tokens
    if (json.contains("colors")) {
        for (auto& [name, value] : json["colors"].items()) {
            m_colorTokens[name] = Color::FromHex(value.get<std::string>());
        }
    }
    
    // Spacing tokens
    if (json.contains("spacing")) {
        m_spacingTokens = json["spacing"].get<std::map<std::string, float>>();
    }
    
    // Font tokens
    if (json.contains("fonts")) {
        for (auto& [name, value] : json["fonts"].items()) {
            FontToken token;
            token.name = name;
            token.fontPath = value["path"];
            token.fontSize = value["size"];
            m_fontTokens[name] = token;
        }
    }
    
    // Widget defaults
    if (json.contains("widgetDefaults")) {
        for (auto& [typeName, props] : json["widgetDefaults"].items()) {
            WidgetType type = UIElement::TypeFromString(typeName);
            m_widgetDefaults[type] = props.get<std::map<std::string, std::string>>();
        }
    }
}

Theme::OptimizationReport Theme::Optimize() {
    OptimizationReport report;
    report.tokensRemoved = 0;
    
    // This would analyze which tokens are actually used and remove unused ones
    // Simplified implementation
    
    return report;
}

std::unique_ptr<Theme> Theme::CreateDefaultDarkTheme() {
    auto theme = std::make_unique<Theme>();
    
    // Color palette
    theme->AddColorToken("primary", Color(0.11f, 0.64f, 0.92f, 1.0f));
    theme->AddColorToken("secondary", Color(0.5f, 0.5f, 0.5f, 1.0f));
    theme->AddColorToken("background", Color(0.13f, 0.14f, 0.15f, 1.0f));
    theme->AddColorToken("surface", Color(0.2f, 0.2f, 0.22f, 1.0f));
    theme->AddColorToken("text", Color(0.95f, 0.95f, 0.95f, 1.0f));
    theme->AddColorToken("textDim", Color(0.7f, 0.7f, 0.7f, 1.0f));
    theme->AddColorToken("border", Color(0.43f, 0.43f, 0.5f, 0.5f));
    theme->AddColorToken("hover", Color(0.38f, 0.38f, 0.38f, 1.0f));
    theme->AddColorToken("active", Color(0.67f, 0.67f, 0.67f, 0.39f));
    
    // Spacing
    theme->AddSpacingToken("none", 0.0f);
    theme->AddSpacingToken("xs", 4.0f);
    theme->AddSpacingToken("sm", 8.0f);
    theme->AddSpacingToken("md", 16.0f);
    theme->AddSpacingToken("lg", 24.0f);
    theme->AddSpacingToken("xl", 32.0f);
    
    // Widget defaults
    theme->SetWidgetDefault(WidgetType::Button, "backgroundColor", "surface");
    theme->SetWidgetDefault(WidgetType::Button, "textColor", "text");
    theme->SetWidgetDefault(WidgetType::Button, "borderColor", "border");
    theme->SetWidgetDefault(WidgetType::Button, "padding", "sm");
    
    theme->SetWidgetDefault(WidgetType::Panel, "backgroundColor", "surface");
    theme->SetWidgetDefault(WidgetType::Panel, "borderColor", "border");
    theme->SetWidgetDefault(WidgetType::Panel, "padding", "md");
    
    theme->SetWidgetDefault(WidgetType::Label, "textColor", "text");
    
    theme->SetWidgetDefault(WidgetType::TextField, "backgroundColor", "background");
    theme->SetWidgetDefault(WidgetType::TextField, "textColor", "text");
    theme->SetWidgetDefault(WidgetType::TextField, "borderColor", "border");
    theme->SetWidgetDefault(WidgetType::TextField, "padding", "sm");
    
    return theme;
}

} // namespace SkinningStudio
