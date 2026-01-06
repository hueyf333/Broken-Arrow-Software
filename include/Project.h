#pragma once

#include "UIElement.h"
#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace SkinningStudio {

class Theme;

class Project {
public:
    Project();
    ~Project();

    // Project management
    void New();
    bool Load(const std::string& path);
    bool Save(const std::string& path);
    bool Save(); // Save to current path
    
    std::string GetPath() const { return m_path; }
    bool IsModified() const { return m_modified; }
    void SetModified(bool modified) { m_modified = modified; }

    // Root element access
    UIElement* GetRoot() const { return m_root.get(); }
    void SetRoot(std::unique_ptr<UIElement> root);

    // Theme access
    Theme* GetTheme() const { return m_theme.get(); }
    void SetTheme(std::unique_ptr<Theme> theme);

    // Import/Export
    bool ImportTheme(const std::string& path);
    bool ExportTheme(const std::string& path);
    bool ExportRuntimePackage(const std::string& directory);

    // Validation
    struct ValidationResult {
        bool success;
        std::vector<std::string> warnings;
        std::vector<std::string> errors;
    };
    ValidationResult Validate() const;

private:
    std::string m_path;
    bool m_modified;
    std::unique_ptr<UIElement> m_root;
    std::unique_ptr<Theme> m_theme;
    
    nlohmann::json SerializeProject() const;
    bool DeserializeProject(const nlohmann::json& json);
};

} // namespace SkinningStudio
