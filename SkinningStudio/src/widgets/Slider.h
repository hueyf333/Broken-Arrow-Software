#pragma once

#include "core/UIElement.h"

class Slider : public UIElement {
public:
    Slider();
    
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float value = 50.0f;
    
    std::string GetType() const override { return "Slider"; }
    void Render(class StyleResolver* resolver) override;
    json Serialize() const override;
    void Deserialize(const json& j) override;
};
