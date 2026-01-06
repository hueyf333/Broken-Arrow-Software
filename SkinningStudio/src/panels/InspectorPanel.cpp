#include "panels/InspectorPanel.h"
#include <imgui.h>
#include <cstring>

void InspectorPanel::Render(UIElement* selectedElement, CommandManager* commandManager) {
    ImGui::Begin("Inspector");
    
    if (!selectedElement) {
        ImGui::Text("No element selected");
        ImGui::End();
        return;
    }
    
    // Basic properties
    ImGui::SeparatorText("Basic");
    
    char nameBuf[256];
    strncpy(nameBuf, selectedElement->name.c_str(), sizeof(nameBuf) - 1);
    if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
        selectedElement->name = nameBuf;
        if (onPropertyChanged) onPropertyChanged();
    }
    
    char idBuf[256];
    strncpy(idBuf, selectedElement->id.c_str(), sizeof(idBuf) - 1);
    ImGui::InputText("ID", idBuf, sizeof(idBuf), ImGuiInputTextFlags_ReadOnly);
    
    ImGui::Checkbox("Visible", &selectedElement->visible);
    ImGui::Checkbox("Enabled", &selectedElement->enabled);
    
    // Transform
    ImGui::SeparatorText("Transform");
    
    if (ImGui::DragFloat("X", &selectedElement->rect.x, 1.0f)) {
        if (onPropertyChanged) onPropertyChanged();
    }
    if (ImGui::DragFloat("Y", &selectedElement->rect.y, 1.0f)) {
        if (onPropertyChanged) onPropertyChanged();
    }
    if (ImGui::DragFloat("Width", &selectedElement->rect.width, 1.0f, 1.0f, 5000.0f)) {
        if (onPropertyChanged) onPropertyChanged();
    }
    if (ImGui::DragFloat("Height", &selectedElement->rect.height, 1.0f, 1.0f, 5000.0f)) {
        if (onPropertyChanged) onPropertyChanged();
    }
    
    // Layout
    ImGui::SeparatorText("Layout");
    
    const char* anchorItems[] = {
        "Top Left", "Top Center", "Top Right",
        "Middle Left", "Middle Center", "Middle Right",
        "Bottom Left", "Bottom Center", "Bottom Right"
    };
    int anchorIndex = static_cast<int>(selectedElement->anchor);
    if (ImGui::Combo("Anchor", &anchorIndex, anchorItems, IM_ARRAYSIZE(anchorItems))) {
        selectedElement->anchor = static_cast<Anchor>(anchorIndex);
        if (onPropertyChanged) onPropertyChanged();
    }
    
    ImGui::DragFloat2("Pivot", &selectedElement->pivot.x, 0.01f, 0.0f, 1.0f);
    
    // Padding
    ImGui::DragFloat4("Padding (T,R,B,L)", selectedElement->padding, 1.0f, 0.0f, 100.0f);
    
    // Margin
    ImGui::DragFloat4("Margin (T,R,B,L)", selectedElement->margin, 1.0f, 0.0f, 100.0f);
    
    // Layout mode
    const char* layoutItems[] = { "Absolute", "Vertical Stack", "Horizontal Stack" };
    int layoutIndex = static_cast<int>(selectedElement->layoutMode);
    if (ImGui::Combo("Layout Mode", &layoutIndex, layoutItems, IM_ARRAYSIZE(layoutItems))) {
        selectedElement->layoutMode = static_cast<LayoutMode>(layoutIndex);
        if (onPropertyChanged) onPropertyChanged();
    }
    
    if (selectedElement->layoutMode != LayoutMode::Absolute) {
        ImGui::DragFloat("Spacing", &selectedElement->spacing, 1.0f, 0.0f, 100.0f);
    }
    
    // Widget-specific properties
    ImGui::SeparatorText("Widget Properties");
    ImGui::Text("Type: %s", selectedElement->GetType().c_str());
    
    ImGui::End();
}
