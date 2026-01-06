#include "Canvas.h"
#include "Application.h"
#include "Project.h"
#include <imgui.h>
#include <algorithm>

namespace SkinningStudio {

Canvas::Canvas()
    : m_zoom(1.0f)
    , m_panX(0.0f)
    , m_panY(0.0f)
    , m_gridEnabled(true)
    , m_snapEnabled(false)
    , m_gizmosEnabled(true)
    , m_gridSize(20.0f)
    , m_isMarqueeSelect(false)
    , m_isDragging(false)
    , m_isResizing(false)
    , m_resizeHandle(-1)
{
}

Canvas::~Canvas() = default;

void Canvas::Initialize() {
}

void Canvas::Update(float deltaTime) {
}

void Canvas::Render() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    // Background
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
                            IM_COL32(45, 45, 48, 255));
    
    // Grid
    if (m_gridEnabled) {
        RenderGrid();
    }
    
    // Render elements
    UIElement* root = Application::Instance().GetProject()->GetRoot();
    if (root) {
        RenderElements(root);
    }
    
    // Selection and gizmos
    if (m_gizmosEnabled) {
        RenderSelection();
    }
    
    // Marquee selection
    if (m_isMarqueeSelect) {
        RenderMarquee();
    }
}

void Canvas::SetZoom(float zoom) {
    m_zoom = std::max(0.1f, std::min(10.0f, zoom));
}

void Canvas::SetPan(float x, float y) {
    m_panX = x;
    m_panY = y;
}

void Canvas::SetSelection(const std::vector<UIElement*>& elements) {
    m_selection = elements;
}

void Canvas::ClearSelection() {
    m_selection.clear();
}

void Canvas::AddToSelection(UIElement* element) {
    if (std::find(m_selection.begin(), m_selection.end(), element) == m_selection.end()) {
        m_selection.push_back(element);
    }
}

void Canvas::RemoveFromSelection(UIElement* element) {
    m_selection.erase(
        std::remove(m_selection.begin(), m_selection.end(), element),
        m_selection.end()
    );
}

UIElement* Canvas::HitTest(float x, float y) {
    // Convert to canvas coordinates
    ScreenToCanvas(x, y);
    
    // Test from front to back
    UIElement* root = Application::Instance().GetProject()->GetRoot();
    if (root) {
        // Recursive hit test
        // TODO: Implement proper hit testing
    }
    
    return nullptr;
}

void Canvas::BeginMarqueeSelect(float x, float y) {
    m_isMarqueeSelect = true;
    m_marqueeStartX = x;
    m_marqueeStartY = y;
    m_marqueeEndX = x;
    m_marqueeEndY = y;
}

void Canvas::UpdateMarqueeSelect(float x, float y) {
    m_marqueeEndX = x;
    m_marqueeEndY = y;
}

void Canvas::EndMarqueeSelect() {
    m_isMarqueeSelect = false;
    // Select elements within marquee
}

void Canvas::BeginDrag() {
    m_isDragging = true;
}

void Canvas::UpdateDrag(float dx, float dy) {
    if (!m_isDragging) return;
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.x += dx;
        rect.y += dy;
        
        if (m_snapEnabled) {
            rect.x = SnapToGrid(rect.x);
            rect.y = SnapToGrid(rect.y);
        }
        
        element->SetRect(rect);
    }
}

void Canvas::EndDrag() {
    m_isDragging = false;
}

void Canvas::BeginResize(int handleIndex) {
    m_isResizing = true;
    m_resizeHandle = handleIndex;
    
    // Store original rects
    m_originalRects.clear();
    for (auto* element : m_selection) {
        m_originalRects.push_back(element->GetRect());
    }
}

void Canvas::UpdateResize(float x, float y) {
    if (!m_isResizing || m_selection.empty()) return;
    
    // TODO: Implement proper resize based on handle
}

void Canvas::EndResize() {
    m_isResizing = false;
    m_originalRects.clear();
}

void Canvas::BringForward() {
    // TODO: Implement z-order manipulation
}

void Canvas::SendBackward() {
    // TODO: Implement z-order manipulation
}

void Canvas::BringToFront() {
    // TODO: Implement z-order manipulation
}

void Canvas::SendToBack() {
    // TODO: Implement z-order manipulation
}

void Canvas::Group() {
    // TODO: Implement grouping
}

void Canvas::Ungroup() {
    // TODO: Implement ungrouping
}

void Canvas::AlignLeft() {
    if (m_selection.size() < 2) return;
    
    float minX = m_selection[0]->GetRect().x;
    for (auto* element : m_selection) {
        minX = std::min(minX, element->GetRect().x);
    }
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.x = minX;
        element->SetRect(rect);
    }
}

void Canvas::AlignRight() {
    if (m_selection.size() < 2) return;
    
    float maxX = m_selection[0]->GetRect().x + m_selection[0]->GetRect().width;
    for (auto* element : m_selection) {
        maxX = std::max(maxX, element->GetRect().x + element->GetRect().width);
    }
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.x = maxX - rect.width;
        element->SetRect(rect);
    }
}

void Canvas::AlignTop() {
    if (m_selection.size() < 2) return;
    
    float minY = m_selection[0]->GetRect().y;
    for (auto* element : m_selection) {
        minY = std::min(minY, element->GetRect().y);
    }
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.y = minY;
        element->SetRect(rect);
    }
}

void Canvas::AlignBottom() {
    if (m_selection.size() < 2) return;
    
    float maxY = m_selection[0]->GetRect().y + m_selection[0]->GetRect().height;
    for (auto* element : m_selection) {
        maxY = std::max(maxY, element->GetRect().y + element->GetRect().height);
    }
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.y = maxY - rect.height;
        element->SetRect(rect);
    }
}

void Canvas::AlignCenterHorizontal() {
    if (m_selection.size() < 2) return;
    
    float sumX = 0;
    for (auto* element : m_selection) {
        sumX += element->GetRect().x + element->GetRect().width / 2;
    }
    float centerX = sumX / m_selection.size();
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.x = centerX - rect.width / 2;
        element->SetRect(rect);
    }
}

void Canvas::AlignCenterVertical() {
    if (m_selection.size() < 2) return;
    
    float sumY = 0;
    for (auto* element : m_selection) {
        sumY += element->GetRect().y + element->GetRect().height / 2;
    }
    float centerY = sumY / m_selection.size();
    
    for (auto* element : m_selection) {
        Rect rect = element->GetRect();
        rect.y = centerY - rect.height / 2;
        element->SetRect(rect);
    }
}

void Canvas::DistributeHorizontal() {
    // TODO: Implement distribution
}

void Canvas::DistributeVertical() {
    // TODO: Implement distribution
}

void Canvas::ScreenToCanvas(float& x, float& y) const {
    x = (x - m_panX) / m_zoom;
    y = (y - m_panY) / m_zoom;
}

void Canvas::CanvasToScreen(float& x, float& y) const {
    x = x * m_zoom + m_panX;
    y = y * m_zoom + m_panY;
}

void Canvas::RenderGrid() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    float gridStep = m_gridSize * m_zoom;
    
    for (float x = fmod(m_panX, gridStep); x < canvas_size.x; x += gridStep) {
        draw_list->AddLine(
            ImVec2(canvas_pos.x + x, canvas_pos.y),
            ImVec2(canvas_pos.x + x, canvas_pos.y + canvas_size.y),
            IM_COL32(60, 60, 60, 100)
        );
    }
    
    for (float y = fmod(m_panY, gridStep); y < canvas_size.y; y += gridStep) {
        draw_list->AddLine(
            ImVec2(canvas_pos.x, canvas_pos.y + y),
            ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + y),
            IM_COL32(60, 60, 60, 100)
        );
    }
}

void Canvas::RenderElements(UIElement* element) {
    if (!element || !element->IsVisible()) return;
    
    // Render this element
    element->Render();
    
    // Render children
    for (const auto& child : element->GetChildren()) {
        RenderElements(child.get());
    }
}

void Canvas::RenderSelection() {
    for (auto* element : m_selection) {
        if (element) {
            element->RenderGizmos(true);
            
            Rect rect = element->GetRect();
            RenderResizeHandles(rect);
        }
    }
}

void Canvas::RenderMarquee() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    float x1 = std::min(m_marqueeStartX, m_marqueeEndX);
    float y1 = std::min(m_marqueeStartY, m_marqueeEndY);
    float x2 = std::max(m_marqueeStartX, m_marqueeEndX);
    float y2 = std::max(m_marqueeStartY, m_marqueeEndY);
    
    draw_list->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(30, 164, 235, 255), 0.0f, 0, 2.0f);
    draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(30, 164, 235, 50));
}

void Canvas::RenderResizeHandles(const Rect& rect) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    const float handleSize = 8.0f;
    const ImU32 handleColor = IM_COL32(255, 255, 255, 255);
    const ImU32 handleBorder = IM_COL32(30, 164, 235, 255);
    
    // 8 handles: corners and edges
    ImVec2 handles[8] = {
        ImVec2(rect.x, rect.y), // Top-left
        ImVec2(rect.x + rect.width / 2, rect.y), // Top-center
        ImVec2(rect.x + rect.width, rect.y), // Top-right
        ImVec2(rect.x + rect.width, rect.y + rect.height / 2), // Right-center
        ImVec2(rect.x + rect.width, rect.y + rect.height), // Bottom-right
        ImVec2(rect.x + rect.width / 2, rect.y + rect.height), // Bottom-center
        ImVec2(rect.x, rect.y + rect.height), // Bottom-left
        ImVec2(rect.x, rect.y + rect.height / 2), // Left-center
    };
    
    for (int i = 0; i < 8; i++) {
        draw_list->AddRectFilled(
            ImVec2(handles[i].x - handleSize/2, handles[i].y - handleSize/2),
            ImVec2(handles[i].x + handleSize/2, handles[i].y + handleSize/2),
            handleColor
        );
        draw_list->AddRect(
            ImVec2(handles[i].x - handleSize/2, handles[i].y - handleSize/2),
            ImVec2(handles[i].x + handleSize/2, handles[i].y + handleSize/2),
            handleBorder
        );
    }
}

float Canvas::SnapToGrid(float value) const {
    return std::round(value / m_gridSize) * m_gridSize;
}

} // namespace SkinningStudio
