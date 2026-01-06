#pragma once

#include <memory>
#include <vector>
#include <string>

namespace SkinningStudio {

class UIElement;
class HierarchyPanel;
class InspectorPanel;
class AssetPanel;
class CanvasPanel;
class PreviewPanel;

class Editor {
public:
    Editor();
    ~Editor();

    void Initialize();
    void Update(float deltaTime);
    void Render();

    // Menu actions
    void NewProject();
    void OpenProject();
    void SaveProject();
    void SaveProjectAs();
    void ImportTheme();
    void ExportTheme();
    void ExportRuntimePackage();
    void Exit();

    void Undo();
    void Redo();
    void Cut();
    void Copy();
    void Paste();
    void Duplicate();
    void Delete();
    void SelectAll();

    void ToggleHierarchyPanel();
    void ToggleInspectorPanel();
    void ToggleAssetPanel();
    void ToggleGrid();
    void ToggleSnap();
    void ToggleGizmos();
    void ResetLayout();

    void ValidateProject();
    void OptimizeTheme();
    void ShowPreferences();
    void ShowShortcuts();
    void ShowAbout();

    // Panel access
    HierarchyPanel* GetHierarchyPanel() const { return m_hierarchyPanel.get(); }
    InspectorPanel* GetInspectorPanel() const { return m_inspectorPanel.get(); }
    AssetPanel* GetAssetPanel() const { return m_assetPanel.get(); }

    // Selection
    void SetSelection(const std::vector<UIElement*>& elements);
    const std::vector<UIElement*>& GetSelection() const { return m_selection; }

    // Recent files
    void AddRecentFile(const std::string& path);
    const std::vector<std::string>& GetRecentFiles() const { return m_recentFiles; }

private:
    void RenderMenuBar();
    void RenderDockSpace();
    void RenderPanels();
    void LoadRecentFiles();
    void SaveRecentFiles();

    std::unique_ptr<HierarchyPanel> m_hierarchyPanel;
    std::unique_ptr<InspectorPanel> m_inspectorPanel;
    std::unique_ptr<AssetPanel> m_assetPanel;
    std::unique_ptr<CanvasPanel> m_canvasPanel;
    std::unique_ptr<PreviewPanel> m_previewPanel;

    std::vector<UIElement*> m_selection;
    std::vector<std::string> m_recentFiles;

    bool m_showHierarchy;
    bool m_showInspector;
    bool m_showAssets;
    bool m_showPreferences;
    bool m_showShortcuts;
    bool m_showAbout;
    bool m_showValidation;
    bool m_showOptimizer;

    std::string m_currentProjectPath;
};

} // namespace SkinningStudio
