#include "widgets/Container.h"
#include "core/StyleSystem.h"
#include <imgui.h>

ContainerVertical::ContainerVertical() {
    name = "ContainerVertical";
    rect = Rect(0, 0, 200, 300);
    layoutMode = LayoutMode::VerticalStack;
}

void ContainerVertical::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color bgColor = resolver->ResolveColor(this, "backgroundColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImVec2 pMin(worldRect.x, worldRect.y);
    ImVec2 pMax(worldRect.x + worldRect.width, worldRect.y + worldRect.height);
    
    ImU32 bgColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, bgColor.a));
    drawList->AddRectFilled(pMin, pMax, bgColorU32);
    drawList->AddRect(pMin, pMax, IM_COL32(80, 80, 80, 255));
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

void ContainerVertical::UpdateLayout() {
    float currentY = padding[0]; // top padding
    
    for (auto& child : children) {
        child->rect.x = padding[3]; // left padding
        child->rect.y = currentY;
        currentY += child->rect.height + spacing;
    }
}

ContainerHorizontal::ContainerHorizontal() {
    name = "ContainerHorizontal";
    rect = Rect(0, 0, 300, 100);
    layoutMode = LayoutMode::HorizontalStack;
}

void ContainerHorizontal::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color bgColor = resolver->ResolveColor(this, "backgroundColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImVec2 pMin(worldRect.x, worldRect.y);
    ImVec2 pMax(worldRect.x + worldRect.width, worldRect.y + worldRect.height);
    
    ImU32 bgColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, bgColor.a));
    drawList->AddRectFilled(pMin, pMax, bgColorU32);
    drawList->AddRect(pMin, pMax, IM_COL32(80, 80, 80, 255));
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

void ContainerHorizontal::UpdateLayout() {
    float currentX = padding[3]; // left padding
    
    for (auto& child : children) {
        child->rect.x = currentX;
        child->rect.y = padding[0]; // top padding
        currentX += child->rect.width + spacing;
    }
}
