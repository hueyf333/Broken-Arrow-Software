#include "widgets/Image.h"
#include "core/StyleSystem.h"
#include <imgui.h>

Image::Image() {
    name = "Image";
    rect = Rect(0, 0, 100, 100);
}

void Image::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImVec2 pMin(worldRect.x, worldRect.y);
    ImVec2 pMax(worldRect.x + worldRect.width, worldRect.y + worldRect.height);
    
    // Placeholder - draw a border to represent the image
    drawList->AddRect(pMin, pMax, IM_COL32(100, 100, 100, 255));
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

json Image::Serialize() const {
    json j = UIElement::Serialize();
    j["imagePath"] = imagePath;
    return j;
}

void Image::Deserialize(const json& j) {
    UIElement::Deserialize(j);
    if (j.contains("imagePath")) imagePath = j["imagePath"];
}
