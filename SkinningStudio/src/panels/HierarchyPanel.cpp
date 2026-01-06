#include "panels/HierarchyPanel.h"
#include <imgui.h>

void HierarchyPanel::Render(
    std::vector<std::shared_ptr<UIElement>>& rootElements,
    UIElement*& selectedElement
) {
    ImGui::Begin("Hierarchy");
    
    for (auto& root : rootElements) {
        RenderElementNode(root.get(), selectedElement);
    }
    
    // Add new root element button
    if (ImGui::Button("+ Add Root Element")) {
        ImGui::OpenPopup("AddElementPopup");
    }
    
    ImGui::End();
}

void HierarchyPanel::RenderElementNode(
    UIElement* element,
    UIElement*& selectedElement,
    UIElement* draggedElement
) {
    if (!element) return;
    
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (element == selectedElement) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    if (element->children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    
    std::string label = element->name + " (" + element->GetType() + ")";
    bool nodeOpen = ImGui::TreeNodeEx(element, flags, "%s", label.c_str());
    
    // Selection
    if (ImGui::IsItemClicked()) {
        selectedElement = element;
    }
    
    // Visibility toggle
    ImGui::SameLine(ImGui::GetWindowWidth() - 60);
    bool visible = element->visible;
    if (ImGui::Checkbox(("##vis" + element->id).c_str(), &visible)) {
        element->visible = visible;
    }
    
    // Lock toggle
    ImGui::SameLine();
    bool locked = element->isLocked;
    if (ImGui::Checkbox(("##lock" + element->id).c_str(), &locked)) {
        element->isLocked = locked;
    }
    
    if (nodeOpen) {
        for (auto& child : element->children) {
            RenderElementNode(child.get(), selectedElement, draggedElement);
        }
        ImGui::TreePop();
    }
}
