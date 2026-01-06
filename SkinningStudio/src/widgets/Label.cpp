#include "widgets/Label.h"
#include "core/StyleSystem.h"
#include <imgui.h>

Label::Label() {
    name = "Label";
    rect = Rect(0, 0, 80, 20);
}

void Label::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color textColor = resolver->ResolveColor(this, "textColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImU32 textColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(textColor.r, textColor.g, textColor.b, textColor.a));
    ImVec2 textPos(worldRect.x, worldRect.y);
    drawList->AddText(textPos, textColorU32, text.c_str());
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

json Label::Serialize() const {
    json j = UIElement::Serialize();
    j["text"] = text;
    return j;
}

void Label::Deserialize(const json& j) {
    UIElement::Deserialize(j);
    if (j.contains("text")) text = j["text"];
}
