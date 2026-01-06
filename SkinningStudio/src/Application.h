#pragma once

#include "core/UIElement.h"
#include "core/StyleSystem.h"
#include "core/CommandManager.h"
#include "panels/HierarchyPanel.h"
#include "panels/InspectorPanel.h"
#include "panels/AssetLibraryPanel.h"
#include "panels/CanvasPanel.h"
#include "panels/RuntimePreviewPanel.h"
#include "serialization/ProjectSerializer.h"
#include "tools/Validator.h"
#include "tools/ThemeOptimizer.h"

#include <SDL.h>
#include <vector>
#include <memory>
#include <string>

class Application {
public:
    Application();
    ~Application();
    
    bool Initialize();
    void Run();
    void Shutdown();
    
private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    
    bool m_running = false;
    std::string m_currentProjectPath;
    std::vector<std::string> m_recentFiles;
    bool m_projectModified = false;
    
    // Data
    std::vector<std::shared_ptr<UIElement>> m_rootElements;
    std::shared_ptr<Theme> m_theme;
    std::shared_ptr<StyleResolver> m_styleResolver;
    CommandManager m_commandManager;
    
    UIElement* m_selectedElement = nullptr;
    
    // Panels
    HierarchyPanel m_hierarchyPanel;
    InspectorPanel m_inspectorPanel;
    AssetLibraryPanel m_assetLibraryPanel;
    CanvasPanel m_canvasPanel;
    RuntimePreviewPanel m_runtimePreviewPanel;
    
    // UI State
    bool m_showAboutDialog = false;
    bool m_showPreferencesDialog = false;
    bool m_showValidationDialog = false;
    bool m_showOptimizerDialog = false;
    bool m_showShortcutsOverlay = false;
    
    std::vector<ValidationIssue> m_validationIssues;
    OptimizationResult m_optimizationResult;
    
    // Methods
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
    
    void RenderMenuBar();
    void RenderPanels();
    void RenderDialogs();
    
    void HandleKeyboardShortcuts();
    
    // File operations
    void NewProject();
    void OpenProject();
    void SaveProject();
    void SaveProjectAs();
    void ExitApplication();
    
    // Edit operations
    void Undo();
    void Redo();
    void Cut();
    void Copy();
    void Paste();
    void Duplicate();
    void Delete();
    void SelectAll();
    
    // View operations
    void ResetLayout();
    
    // Tools
    void RunValidation();
    void RunOptimizer();
    void ShowPreferences();
    
    // Export
    void ExportTheme();
    void ImportTheme();
    void ExportRuntimePackage();
    
    // Helpers
    void ShowErrorDialog(const std::string& message);
    void ShowToast(const std::string& message);
    void AddRecentFile(const std::string& path);
    bool PromptSaveIfModified();
};
