#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Simple runtime for loading and displaying exported UI
class UIRuntime {
public:
    bool LoadFromFiles(const std::string& themeFile, const std::string& layoutFile);
    void Render();
    
private:
    json m_theme;
    json m_layout;
};
