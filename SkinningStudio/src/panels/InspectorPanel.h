#pragma once

#include "core/UIElement.h"
#include "core/CommandManager.h"
#include <functional>

class InspectorPanel {
public:
    void Render(UIElement* selectedElement, CommandManager* commandManager);
    
    std::function<void()> onPropertyChanged;
};
