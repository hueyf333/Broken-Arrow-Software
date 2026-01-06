#include "widgets/Checkbox.h"
#include "core/StyleSystem.h"
#include <imgui.h>

Checkbox::Checkbox() {
    name = "Checkbox";
    rect = Rect(0, 0, 120, 20);
}

void Checkbox::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color textColor = resolver->ResolveColor(this, "textColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImU32 textColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(textColor.r, textColor.g, textColor.b, textColor.a));
    
    // Draw checkbox box
    float boxSize = 16;
    ImVec2 boxMin(worldRect.x, worldRect.y);
    ImVec2 boxMax(worldRect.x + boxSize, worldRect.y + boxSize);
    
    drawList->AddRect(boxMin, boxMax, IM_COL32(100, 100, 100, 255));
    if (checked) {
        drawList->AddRectFilled(
            ImVec2(boxMin.x + 3, boxMin.y + 3),
            ImVec2(boxMax.x - 3, boxMax.y - 3),
            textColorU32
        );
    }
    
    // Draw label
    ImVec2 textPos(worldRect.x + boxSize + 5, worldRect.y);
    drawList->AddText(textPos, textColorU32, label.c_str());
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

json Checkbox::Serialize() const {
    json j = UIElement::Serialize();
    j["label"] = label;
    j["checked"] = checked;
    return j;
}

void Checkbox::Deserialize(const json& j) {
    UIElement::Deserialize(j);
    if (j.contains("label")) label = j["label"];
    if (j.contains("checked")) checked = j["checked"];
}
