#include "core/UIElement.h"
#include <algorithm>

int UIElement::s_nextId = 1;

UIElement::UIElement() {
    id = "element_" + std::to_string(s_nextId++);
    name = "Element";
}

void UIElement::AddChild(std::shared_ptr<UIElement> child) {
    if (!child) return;
    
    // Remove from old parent
    if (child->parent) {
        child->parent->RemoveChild(child.get());
    }
    
    child->parent = this;
    children.push_back(child);
}

void UIElement::RemoveChild(UIElement* child) {
    if (!child) return;
    
    auto it = std::find_if(children.begin(), children.end(),
        [child](const std::shared_ptr<UIElement>& ptr) {
            return ptr.get() == child;
        });
    
    if (it != children.end()) {
        (*it)->parent = nullptr;
        children.erase(it);
    }
}

void UIElement::SetParent(UIElement* newParent) {
    if (parent) {
        parent->RemoveChild(this);
    }
    parent = newParent;
}

Rect UIElement::GetWorldRect() const {
    Rect worldRect = rect;
    const UIElement* current = parent;
    
    while (current) {
        worldRect.x += current->rect.x;
        worldRect.y += current->rect.y;
        current = current->parent;
    }
    
    return worldRect;
}

bool UIElement::HitTest(float x, float y) const {
    if (!visible || !enabled) return false;
    Rect worldRect = GetWorldRect();
    return worldRect.Contains(x, y);
}

json UIElement::Serialize() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["type"] = GetType();
    j["rect"] = {rect.x, rect.y, rect.width, rect.height};
    j["visible"] = visible;
    j["enabled"] = enabled;
    j["anchor"] = static_cast<int>(anchor);
    j["pivot"] = {pivot.x, pivot.y};
    j["padding"] = {padding[0], padding[1], padding[2], padding[3]};
    j["margin"] = {margin[0], margin[1], margin[2], margin[3]};
    j["layoutMode"] = static_cast<int>(layoutMode);
    j["spacing"] = spacing;
    j["styleClass"] = styleClass;
    
    json childrenJson = json::array();
    for (const auto& child : children) {
        childrenJson.push_back(child->Serialize());
    }
    j["children"] = childrenJson;
    
    return j;
}

void UIElement::Deserialize(const json& j) {
    if (j.contains("id")) id = j["id"];
    if (j.contains("name")) name = j["name"];
    if (j.contains("rect")) {
        auto r = j["rect"];
        rect = Rect(r[0], r[1], r[2], r[3]);
    }
    if (j.contains("visible")) visible = j["visible"];
    if (j.contains("enabled")) enabled = j["enabled"];
    if (j.contains("anchor")) anchor = static_cast<Anchor>(j["anchor"].get<int>());
    if (j.contains("pivot")) {
        auto p = j["pivot"];
        pivot = Vec2(p[0], p[1]);
    }
    if (j.contains("padding")) {
        auto p = j["padding"];
        for (int i = 0; i < 4; i++) padding[i] = p[i];
    }
    if (j.contains("margin")) {
        auto m = j["margin"];
        for (int i = 0; i < 4; i++) margin[i] = m[i];
    }
    if (j.contains("layoutMode")) layoutMode = static_cast<LayoutMode>(j["layoutMode"].get<int>());
    if (j.contains("spacing")) spacing = j["spacing"];
    if (j.contains("styleClass")) styleClass = j["styleClass"];
}
