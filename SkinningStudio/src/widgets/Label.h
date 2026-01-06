#pragma once

#include "core/UIElement.h"

class Label : public UIElement {
public:
    Label();
    
    std::string text = "Label";
    
    std::string GetType() const override { return "Label"; }
    void Render(class StyleResolver* resolver) override;
    json Serialize() const override;
    void Deserialize(const json& j) override;
};
