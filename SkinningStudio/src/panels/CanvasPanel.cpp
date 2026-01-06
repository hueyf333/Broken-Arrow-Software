#include "panels/CanvasPanel.h"
#include <imgui.h>
#include <algorithm>

void CanvasPanel::Render(
    std::vector<std::shared_ptr<UIElement>>& rootElements,
    StyleResolver* resolver,
    UIElement*& selectedElement,
    CommandManager* commandManager
) {
    ImGui::Begin("Canvas");
    
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Background
    drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(40, 40, 40, 255));
    
    // Grid
    if (showGrid) {
        DrawGrid(drawList, canvasPos, canvasSize);
    }
    
    // Canvas area for interaction
    ImGui::InvisibleButton("canvas", canvasSize);
    
    // Handle panning
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        m_panOffset.x += delta.x;
        m_panOffset.y += delta.y;
    }
    
    // Render elements
    drawList->PushClipRect(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), true);
    
    // Save current positions and apply pan offset for rendering
    std::vector<Vec2> originalPositions;
    originalPositions.reserve(rootElements.size());
    
    for (auto& root : rootElements) {
        originalPositions.push_back(Vec2(root->rect.x, root->rect.y));
        root->rect.x += m_panOffset.x;
        root->rect.y += m_panOffset.y;
    }
    
    // Render with offset
    for (auto& root : rootElements) {
        root->Render(resolver);
    }
    
    // Restore original positions
    for (size_t i = 0; i < rootElements.size(); ++i) {
        rootElements[i]->rect.x = originalPositions[i].x;
        rootElements[i]->rect.y = originalPositions[i].y;
    }
    
    // Draw selection box
    if (selectedElement) {
        DrawSelectionBox(drawList, selectedElement);
        DrawResizeHandles(drawList, selectedElement);
    }
    
    drawList->PopClipRect();
    
    // Handle selection
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        mousePos.x -= canvasPos.x + m_panOffset.x;
        mousePos.y -= canvasPos.y + m_panOffset.y;
        HandleSelection(rootElements, mousePos, selectedElement);
    }
    
    ImGui::End();
}

void CanvasPanel::DrawGrid(ImDrawList* drawList, const ImVec2& canvasPos, const ImVec2& canvasSize) {
    const float step = gridSize * zoom;
    const ImU32 gridColor = IM_COL32(60, 60, 60, 255);
    
    for (float x = fmodf(m_panOffset.x, step); x < canvasSize.x; x += step) {
        drawList->AddLine(
            ImVec2(canvasPos.x + x, canvasPos.y),
            ImVec2(canvasPos.x + x, canvasPos.y + canvasSize.y),
            gridColor
        );
    }
    
    for (float y = fmodf(m_panOffset.y, step); y < canvasSize.y; y += step) {
        drawList->AddLine(
            ImVec2(canvasPos.x, canvasPos.y + y),
            ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + y),
            gridColor
        );
    }
}

void CanvasPanel::HandleSelection(
    const std::vector<std::shared_ptr<UIElement>>& elements,
    const ImVec2& mousePos,
    UIElement*& selected
) {
    selected = nullptr;
    
    std::function<void(const std::vector<std::shared_ptr<UIElement>>&)> findElement;
    findElement = [&](const std::vector<std::shared_ptr<UIElement>>& elems) {
        for (auto it = elems.rbegin(); it != elems.rend(); ++it) {
            auto& elem = *it;
            if (elem->HitTest(mousePos.x, mousePos.y)) {
                selected = elem.get();
                return;
            }
            findElement(elem->children);
            if (selected) return;
        }
    };
    
    findElement(elements);
    
    if (onSelectionChanged && selected) {
        onSelectionChanged();
    }
}

void CanvasPanel::DrawSelectionBox(ImDrawList* drawList, UIElement* element) {
    if (!element) return;
    
    Rect worldRect = element->GetWorldRect();
    worldRect.x += m_panOffset.x;
    worldRect.y += m_panOffset.y;
    
    ImVec2 pMin(worldRect.x, worldRect.y);
    ImVec2 pMax(worldRect.x + worldRect.width, worldRect.y + worldRect.height);
    
    drawList->AddRect(pMin, pMax, IM_COL32(0, 150, 255, 255), 0.0f, 0, 2.0f);
}

void CanvasPanel::DrawResizeHandles(ImDrawList* drawList, UIElement* element) {
    if (!element) return;
    
    Rect worldRect = element->GetWorldRect();
    worldRect.x += m_panOffset.x;
    worldRect.y += m_panOffset.y;
    
    const float handleSize = 6.0f;
    const ImU32 handleColor = IM_COL32(255, 255, 255, 255);
    
    // 8 handles: corners and midpoints
    ImVec2 handles[] = {
        {worldRect.x, worldRect.y}, // Top-left
        {worldRect.x + worldRect.width * 0.5f, worldRect.y}, // Top-middle
        {worldRect.x + worldRect.width, worldRect.y}, // Top-right
        {worldRect.x + worldRect.width, worldRect.y + worldRect.height * 0.5f}, // Right-middle
        {worldRect.x + worldRect.width, worldRect.y + worldRect.height}, // Bottom-right
        {worldRect.x + worldRect.width * 0.5f, worldRect.y + worldRect.height}, // Bottom-middle
        {worldRect.x, worldRect.y + worldRect.height}, // Bottom-left
        {worldRect.x, worldRect.y + worldRect.height * 0.5f} // Left-middle
    };
    
    for (const auto& handle : handles) {
        drawList->AddRectFilled(
            ImVec2(handle.x - handleSize * 0.5f, handle.y - handleSize * 0.5f),
            ImVec2(handle.x + handleSize * 0.5f, handle.y + handleSize * 0.5f),
            handleColor
        );
    }
}
