#pragma once

#include "core/UIElement.h"

class Button : public UIElement {
public:
    Button();
    
    std::string text = "Button";
    
    std::string GetType() const override { return "Button"; }
    void Render(class StyleResolver* resolver) override;
    json Serialize() const override;
    void Deserialize(const json& j) override;
};
