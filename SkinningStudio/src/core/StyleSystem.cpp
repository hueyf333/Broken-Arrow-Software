#include "core/StyleSystem.h"
#include "core/UIElement.h"
#include <sstream>

StyleResolver::StyleResolver(std::shared_ptr<Theme> theme) : m_theme(theme) {}

std::string StyleResolver::ResolveProperty(
    const UIElement* element,
    const std::string& propertyName,
    WidgetState state
) const {
    if (!element || !m_theme) return "";
    
    // 1. Check element state overrides
    auto stateIt = element->stateOverrides.find(state);
    if (stateIt != element->stateOverrides.end()) {
        auto propIt = stateIt->second.find(propertyName);
        if (propIt != stateIt->second.end()) {
            return ResolveToken(propIt->second);
        }
    }
    
    // 2. Check widget type defaults
    std::string widgetType = element->GetType();
    auto widgetIt = m_theme->widgetDefaults.find(widgetType);
    if (widgetIt != m_theme->widgetDefaults.end()) {
        auto propIt = widgetIt->second.properties.find(propertyName);
        if (propIt != widgetIt->second.properties.end()) {
            return ResolveToken(propIt->second);
        }
    }
    
    return "";
}

std::string StyleResolver::ResolveToken(const std::string& value) const {
    if (value.empty() || value[0] != '$') return value;
    
    // Token reference like $color.primary
    std::string tokenName = value.substr(1);
    auto it = m_theme->tokens.find(tokenName);
    if (it != m_theme->tokens.end()) {
        return it->second.value;
    }
    
    return value;
}

Color StyleResolver::ResolveColor(
    const UIElement* element,
    const std::string& propertyName,
    WidgetState state
) const {
    std::string value = ResolveProperty(element, propertyName, state);
    if (value.empty()) return Color();
    
    // Parse hex color #RRGGBB
    if (value[0] == '#' && value.length() == 7) {
        uint32_t hex = std::stoul(value.substr(1), nullptr, 16);
        return Color::FromHex(hex);
    }
    
    return Color();
}

float StyleResolver::ResolveFloat(
    const UIElement* element,
    const std::string& propertyName,
    WidgetState state
) const {
    std::string value = ResolveProperty(element, propertyName, state);
    if (value.empty()) return 0.0f;
    
    try {
        return std::stof(value);
    } catch (...) {
        return 0.0f;
    }
}

void StyleResolver::SetTheme(std::shared_ptr<Theme> theme) {
    m_theme = theme;
}

json Theme::Serialize() const {
    json j;
    j["name"] = name;
    
    json tokensJson;
    for (const auto& [key, token] : tokens) {
        tokensJson[key] = {
            {"name", token.name},
            {"value", token.value},
            {"category", token.category}
        };
    }
    j["tokens"] = tokensJson;
    
    json widgetsJson;
    for (const auto& [type, style] : widgetDefaults) {
        widgetsJson[type] = style.properties;
    }
    j["widgetDefaults"] = widgetsJson;
    
    return j;
}

void Theme::Deserialize(const json& j) {
    if (j.contains("name")) name = j["name"];
    
    if (j.contains("tokens")) {
        for (const auto& [key, val] : j["tokens"].items()) {
            StyleToken token;
            token.name = val["name"];
            token.value = val["value"];
            token.category = val["category"];
            tokens[key] = token;
        }
    }
    
    if (j.contains("widgetDefaults")) {
        for (const auto& [type, props] : j["widgetDefaults"].items()) {
            WidgetStyle style;
            style.properties = props.get<std::unordered_map<std::string, std::string>>();
            widgetDefaults[type] = style;
        }
    }
}

std::shared_ptr<Theme> Theme::CreateDefaultDarkTheme() {
    auto theme = std::make_shared<Theme>();
    theme->name = "Dark";
    
    // Define color tokens
    theme->tokens["color.primary"] = {"color.primary", "#3B82F6", "color"};
    theme->tokens["color.background"] = {"color.background", "#1E1E1E", "color"};
    theme->tokens["color.surface"] = {"color.surface", "#2D2D2D", "color"};
    theme->tokens["color.text"] = {"color.text", "#E0E0E0", "color"};
    theme->tokens["color.textSecondary"] = {"color.textSecondary", "#A0A0A0", "color"};
    theme->tokens["color.border"] = {"color.border", "#404040", "color"};
    theme->tokens["color.hover"] = {"color.hover", "#404040", "color"};
    theme->tokens["color.active"] = {"color.active", "#4A4A4A", "color"};
    theme->tokens["color.disabled"] = {"color.disabled", "#606060", "color"};
    
    // Size tokens
    theme->tokens["size.padding"] = {"size.padding", "8", "size"};
    theme->tokens["size.borderRadius"] = {"size.borderRadius", "4", "size"};
    theme->tokens["size.fontSize"] = {"size.fontSize", "14", "size"};
    
    // Button defaults
    WidgetStyle buttonStyle;
    buttonStyle.properties["backgroundColor"] = "$color.surface";
    buttonStyle.properties["textColor"] = "$color.text";
    buttonStyle.properties["borderColor"] = "$color.border";
    buttonStyle.properties["padding"] = "$size.padding";
    buttonStyle.properties["borderRadius"] = "$size.borderRadius";
    theme->widgetDefaults["Button"] = buttonStyle;
    
    // Label defaults
    WidgetStyle labelStyle;
    labelStyle.properties["textColor"] = "$color.text";
    labelStyle.properties["fontSize"] = "$size.fontSize";
    theme->widgetDefaults["Label"] = labelStyle;
    
    // TextField defaults
    WidgetStyle textFieldStyle;
    textFieldStyle.properties["backgroundColor"] = "$color.background";
    textFieldStyle.properties["textColor"] = "$color.text";
    textFieldStyle.properties["borderColor"] = "$color.border";
    textFieldStyle.properties["padding"] = "$size.padding";
    theme->widgetDefaults["TextField"] = textFieldStyle;
    
    // Container defaults
    WidgetStyle containerStyle;
    containerStyle.properties["backgroundColor"] = "$color.surface";
    containerStyle.properties["padding"] = "$size.padding";
    theme->widgetDefaults["ContainerVertical"] = containerStyle;
    theme->widgetDefaults["ContainerHorizontal"] = containerStyle;
    
    return theme;
}
