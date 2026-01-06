#pragma once

#include "core/UIElement.h"

class TextField : public UIElement {
public:
    TextField();
    
    std::string placeholder = "Enter text...";
    std::string value;
    
    std::string GetType() const override { return "TextField"; }
    void Render(class StyleResolver* resolver) override;
    json Serialize() const override;
    void Deserialize(const json& j) override;
};
