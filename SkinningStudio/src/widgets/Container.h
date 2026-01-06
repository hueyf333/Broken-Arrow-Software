#pragma once

#include "core/UIElement.h"

class ContainerVertical : public UIElement {
public:
    ContainerVertical();
    
    std::string GetType() const override { return "ContainerVertical"; }
    void Render(class StyleResolver* resolver) override;
    void UpdateLayout();
};

class ContainerHorizontal : public UIElement {
public:
    ContainerHorizontal();
    
    std::string GetType() const override { return "ContainerHorizontal"; }
    void Render(class StyleResolver* resolver) override;
    void UpdateLayout();
};
