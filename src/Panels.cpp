#include "Panels.h"
#include "Application.h"
#include "Project.h"
#include "Canvas.h"
#include "Theme.h"
#include <imgui.h>

namespace SkinningStudio {

// HierarchyPanel
HierarchyPanel::HierarchyPanel()
    : m_draggedElement(nullptr)
{
}

HierarchyPanel::~HierarchyPanel() = default;

void HierarchyPanel::Render() {
    ImGui::Text("Hierarchy");
    ImGui::Separator();
    
    // Add widget buttons
    if (ImGui::Button("+ Panel")) {
        auto* project = Application::Instance().GetProject();
        if (project->GetRoot()) {
            project->GetRoot()->AddChild(std::make_unique<Panel>());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+ Button")) {
        auto* project = Application::Instance().GetProject();
        if (project->GetRoot()) {
            project->GetRoot()->AddChild(std::make_unique<Button>());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+ Label")) {
        auto* project = Application::Instance().GetProject();
        if (project->GetRoot()) {
            project->GetRoot()->AddChild(std::make_unique<Label>());
        }
    }
    
    ImGui::Separator();
    
    UIElement* root = Application::Instance().GetProject()->GetRoot();
    if (root) {
        RenderElement(root, 0);
    }
}

void HierarchyPanel::RenderElement(UIElement* element, int depth) {
    if (!element) return;
    
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (element->GetChildren().empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    
    // Check if selected
    auto* canvas = Application::Instance().GetCanvas();
    const auto& selection = canvas->GetSelection();
    bool isSelected = std::find(selection.begin(), selection.end(), element) != selection.end();
    if (isSelected) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }
    
    bool nodeOpen = ImGui::TreeNodeEx(element, flags, "%s (%s)", 
                                     element->GetName().c_str(),
                                     UIElement::TypeToString(element->GetType()).c_str());
    
    // Handle selection
    if (ImGui::IsItemClicked()) {
        std::vector<UIElement*> newSelection = {element};
        canvas->SetSelection(newSelection);
        Application::Instance().GetEditor()->SetSelection(newSelection);
    }
    
    // Drag and drop
    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("HIERARCHY_ITEM", &element, sizeof(UIElement*));
        ImGui::Text("%s", element->GetName().c_str());
        ImGui::EndDragDropSource();
    }
    
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM")) {
            UIElement* draggedElement = *(UIElement**)payload->Data;
            // TODO: Reparent draggedElement to element
        }
        ImGui::EndDragDropTarget();
    }
    
    if (nodeOpen) {
        for (const auto& child : element->GetChildren()) {
            RenderElement(child.get(), depth + 1);
        }
        ImGui::TreePop();
    }
}

void HierarchyPanel::HandleDragDrop(UIElement* element) {
    // TODO: Implement drag and drop reparenting
}

// InspectorPanel
InspectorPanel::InspectorPanel() {
}

InspectorPanel::~InspectorPanel() = default;

void InspectorPanel::Render() {
    ImGui::Text("Inspector");
    ImGui::Separator();
    
    if (m_selection.empty()) {
        ImGui::TextDisabled("No selection");
        return;
    }
    
    if (m_selection.size() == 1) {
        RenderProperties(m_selection[0]);
    } else {
        ImGui::Text("Multiple objects selected (%d)", (int)m_selection.size());
    }
}

void InspectorPanel::SetSelection(const std::vector<UIElement*>& selection) {
    m_selection = selection;
}

void InspectorPanel::RenderProperties(UIElement* element) {
    if (!element) return;
    
    // Basic properties
    if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
        char nameBuf[256];
        strncpy(nameBuf, element->GetName().c_str(), sizeof(nameBuf));
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
            element->SetName(nameBuf);
        }
        
        char idBuf[256];
        strncpy(idBuf, element->GetId().c_str(), sizeof(idBuf));
        if (ImGui::InputText("ID", idBuf, sizeof(idBuf))) {
            element->SetId(idBuf);
        }
        
        bool visible = element->IsVisible();
        if (ImGui::Checkbox("Visible", &visible)) {
            element->SetVisible(visible);
        }
        
        bool enabled = element->IsEnabled();
        if (ImGui::Checkbox("Enabled", &enabled)) {
            element->SetEnabled(enabled);
        }
        
        bool locked = element->IsLocked();
        if (ImGui::Checkbox("Locked", &locked)) {
            element->SetLocked(locked);
        }
    }
    
    // Transform
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        Rect rect = element->GetRect();
        bool changed = false;
        
        changed |= ImGui::DragFloat("X", &rect.x, 1.0f);
        changed |= ImGui::DragFloat("Y", &rect.y, 1.0f);
        changed |= ImGui::DragFloat("Width", &rect.width, 1.0f);
        changed |= ImGui::DragFloat("Height", &rect.height, 1.0f);
        
        if (changed) {
            element->SetRect(rect);
        }
    }
    
    // Widget-specific properties
    if (ImGui::CollapsingHeader("Widget Properties")) {
        if (auto* label = dynamic_cast<Label*>(element)) {
            char textBuf[256];
            strncpy(textBuf, label->GetText().c_str(), sizeof(textBuf));
            if (ImGui::InputText("Text", textBuf, sizeof(textBuf))) {
                label->SetText(textBuf);
            }
        } else if (auto* button = dynamic_cast<Button*>(element)) {
            char textBuf[256];
            strncpy(textBuf, button->GetText().c_str(), sizeof(textBuf));
            if (ImGui::InputText("Text", textBuf, sizeof(textBuf))) {
                button->SetText(textBuf);
            }
        } else if (auto* image = dynamic_cast<Image*>(element)) {
            char pathBuf[512];
            strncpy(pathBuf, image->GetImagePath().c_str(), sizeof(pathBuf));
            if (ImGui::InputText("Image Path", pathBuf, sizeof(pathBuf))) {
                image->SetImagePath(pathBuf);
            }
        } else if (auto* checkbox = dynamic_cast<Checkbox*>(element)) {
            bool checked = checkbox->IsChecked();
            if (ImGui::Checkbox("Checked", &checked)) {
                checkbox->SetChecked(checked);
            }
        } else if (auto* slider = dynamic_cast<Slider*>(element)) {
            float value = slider->GetValue();
            if (ImGui::SliderFloat("Value", &value, 0.0f, 1.0f)) {
                slider->SetValue(value);
            }
        }
    }
    
    // Style overrides
    RenderStyleOverrides(element);
}

void InspectorPanel::RenderStyleOverrides(UIElement* element) {
    if (ImGui::CollapsingHeader("Style Overrides")) {
        ImGui::TextDisabled("Add style overrides here");
        // TODO: Implement style override editing
    }
}

void InspectorPanel::RenderStateOverrides(UIElement* element) {
    if (ImGui::CollapsingHeader("State Overrides")) {
        ImGui::TextDisabled("Add state-specific overrides here");
        // TODO: Implement state override editing
    }
}

// AssetPanel
AssetPanel::AssetPanel()
    : m_selectedTab(0)
{
}

AssetPanel::~AssetPanel() = default;

void AssetPanel::Render() {
    ImGui::Text("Assets & Styles");
    ImGui::Separator();
    
    if (ImGui::BeginTabBar("AssetTabs")) {
        if (ImGui::BeginTabItem("Themes")) {
            RenderThemes();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fonts")) {
            RenderFonts();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Images")) {
            RenderImages();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Palettes")) {
            RenderPalettes();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void AssetPanel::RenderThemes() {
    ImGui::Text("Theme Manager");
    
    if (ImGui::Button("Create Theme")) {
        // TODO: Create new theme
    }
    ImGui::SameLine();
    if (ImGui::Button("Import Theme")) {
        // TODO: Import theme
    }
    
    ImGui::Separator();
    
    Theme* theme = Application::Instance().GetProject()->GetTheme();
    if (theme) {
        ImGui::Text("Color Tokens:");
        for (const auto& [name, color] : theme->GetColorTokens()) {
            ImVec4 col(color.r, color.g, color.b, color.a);
            ImGui::ColorEdit4(name.c_str(), &col.x);
        }
        
        ImGui::Separator();
        ImGui::Text("Spacing Tokens:");
        for (const auto& [name, value] : theme->GetSpacingTokens()) {
            ImGui::Text("%s: %.1f", name.c_str(), value);
        }
    }
}

void AssetPanel::RenderFonts() {
    ImGui::Text("Font Library");
    ImGui::TextDisabled("No fonts loaded");
}

void AssetPanel::RenderImages() {
    ImGui::Text("Image Assets");
    ImGui::TextDisabled("No images loaded");
}

void AssetPanel::RenderPalettes() {
    ImGui::Text("Color Palettes");
    ImGui::TextDisabled("Create and manage color palettes");
}

// CanvasPanel
CanvasPanel::CanvasPanel() {
}

CanvasPanel::~CanvasPanel() = default;

void CanvasPanel::Render() {
    RenderToolbar();
    ImGui::Separator();
    RenderCanvas();
}

void CanvasPanel::RenderToolbar() {
    auto* canvas = Application::Instance().GetCanvas();
    
    bool gridEnabled = canvas->IsGridEnabled();
    if (ImGui::Checkbox("Grid", &gridEnabled)) {
        canvas->SetGridEnabled(gridEnabled);
    }
    
    ImGui::SameLine();
    bool snapEnabled = canvas->IsSnapEnabled();
    if (ImGui::Checkbox("Snap", &snapEnabled)) {
        canvas->SetSnapEnabled(snapEnabled);
    }
    
    ImGui::SameLine();
    bool gizmosEnabled = canvas->IsGizmosEnabled();
    if (ImGui::Checkbox("Gizmos", &gizmosEnabled)) {
        canvas->SetGizmosEnabled(gizmosEnabled);
    }
    
    ImGui::SameLine();
    float zoom = canvas->GetZoom();
    if (ImGui::SliderFloat("Zoom", &zoom, 0.1f, 5.0f, "%.1fx")) {
        canvas->SetZoom(zoom);
    }
}

void CanvasPanel::RenderCanvas() {
    auto* canvas = Application::Instance().GetCanvas();
    canvas->Render();
}

// PreviewPanel
PreviewPanel::PreviewPanel() {
}

PreviewPanel::~PreviewPanel() = default;

void PreviewPanel::Render() {
    ImGui::Text("Runtime Preview");
    ImGui::Separator();
    RenderPreview();
}

void PreviewPanel::RenderPreview() {
    UIElement* root = Application::Instance().GetProject()->GetRoot();
    if (root) {
        // Render preview
        ImGui::BeginChild("PreviewArea", ImVec2(0, 0), true);
        
        // Render elements as they would appear at runtime
        root->Render();
        for (const auto& child : root->GetChildren()) {
            child->Render();
        }
        
        ImGui::EndChild();
    } else {
        ImGui::TextDisabled("No content to preview");
    }
}

} // namespace SkinningStudio
