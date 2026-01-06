#include "UIRuntime.h"
#include <fstream>
#include <imgui.h>

bool UIRuntime::LoadFromFiles(const std::string& themeFile, const std::string& layoutFile) {
    try {
        std::ifstream themeStream(themeFile);
        if (!themeStream.is_open()) return false;
        themeStream >> m_theme;
        
        std::ifstream layoutStream(layoutFile);
        if (!layoutStream.is_open()) return false;
        layoutStream >> m_layout;
        
        return true;
    } catch (...) {
        return false;
    }
}

void UIRuntime::Render() {
    // Simple rendering of loaded UI
    ImGui::Begin("Runtime UI");
    
    if (m_layout.contains("elements")) {
        for (const auto& element : m_layout["elements"]) {
            std::string type = element["type"];
            std::string name = element["name"];
            
            ImGui::Text("%s: %s", type.c_str(), name.c_str());
            
            if (type == "Button" && element.contains("text")) {
                ImGui::Button(element["text"].get<std::string>().c_str());
            } else if (type == "Label" && element.contains("text")) {
                ImGui::Text("%s", element["text"].get<std::string>().c_str());
            }
        }
    }
    
    ImGui::End();
}
