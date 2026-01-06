#pragma once

#include "core/UIElement.h"
#include "core/StyleSystem.h"
#include <vector>
#include <memory>

class RuntimePreviewPanel {
public:
    void Render(
        const std::vector<std::shared_ptr<UIElement>>& rootElements,
        StyleResolver* resolver
    );
};
