#pragma once

#include "core/UIElement.h"

class Image : public UIElement {
public:
    Image();
    
    std::string imagePath;
    
    std::string GetType() const override { return "Image"; }
    void Render(class StyleResolver* resolver) override;
    json Serialize() const override;
    void Deserialize(const json& j) override;
};
