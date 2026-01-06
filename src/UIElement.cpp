#include "UIElement.h"
#include <imgui.h>
#include <sstream>
#include <iomanip>

namespace SkinningStudio {

int UIElement::s_nextId = 1;

Color Color::FromHex(const std::string& hex) {
    std::string h = hex;
    if (h[0] == '#') h = h.substr(1);
    
    unsigned int value;
    std::stringstream ss;
    ss << std::hex << h;
    ss >> value;
    
    if (h.length() == 6) {
        return Color(
            ((value >> 16) & 0xFF) / 255.0f,
            ((value >> 8) & 0xFF) / 255.0f,
            (value & 0xFF) / 255.0f,
            1.0f
        );
    } else if (h.length() == 8) {
        return Color(
            ((value >> 24) & 0xFF) / 255.0f,
            ((value >> 16) & 0xFF) / 255.0f,
            ((value >> 8) & 0xFF) / 255.0f,
            (value & 0xFF) / 255.0f
        );
    }
    
    return Color();
}

std::string Color::ToHex() const {
    std::stringstream ss;
    ss << "#";
    ss << std::hex << std::setfill('0');
    ss << std::setw(2) << static_cast<int>(r * 255);
    ss << std::setw(2) << static_cast<int>(g * 255);
    ss << std::setw(2) << static_cast<int>(b * 255);
    ss << std::setw(2) << static_cast<int>(a * 255);
    return ss.str();
}

UIElement::UIElement(WidgetType type)
    : m_type(type)
    , m_visible(true)
    , m_enabled(true)
    , m_locked(false)
    , m_parent(nullptr)
{
    m_id = "element_" + std::to_string(s_nextId++);
    m_name = TypeToString(type);
}

void UIElement::SetParent(UIElement* parent) {
    m_parent = parent;
}

void UIElement::AddChild(std::unique_ptr<UIElement> child) {
    if (child) {
        child->SetParent(this);
        m_children.push_back(std::move(child));
    }
}

void UIElement::RemoveChild(UIElement* child) {
    m_children.erase(
        std::remove_if(m_children.begin(), m_children.end(),
            [child](const std::unique_ptr<UIElement>& ptr) { return ptr.get() == child; }),
        m_children.end()
    );
}

UIElement* UIElement::FindChild(const std::string& id) const {
    for (const auto& child : m_children) {
        if (child->GetId() == id) return child.get();
        UIElement* found = child->FindChild(id);
        if (found) return found;
    }
    return nullptr;
}

void UIElement::Render(WidgetState state) {
    // Base rendering
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(60, 60, 60, 255));
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(100, 100, 100, 255));
}

void UIElement::RenderGizmos(bool selected) {
    if (!selected) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    
    // Selection border
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(30, 164, 235, 255), 0.0f, 0, 2.0f);
    
    // Resize handles
    const float handleSize = 6.0f;
    const ImU32 handleColor = IM_COL32(255, 255, 255, 255);
    
    // Corners
    draw_list->AddRectFilled(
        ImVec2(pos.x - handleSize/2, pos.y - handleSize/2),
        ImVec2(pos.x + handleSize/2, pos.y + handleSize/2),
        handleColor
    );
    draw_list->AddRectFilled(
        ImVec2(pos.x + size.x - handleSize/2, pos.y - handleSize/2),
        ImVec2(pos.x + size.x + handleSize/2, pos.y + handleSize/2),
        handleColor
    );
    draw_list->AddRectFilled(
        ImVec2(pos.x - handleSize/2, pos.y + size.y - handleSize/2),
        ImVec2(pos.x + handleSize/2, pos.y + size.y + handleSize/2),
        handleColor
    );
    draw_list->AddRectFilled(
        ImVec2(pos.x + size.x - handleSize/2, pos.y + size.y - handleSize/2),
        ImVec2(pos.x + size.x + handleSize/2, pos.y + size.y + handleSize/2),
        handleColor
    );
}

nlohmann::json UIElement::Serialize() const {
    nlohmann::json j;
    j["id"] = m_id;
    j["name"] = m_name;
    j["type"] = TypeToString(m_type);
    j["rect"] = {m_rect.x, m_rect.y, m_rect.width, m_rect.height};
    j["visible"] = m_visible;
    j["enabled"] = m_enabled;
    j["locked"] = m_locked;
    
    nlohmann::json children = nlohmann::json::array();
    for (const auto& child : m_children) {
        children.push_back(child->Serialize());
    }
    j["children"] = children;
    
    return j;
}

void UIElement::Deserialize(const nlohmann::json& json) {
    if (json.contains("id")) m_id = json["id"];
    if (json.contains("name")) m_name = json["name"];
    if (json.contains("rect")) {
        auto r = json["rect"];
        m_rect = Rect(r[0], r[1], r[2], r[3]);
    }
    if (json.contains("visible")) m_visible = json["visible"];
    if (json.contains("enabled")) m_enabled = json["enabled"];
    if (json.contains("locked")) m_locked = json["locked"];
    
    if (json.contains("children")) {
        for (const auto& childJson : json["children"]) {
            auto type = TypeFromString(childJson["type"]);
            auto child = Create(type);
            child->Deserialize(childJson);
            AddChild(std::move(child));
        }
    }
}

std::unique_ptr<UIElement> UIElement::Create(WidgetType type) {
    switch (type) {
        case WidgetType::Panel: return std::make_unique<Panel>();
        case WidgetType::Label: return std::make_unique<Label>();
        case WidgetType::Button: return std::make_unique<Button>();
        case WidgetType::Image: return std::make_unique<Image>();
        case WidgetType::TextField: return std::make_unique<TextField>();
        case WidgetType::Checkbox: return std::make_unique<Checkbox>();
        case WidgetType::Slider: return std::make_unique<Slider>();
        case WidgetType::ContainerVertical: return std::make_unique<ContainerVertical>();
        case WidgetType::ContainerHorizontal: return std::make_unique<ContainerHorizontal>();
        default: return std::make_unique<Panel>();
    }
}

WidgetType UIElement::TypeFromString(const std::string& str) {
    if (str == "Panel") return WidgetType::Panel;
    if (str == "Label") return WidgetType::Label;
    if (str == "Button") return WidgetType::Button;
    if (str == "Image") return WidgetType::Image;
    if (str == "TextField") return WidgetType::TextField;
    if (str == "Checkbox") return WidgetType::Checkbox;
    if (str == "Slider") return WidgetType::Slider;
    if (str == "ContainerVertical") return WidgetType::ContainerVertical;
    if (str == "ContainerHorizontal") return WidgetType::ContainerHorizontal;
    return WidgetType::Panel;
}

std::string UIElement::TypeToString(WidgetType type) {
    switch (type) {
        case WidgetType::Panel: return "Panel";
        case WidgetType::Label: return "Label";
        case WidgetType::Button: return "Button";
        case WidgetType::Image: return "Image";
        case WidgetType::TextField: return "TextField";
        case WidgetType::Checkbox: return "Checkbox";
        case WidgetType::Slider: return "Slider";
        case WidgetType::ContainerVertical: return "ContainerVertical";
        case WidgetType::ContainerHorizontal: return "ContainerHorizontal";
        default: return "Panel";
    }
}

// Widget implementations
void Panel::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(40, 40, 45, 255));
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(80, 80, 85, 255));
}

void Label::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    draw_list->AddText(pos, IM_COL32(255, 255, 255, 255), m_text.c_str());
}

nlohmann::json Label::Serialize() const {
    auto j = UIElement::Serialize();
    j["text"] = m_text;
    return j;
}

void Label::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("text")) m_text = json["text"];
}

void Button::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    
    ImU32 bgColor = IM_COL32(60, 60, 60, 255);
    if (state == WidgetState::Hover) bgColor = IM_COL32(90, 90, 90, 255);
    if (state == WidgetState::Active) bgColor = IM_COL32(120, 120, 120, 255);
    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, 3.0f);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(100, 100, 100, 255), 3.0f);
    
    ImVec2 textSize = ImGui::CalcTextSize(m_text.c_str());
    ImVec2 textPos(pos.x + (size.x - textSize.x) / 2, pos.y + (size.y - textSize.y) / 2);
    draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), m_text.c_str());
}

nlohmann::json Button::Serialize() const {
    auto j = UIElement::Serialize();
    j["text"] = m_text;
    return j;
}

void Button::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("text")) m_text = json["text"];
}

void Image::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(80, 80, 80, 255));
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(120, 120, 120, 255));
    
    ImVec2 textPos(pos.x + 5, pos.y + 5);
    draw_list->AddText(textPos, IM_COL32(150, 150, 150, 255), "[Image]");
}

nlohmann::json Image::Serialize() const {
    auto j = UIElement::Serialize();
    j["imagePath"] = m_imagePath;
    return j;
}

void Image::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("imagePath")) m_imagePath = json["imagePath"];
}

void TextField::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    
    ImU32 bgColor = IM_COL32(35, 35, 35, 255);
    if (state == WidgetState::Focused) bgColor = IM_COL32(45, 45, 45, 255);
    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(80, 80, 80, 255));
    
    ImVec2 textPos(pos.x + 5, pos.y + 5);
    draw_list->AddText(textPos, IM_COL32(220, 220, 220, 255), m_text.c_str());
}

nlohmann::json TextField::Serialize() const {
    auto j = UIElement::Serialize();
    j["text"] = m_text;
    return j;
}

void TextField::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("text")) m_text = json["text"];
}

void Checkbox::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    float checkSize = 20.0f;
    
    draw_list->AddRect(pos, ImVec2(pos.x + checkSize, pos.y + checkSize), IM_COL32(150, 150, 150, 255));
    if (m_checked) {
        draw_list->AddRectFilled(
            ImVec2(pos.x + 4, pos.y + 4),
            ImVec2(pos.x + checkSize - 4, pos.y + checkSize - 4),
            IM_COL32(30, 164, 235, 255)
        );
    }
}

nlohmann::json Checkbox::Serialize() const {
    auto j = UIElement::Serialize();
    j["checked"] = m_checked;
    return j;
}

void Checkbox::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("checked")) m_checked = json["checked"];
}

void Slider::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    
    float trackHeight = 4.0f;
    ImVec2 trackPos(pos.x, pos.y + (size.y - trackHeight) / 2);
    draw_list->AddRectFilled(trackPos, ImVec2(trackPos.x + size.x, trackPos.y + trackHeight), IM_COL32(60, 60, 60, 255));
    
    float handleX = pos.x + (size.x * m_value);
    float handleRadius = 8.0f;
    draw_list->AddCircleFilled(ImVec2(handleX, pos.y + size.y / 2), handleRadius, IM_COL32(30, 164, 235, 255));
}

nlohmann::json Slider::Serialize() const {
    auto j = UIElement::Serialize();
    j["value"] = m_value;
    j["min"] = m_min;
    j["max"] = m_max;
    return j;
}

void Slider::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("value")) m_value = json["value"];
    if (json.contains("min")) m_min = json["min"];
    if (json.contains("max")) m_max = json["max"];
}

void ContainerVertical::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(100, 150, 100, 255), 0.0f, 0, 1.0f);
}

nlohmann::json ContainerVertical::Serialize() const {
    auto j = UIElement::Serialize();
    j["spacing"] = m_spacing;
    j["padding"] = m_padding;
    return j;
}

void ContainerVertical::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("spacing")) m_spacing = json["spacing"];
    if (json.contains("padding")) m_padding = json["padding"];
}

void ContainerHorizontal::Render(WidgetState state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos(m_rect.x, m_rect.y);
    ImVec2 size(m_rect.width, m_rect.height);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(150, 100, 100, 255), 0.0f, 0, 1.0f);
}

nlohmann::json ContainerHorizontal::Serialize() const {
    auto j = UIElement::Serialize();
    j["spacing"] = m_spacing;
    j["padding"] = m_padding;
    return j;
}

void ContainerHorizontal::Deserialize(const nlohmann::json& json) {
    UIElement::Deserialize(json);
    if (json.contains("spacing")) m_spacing = json["spacing"];
    if (json.contains("padding")) m_padding = json["padding"];
}

} // namespace SkinningStudio
