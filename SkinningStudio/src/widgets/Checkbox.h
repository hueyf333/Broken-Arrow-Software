#pragma once

#include "core/UIElement.h"

class Checkbox : public UIElement {
public:
    Checkbox();
    
    std::string label = "Checkbox";
    bool checked = false;
    
    std::string GetType() const override { return "Checkbox"; }
    void Render(class StyleResolver* resolver) override;
    json Serialize() const override;
    void Deserialize(const json& j) override;
};
