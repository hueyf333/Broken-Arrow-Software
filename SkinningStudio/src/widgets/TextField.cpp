#include "widgets/TextField.h"
#include "core/StyleSystem.h"
#include <imgui.h>

TextField::TextField() {
    name = "TextField";
    rect = Rect(0, 0, 150, 30);
}

void TextField::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color bgColor = resolver->ResolveColor(this, "backgroundColor", currentState);
    Color textColor = resolver->ResolveColor(this, "textColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImVec2 pMin(worldRect.x, worldRect.y);
    ImVec2 pMax(worldRect.x + worldRect.width, worldRect.y + worldRect.height);
    
    ImU32 bgColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, bgColor.a));
    ImU32 textColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(textColor.r, textColor.g, textColor.b, textColor.a));
    
    drawList->AddRectFilled(pMin, pMax, bgColorU32);
    drawList->AddRect(pMin, pMax, IM_COL32(100, 100, 100, 255));
    
    const char* displayText = value.empty() ? placeholder.c_str() : value.c_str();
    ImVec2 textPos(worldRect.x + 5, worldRect.y + 5);
    drawList->AddText(textPos, textColorU32, displayText);
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

json TextField::Serialize() const {
    json j = UIElement::Serialize();
    j["placeholder"] = placeholder;
    j["value"] = value;
    return j;
}

void TextField::Deserialize(const json& j) {
    UIElement::Deserialize(j);
    if (j.contains("placeholder")) placeholder = j["placeholder"];
    if (j.contains("value")) value = j["value"];
}
