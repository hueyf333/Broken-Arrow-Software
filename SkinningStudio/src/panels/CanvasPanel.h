#pragma once

#include "core/UIElement.h"
#include "core/StyleSystem.h"
#include "core/CommandManager.h"
#include <vector>
#include <memory>
#include <functional>

class CanvasPanel {
public:
    void Render(
        std::vector<std::shared_ptr<UIElement>>& rootElements,
        StyleResolver* resolver,
        UIElement*& selectedElement,
        CommandManager* commandManager
    );
    
    std::function<void()> onSelectionChanged;
    
    bool showGrid = true;
    bool snapToGrid = false;
    float gridSize = 10.0f;
    float zoom = 1.0f;
    
private:
    Vec2 m_panOffset{0, 0};
    Vec2 m_dragStart{0, 0};
    bool m_isDragging = false;
    bool m_isResizing = false;
    int m_resizeHandle = -1; // 0-7 for 8 handles
    
    void DrawGrid(ImDrawList* drawList, const ImVec2& canvasPos, const ImVec2& canvasSize);
    void HandleSelection(const std::vector<std::shared_ptr<UIElement>>& elements, const ImVec2& mousePos, UIElement*& selected);
    void DrawSelectionBox(ImDrawList* drawList, UIElement* element);
    void DrawResizeHandles(ImDrawList* drawList, UIElement* element);
};
