#pragma once

#include "core/Common.h"
#include <string>
#include <unordered_map>
#include <memory>

struct StyleToken {
    std::string name;
    std::string value;
    std::string category; // color, size, font, etc.
};

struct WidgetStyle {
    std::unordered_map<std::string, std::string> properties;
};

class Theme {
public:
    std::string name;
    std::unordered_map<std::string, StyleToken> tokens;
    std::unordered_map<std::string, WidgetStyle> widgetDefaults; // keyed by widget type
    
    json Serialize() const;
    void Deserialize(const json& j);
    
    static std::shared_ptr<Theme> CreateDefaultDarkTheme();
};

class StyleResolver {
public:
    StyleResolver(std::shared_ptr<Theme> theme);
    
    std::string ResolveProperty(
        const UIElement* element,
        const std::string& propertyName,
        WidgetState state = WidgetState::Normal
    ) const;
    
    Color ResolveColor(
        const UIElement* element,
        const std::string& propertyName,
        WidgetState state = WidgetState::Normal
    ) const;
    
    float ResolveFloat(
        const UIElement* element,
        const std::string& propertyName,
        WidgetState state = WidgetState::Normal
    ) const;
    
    void SetTheme(std::shared_ptr<Theme> theme);
    std::shared_ptr<Theme> GetTheme() const { return m_theme; }
    
private:
    std::shared_ptr<Theme> m_theme;
    
    std::string ResolveToken(const std::string& value) const;
};
