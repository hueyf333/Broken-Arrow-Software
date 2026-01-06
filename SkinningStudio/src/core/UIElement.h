#pragma once

#include "core/Common.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class UIElement {
public:
    UIElement();
    virtual ~UIElement() = default;

    // Core properties
    std::string id;
    std::string name;
    Rect rect;
    bool visible = true;
    bool enabled = true;
    
    // Hierarchy
    UIElement* parent = nullptr;
    std::vector<std::shared_ptr<UIElement>> children;
    
    // Layout
    Anchor anchor = Anchor::TopLeft;
    Vec2 pivot{0.5f, 0.5f};
    float padding[4] = {0, 0, 0, 0}; // top, right, bottom, left
    float margin[4] = {0, 0, 0, 0};
    LayoutMode layoutMode = LayoutMode::Absolute;
    float spacing = 0;
    
    // Style
    std::string styleClass;
    std::unordered_map<WidgetState, std::unordered_map<std::string, std::string>> stateOverrides;
    
    // Interaction state
    WidgetState currentState = WidgetState::Normal;
    bool isLocked = false;
    bool isSelected = false;
    
    // Methods
    virtual std::string GetType() const = 0;
    virtual void Render(class StyleResolver* resolver) = 0;
    virtual json Serialize() const;
    virtual void Deserialize(const json& j);
    
    void AddChild(std::shared_ptr<UIElement> child);
    void RemoveChild(UIElement* child);
    void SetParent(UIElement* newParent);
    
    // Helper methods
    Rect GetWorldRect() const;
    bool HitTest(float x, float y) const;
    
protected:
    static int s_nextId;
};
