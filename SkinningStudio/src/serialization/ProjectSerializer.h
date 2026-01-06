#pragma once

#include "core/Common.h"
#include "core/UIElement.h"
#include "core/StyleSystem.h"
#include <string>
#include <memory>
#include <vector>

class ProjectSerializer {
public:
    static bool SaveProject(
        const std::string& filePath,
        const std::vector<std::shared_ptr<UIElement>>& rootElements,
        std::shared_ptr<Theme> theme
    );
    
    static bool LoadProject(
        const std::string& filePath,
        std::vector<std::shared_ptr<UIElement>>& outRootElements,
        std::shared_ptr<Theme>& outTheme
    );
    
    static bool ExportTheme(const std::string& filePath, std::shared_ptr<Theme> theme);
    static std::shared_ptr<Theme> ImportTheme(const std::string& filePath);
    
    static bool ExportRuntimePackage(
        const std::string& outputDir,
        const std::vector<std::shared_ptr<UIElement>>& rootElements,
        std::shared_ptr<Theme> theme
    );
    
    static std::shared_ptr<UIElement> CreateElementFromType(const std::string& type);
};
