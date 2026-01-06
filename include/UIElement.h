#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <nlohmann/json.hpp>

namespace SkinningStudio {

// Forward declarations
class Theme;
class Style;

enum class WidgetType {
    Panel,
    Label,
    Button,
    Image,
    TextField,
    Checkbox,
    Slider,
    ContainerVertical,
    ContainerHorizontal
};

enum class WidgetState {
    Normal,
    Hover,
    Active,
    Disabled,
    Focused
};

struct Rect {
    float x, y, width, height;
    
    Rect() : x(0), y(0), width(100), height(100) {}
    Rect(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
    
    bool Contains(float px, float py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
};

struct Color {
    float r, g, b, a;
    
    Color() : r(1), g(1), b(1), a(1) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    static Color FromHex(const std::string& hex);
    std::string ToHex() const;
};

class UIElement {
public:
    UIElement(WidgetType type = WidgetType::Panel);
    virtual ~UIElement() = default;

    // Properties
    std::string GetId() const { return m_id; }
    void SetId(const std::string& id) { m_id = id; }
    
    std::string GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }
    
    WidgetType GetType() const { return m_type; }
    
    Rect GetRect() const { return m_rect; }
    void SetRect(const Rect& rect) { m_rect = rect; }
    
    bool IsVisible() const { return m_visible; }
    void SetVisible(bool visible) { m_visible = visible; }
    
    bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enabled) { m_enabled = enabled; }
    
    bool IsLocked() const { return m_locked; }
    void SetLocked(bool locked) { m_locked = locked; }

    // Hierarchy
    UIElement* GetParent() const { return m_parent; }
    void SetParent(UIElement* parent);
    
    const std::vector<std::unique_ptr<UIElement>>& GetChildren() const { return m_children; }
    void AddChild(std::unique_ptr<UIElement> child);
    void RemoveChild(UIElement* child);
    UIElement* FindChild(const std::string& id) const;

    // Rendering
    virtual void Render(WidgetState state = WidgetState::Normal);
    virtual void RenderGizmos(bool selected);

    // Serialization
    virtual nlohmann::json Serialize() const;
    virtual void Deserialize(const nlohmann::json& json);

    // Factory
    static std::unique_ptr<UIElement> Create(WidgetType type);
    static WidgetType TypeFromString(const std::string& str);
    static std::string TypeToString(WidgetType type);

protected:
    std::string m_id;
    std::string m_name;
    WidgetType m_type;
    Rect m_rect;
    bool m_visible;
    bool m_enabled;
    bool m_locked;
    
    UIElement* m_parent;
    std::vector<std::unique_ptr<UIElement>> m_children;
    
    std::map<std::string, std::string> m_styleOverrides;
    std::map<WidgetState, std::map<std::string, std::string>> m_stateOverrides;

    static int s_nextId;
};

// Derived widget classes
class Panel : public UIElement {
public:
    Panel() : UIElement(WidgetType::Panel) {}
    void Render(WidgetState state = WidgetState::Normal) override;
};

class Label : public UIElement {
public:
    Label() : UIElement(WidgetType::Label) { m_text = "Label"; }
    void Render(WidgetState state = WidgetState::Normal) override;
    
    std::string GetText() const { return m_text; }
    void SetText(const std::string& text) { m_text = text; }
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    std::string m_text;
};

class Button : public UIElement {
public:
    Button() : UIElement(WidgetType::Button) { m_text = "Button"; }
    void Render(WidgetState state = WidgetState::Normal) override;
    
    std::string GetText() const { return m_text; }
    void SetText(const std::string& text) { m_text = text; }
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    std::string m_text;
};

class Image : public UIElement {
public:
    Image() : UIElement(WidgetType::Image) {}
    void Render(WidgetState state = WidgetState::Normal) override;
    
    std::string GetImagePath() const { return m_imagePath; }
    void SetImagePath(const std::string& path) { m_imagePath = path; }
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    std::string m_imagePath;
};

class TextField : public UIElement {
public:
    TextField() : UIElement(WidgetType::TextField) { m_text = ""; }
    void Render(WidgetState state = WidgetState::Normal) override;
    
    std::string GetText() const { return m_text; }
    void SetText(const std::string& text) { m_text = text; }
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    std::string m_text;
    char m_buffer[256];
};

class Checkbox : public UIElement {
public:
    Checkbox() : UIElement(WidgetType::Checkbox), m_checked(false) {}
    void Render(WidgetState state = WidgetState::Normal) override;
    
    bool IsChecked() const { return m_checked; }
    void SetChecked(bool checked) { m_checked = checked; }
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    bool m_checked;
};

class Slider : public UIElement {
public:
    Slider() : UIElement(WidgetType::Slider), m_value(0.5f), m_min(0.0f), m_max(1.0f) {}
    void Render(WidgetState state = WidgetState::Normal) override;
    
    float GetValue() const { return m_value; }
    void SetValue(float value) { m_value = value; }
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    float m_value;
    float m_min;
    float m_max;
};

class ContainerVertical : public UIElement {
public:
    ContainerVertical() : UIElement(WidgetType::ContainerVertical), m_spacing(4.0f), m_padding(8.0f) {}
    void Render(WidgetState state = WidgetState::Normal) override;
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    float m_spacing;
    float m_padding;
};

class ContainerHorizontal : public UIElement {
public:
    ContainerHorizontal() : UIElement(WidgetType::ContainerHorizontal), m_spacing(4.0f), m_padding(8.0f) {}
    void Render(WidgetState state = WidgetState::Normal) override;
    
    nlohmann::json Serialize() const override;
    void Deserialize(const nlohmann::json& json) override;

private:
    float m_spacing;
    float m_padding;
};

} // namespace SkinningStudio
