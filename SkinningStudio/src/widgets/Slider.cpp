#include "widgets/Slider.h"
#include "core/StyleSystem.h"
#include <imgui.h>

Slider::Slider() {
    name = "Slider";
    rect = Rect(0, 0, 150, 20);
}

void Slider::Render(StyleResolver* resolver) {
    if (!visible) return;
    
    Color bgColor = resolver->ResolveColor(this, "backgroundColor", currentState);
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    Rect worldRect = GetWorldRect();
    
    ImU32 bgColorU32 = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor.r, bgColor.g, bgColor.b, bgColor.a));
    
    // Draw track
    float trackHeight = 4;
    ImVec2 trackMin(worldRect.x, worldRect.y + (worldRect.height - trackHeight) * 0.5f);
    ImVec2 trackMax(worldRect.x + worldRect.width, trackMin.y + trackHeight);
    drawList->AddRectFilled(trackMin, trackMax, IM_COL32(60, 60, 60, 255));
    
    // Draw thumb
    float normalizedValue = (value - minValue) / (maxValue - minValue);
    float thumbX = worldRect.x + normalizedValue * worldRect.width;
    float thumbRadius = 8;
    drawList->AddCircleFilled(ImVec2(thumbX, worldRect.y + worldRect.height * 0.5f), thumbRadius, bgColorU32);
    
    // Render children
    for (auto& child : children) {
        child->Render(resolver);
    }
}

json Slider::Serialize() const {
    json j = UIElement::Serialize();
    j["minValue"] = minValue;
    j["maxValue"] = maxValue;
    j["value"] = value;
    return j;
}

void Slider::Deserialize(const json& j) {
    UIElement::Deserialize(j);
    if (j.contains("minValue")) minValue = j["minValue"];
    if (j.contains("maxValue")) maxValue = j["maxValue"];
    if (j.contains("value")) value = j["value"];
}
