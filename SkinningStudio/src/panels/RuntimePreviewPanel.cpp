#include "panels/RuntimePreviewPanel.h"
#include <imgui.h>

void RuntimePreviewPanel::Render(
    const std::vector<std::shared_ptr<UIElement>>& rootElements,
    StyleResolver* resolver
) {
    ImGui::Begin("Runtime Preview");
    
    ImVec2 previewPos = ImGui::GetCursorScreenPos();
    ImVec2 previewSize = ImGui::GetContentRegionAvail();
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Background
    drawList->AddRectFilled(
        previewPos,
        ImVec2(previewPos.x + previewSize.x, previewPos.y + previewSize.y),
        IM_COL32(30, 30, 30, 255)
    );
    
    // Render elements
    drawList->PushClipRect(
        previewPos,
        ImVec2(previewPos.x + previewSize.x, previewPos.y + previewSize.y),
        true
    );
    
    for (const auto& root : rootElements) {
        root->Render(resolver);
    }
    
    drawList->PopClipRect();
    
    ImGui::End();
}
