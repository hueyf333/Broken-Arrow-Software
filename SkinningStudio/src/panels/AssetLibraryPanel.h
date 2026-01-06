#pragma once

#include "core/StyleSystem.h"
#include <memory>
#include <functional>

class AssetLibraryPanel {
public:
    void Render(std::shared_ptr<Theme> theme);
    
    std::function<void()> onThemeChanged;
    
private:
    void RenderThemeTokens(std::shared_ptr<Theme> theme);
    void RenderWidgetDefaults(std::shared_ptr<Theme> theme);
};
