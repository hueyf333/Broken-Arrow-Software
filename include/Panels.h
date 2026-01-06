#pragma once

#include "UIElement.h"
#include <vector>

namespace SkinningStudio {

class HierarchyPanel {
public:
    HierarchyPanel();
    ~HierarchyPanel();

    void Render();

private:
    void RenderElement(UIElement* element, int depth = 0);
    void HandleDragDrop(UIElement* element);

    UIElement* m_draggedElement;
};

class InspectorPanel {
public:
    InspectorPanel();
    ~InspectorPanel();

    void Render();
    void SetSelection(const std::vector<UIElement*>& selection);

private:
    void RenderProperties(UIElement* element);
    void RenderStyleOverrides(UIElement* element);
    void RenderStateOverrides(UIElement* element);

    std::vector<UIElement*> m_selection;
};

class AssetPanel {
public:
    AssetPanel();
    ~AssetPanel();

    void Render();

private:
    void RenderThemes();
    void RenderFonts();
    void RenderImages();
    void RenderPalettes();

    int m_selectedTab;
};

class CanvasPanel {
public:
    CanvasPanel();
    ~CanvasPanel();

    void Render();

private:
    void RenderToolbar();
    void RenderCanvas();
};

class PreviewPanel {
public:
    PreviewPanel();
    ~PreviewPanel();

    void Render();

private:
    void RenderPreview();
};

} // namespace SkinningStudio
