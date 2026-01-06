#pragma once

#include "core/UIElement.h"
#include <vector>
#include <memory>
#include <functional>

class HierarchyPanel {
public:
    void Render(
        std::vector<std::shared_ptr<UIElement>>& rootElements,
        UIElement*& selectedElement
    );
    
    std::function<void(UIElement*, UIElement*)> onElementReparent;
    
private:
    void RenderElementNode(
        UIElement* element,
        UIElement*& selectedElement,
        UIElement* draggedElement = nullptr
    );
    
    UIElement* m_draggedElement = nullptr;
};
