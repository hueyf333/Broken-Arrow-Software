#pragma once

#include "UIElement.h"
#include <vector>
#include <memory>

namespace SkinningStudio {

enum class SelectionMode {
    Replace,
    Add,
    Toggle
};

class Canvas {
public:
    Canvas();
    ~Canvas();

    void Initialize();
    void Update(float deltaTime);
    void Render();

    // View transformation
    void SetZoom(float zoom);
    float GetZoom() const { return m_zoom; }
    void SetPan(float x, float y);
    void GetPan(float& x, float& y) const { x = m_panX; y = m_panY; }

    // Grid and snap
    void SetGridEnabled(bool enabled) { m_gridEnabled = enabled; }
    bool IsGridEnabled() const { return m_gridEnabled; }
    void SetSnapEnabled(bool enabled) { m_snapEnabled = enabled; }
    bool IsSnapEnabled() const { return m_snapEnabled; }
    void SetGridSize(float size) { m_gridSize = size; }
    float GetGridSize() const { return m_gridSize; }

    // Gizmos
    void SetGizmosEnabled(bool enabled) { m_gizmosEnabled = enabled; }
    bool IsGizmosEnabled() const { return m_gizmosEnabled; }

    // Selection
    void SetSelection(const std::vector<UIElement*>& elements);
    const std::vector<UIElement*>& GetSelection() const { return m_selection; }
    void ClearSelection();
    void AddToSelection(UIElement* element);
    void RemoveFromSelection(UIElement* element);

    // Canvas operations
    UIElement* HitTest(float x, float y);
    void BeginMarqueeSelect(float x, float y);
    void UpdateMarqueeSelect(float x, float y);
    void EndMarqueeSelect();

    // Transform operations
    void BeginDrag();
    void UpdateDrag(float dx, float dy);
    void EndDrag();

    void BeginResize(int handleIndex);
    void UpdateResize(float x, float y);
    void EndResize();

    // Arrange operations
    void BringForward();
    void SendBackward();
    void BringToFront();
    void SendToBack();
    void Group();
    void Ungroup();

    // Alignment
    void AlignLeft();
    void AlignRight();
    void AlignTop();
    void AlignBottom();
    void AlignCenterHorizontal();
    void AlignCenterVertical();
    void DistributeHorizontal();
    void DistributeVertical();

    // Coordinate conversion
    void ScreenToCanvas(float& x, float& y) const;
    void CanvasToScreen(float& x, float& y) const;

private:
    void RenderGrid();
    void RenderElements(UIElement* element);
    void RenderSelection();
    void RenderMarquee();
    void RenderResizeHandles(const Rect& rect);

    float SnapToGrid(float value) const;

    float m_zoom;
    float m_panX;
    float m_panY;
    bool m_gridEnabled;
    bool m_snapEnabled;
    bool m_gizmosEnabled;
    float m_gridSize;

    std::vector<UIElement*> m_selection;

    bool m_isMarqueeSelect;
    float m_marqueeStartX;
    float m_marqueeStartY;
    float m_marqueeEndX;
    float m_marqueeEndY;

    bool m_isDragging;
    float m_dragStartX;
    float m_dragStartY;

    bool m_isResizing;
    int m_resizeHandle;
    float m_resizeStartX;
    float m_resizeStartY;
    std::vector<Rect> m_originalRects;
};

} // namespace SkinningStudio
