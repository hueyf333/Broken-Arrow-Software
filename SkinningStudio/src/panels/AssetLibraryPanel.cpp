#include "panels/AssetLibraryPanel.h"
#include <imgui.h>

void AssetLibraryPanel::Render(std::shared_ptr<Theme> theme) {
    ImGui::Begin("Asset Library");
    
    if (!theme) {
        ImGui::Text("No theme loaded");
        ImGui::End();
        return;
    }
    
    if (ImGui::BeginTabBar("AssetTabs")) {
        if (ImGui::BeginTabItem("Theme Tokens")) {
            RenderThemeTokens(theme);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Widget Defaults")) {
            RenderWidgetDefaults(theme);
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void AssetLibraryPanel::RenderThemeTokens(std::shared_ptr<Theme> theme) {
    ImGui::Text("Theme: %s", theme->name.c_str());
    ImGui::Separator();
    
    if (ImGui::BeginTable("TokensTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Value");
        ImGui::TableSetupColumn("Category");
        ImGui::TableHeadersRow();
        
        for (auto& [key, token] : theme->tokens) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", token.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", token.value.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", token.category.c_str());
        }
        
        ImGui::EndTable();
    }
}

void AssetLibraryPanel::RenderWidgetDefaults(std::shared_ptr<Theme> theme) {
    for (auto& [widgetType, style] : theme->widgetDefaults) {
        if (ImGui::CollapsingHeader(widgetType.c_str())) {
            for (auto& [prop, value] : style.properties) {
                ImGui::Text("%s: %s", prop.c_str(), value.c_str());
            }
        }
    }
}
