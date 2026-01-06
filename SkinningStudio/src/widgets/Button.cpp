#include "widgets/Button.h"
#include "core/StyleSystem.h"
#include <imgui.h>

Button::Button() {
    name = "Button";
    rect = Rect(0, 0, 100, 30);
}

void Button::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color bgColor = resolver->ResolveColor(this, "backgroundColor", currentState);
    Color textColor = resolver->ResolveColor(this, "textColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImVec2 pMin(worldRect.x, worldRect.y);
    ImVec2 pMax(worldRect.x + worldRect.width, worldRect.y + worldRect.height);
    
    ImU32 bgColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, bgColor.a));
    ImU32 textColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(textColor.r, textColor.g, textColor.b, textColor.a));
    
    drawList->AddRectFilled(pMin, pMax, bgColorU32, 4.0f);
    
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 textPos(
        worldRect.x + (worldRect.width - textSize.x) * 0.5f,
        worldRect.y + (worldRect.height - textSize.y) * 0.5f
    );
    drawList->AddText(textPos, textColorU32, text.c_str());
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

json Button::Serialize() const {
    json j = UIElement::Serialize();
    j["text"] = text;
    return j;
}

void Button::Deserialize(const json& j) {
    UIElement::Deserialize(j);
    if (j.contains("text")) text = j["text"];
}
