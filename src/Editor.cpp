#include "Editor.h"
#include "Application.h"
#include "Panels.h"
#include "Project.h"
#include "CommandManager.h"
#include "Canvas.h"
#include <imgui.h>
#include <fstream>

namespace SkinningStudio {

Editor::Editor()
    : m_showHierarchy(true)
    , m_showInspector(true)
    , m_showAssets(true)
    , m_showPreferences(false)
    , m_showShortcuts(false)
    , m_showAbout(false)
    , m_showValidation(false)
    , m_showOptimizer(false)
{
}

Editor::~Editor() = default;

void Editor::Initialize() {
    m_hierarchyPanel = std::make_unique<HierarchyPanel>();
    m_inspectorPanel = std::make_unique<InspectorPanel>();
    m_assetPanel = std::make_unique<AssetPanel>();
    m_canvasPanel = std::make_unique<CanvasPanel>();
    m_previewPanel = std::make_unique<PreviewPanel>();
    
    LoadRecentFiles();
}

void Editor::Update(float deltaTime) {
    // Update panels
}

void Editor::Render() {
    RenderMenuBar();
    RenderPanels();
    
    // Modal dialogs
    if (m_showAbout) {
        ImGui::OpenPopup("About SkinningStudio");
        if (ImGui::BeginPopupModal("About SkinningStudio", &m_showAbout, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("SkinningStudio");
            ImGui::Text("Version: %s", SKINNING_STUDIO_VERSION);
            ImGui::Text("WYSIWYG GUI Editor");
            ImGui::Separator();
            ImGui::Text("Built with Dear ImGui, SDL2, and OpenGL");
            if (ImGui::Button("Close")) {
                m_showAbout = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    
    if (m_showShortcuts) {
        ImGui::OpenPopup("Keyboard Shortcuts");
        if (ImGui::BeginPopupModal("Keyboard Shortcuts", &m_showShortcuts, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("File Operations:");
            ImGui::BulletText("Ctrl+N: New Project");
            ImGui::BulletText("Ctrl+O: Open Project");
            ImGui::BulletText("Ctrl+S: Save Project");
            ImGui::BulletText("Ctrl+Shift+S: Save As");
            ImGui::Separator();
            ImGui::Text("Edit Operations:");
            ImGui::BulletText("Ctrl+Z: Undo");
            ImGui::BulletText("Ctrl+Y: Redo");
            ImGui::BulletText("Ctrl+X: Cut");
            ImGui::BulletText("Ctrl+C: Copy");
            ImGui::BulletText("Ctrl+V: Paste");
            ImGui::BulletText("Ctrl+D: Duplicate");
            ImGui::BulletText("Delete: Delete");
            ImGui::BulletText("Ctrl+A: Select All");
            if (ImGui::Button("Close")) {
                m_showShortcuts = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    
    if (m_showPreferences) {
        ImGui::OpenPopup("Preferences");
        if (ImGui::BeginPopupModal("Preferences", &m_showPreferences)) {
            ImGui::Text("Grid Size:");
            static float gridSize = 10.0f;
            ImGui::SliderFloat("##gridsize", &gridSize, 5.0f, 50.0f);
            
            if (ImGui::Button("OK")) {
                m_showPreferences = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                m_showPreferences = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void Editor::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) NewProject();
            if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenProject();
            if (ImGui::MenuItem("Save", "Ctrl+S")) SaveProject();
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveProjectAs();
            ImGui::Separator();
            if (ImGui::MenuItem("Import Theme...")) ImportTheme();
            if (ImGui::MenuItem("Export Theme...")) ExportTheme();
            if (ImGui::MenuItem("Export Runtime Package...")) ExportRuntimePackage();
            ImGui::Separator();
            if (ImGui::BeginMenu("Recent Files")) {
                if (m_recentFiles.empty()) {
                    ImGui::MenuItem("(None)", nullptr, false, false);
                } else {
                    for (const auto& file : m_recentFiles) {
                        if (ImGui::MenuItem(file.c_str())) {
                            // Load file
                        }
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) Exit();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")) {
            bool canUndo = Application::Instance().GetCommandManager()->CanUndo();
            bool canRedo = Application::Instance().GetCommandManager()->CanRedo();
            
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, canUndo)) Undo();
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, canRedo)) Redo();
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) Cut();
            if (ImGui::MenuItem("Copy", "Ctrl+C")) Copy();
            if (ImGui::MenuItem("Paste", "Ctrl+V")) Paste();
            if (ImGui::MenuItem("Duplicate", "Ctrl+D")) Duplicate();
            if (ImGui::MenuItem("Delete", "Delete")) Delete();
            ImGui::Separator();
            if (ImGui::MenuItem("Select All", "Ctrl+A")) SelectAll();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Hierarchy", nullptr, &m_showHierarchy);
            ImGui::MenuItem("Inspector", nullptr, &m_showInspector);
            ImGui::MenuItem("Assets", nullptr, &m_showAssets);
            ImGui::Separator();
            Canvas* canvas = Application::Instance().GetCanvas();
            bool gridEnabled = canvas->IsGridEnabled();
            bool snapEnabled = canvas->IsSnapEnabled();
            bool gizmosEnabled = canvas->IsGizmosEnabled();
            if (ImGui::MenuItem("Grid", nullptr, &gridEnabled)) canvas->SetGridEnabled(gridEnabled);
            if (ImGui::MenuItem("Snap to Grid", nullptr, &snapEnabled)) canvas->SetSnapEnabled(snapEnabled);
            if (ImGui::MenuItem("Gizmos", nullptr, &gizmosEnabled)) canvas->SetGizmosEnabled(gizmosEnabled);
            ImGui::Separator();
            if (ImGui::MenuItem("Reset Layout")) ResetLayout();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Validate Project")) ValidateProject();
            if (ImGui::MenuItem("Theme Optimizer")) OptimizeTheme();
            ImGui::Separator();
            if (ImGui::MenuItem("Preferences...")) ShowPreferences();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Keyboard Shortcuts")) ShowShortcuts();
            ImGui::Separator();
            if (ImGui::MenuItem("About")) ShowAbout();
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }
    
    // Handle keyboard shortcuts
    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N)) NewProject();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O)) OpenProject();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) {
        if (io.KeyShift) SaveProjectAs();
        else SaveProject();
    }
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z)) Undo();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y)) Redo();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_X)) Cut();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C)) Copy();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V)) Paste();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_D)) Duplicate();
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) Delete();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_A)) SelectAll();
}

void Editor::RenderPanels() {
    if (m_showHierarchy && m_hierarchyPanel) {
        ImGui::Begin("Hierarchy", &m_showHierarchy);
        m_hierarchyPanel->Render();
        ImGui::End();
    }
    
    if (m_showInspector && m_inspectorPanel) {
        ImGui::Begin("Inspector", &m_showInspector);
        m_inspectorPanel->Render();
        ImGui::End();
    }
    
    if (m_showAssets && m_assetPanel) {
        ImGui::Begin("Assets", &m_showAssets);
        m_assetPanel->Render();
        ImGui::End();
    }
    
    if (m_canvasPanel) {
        ImGui::Begin("Canvas");
        m_canvasPanel->Render();
        ImGui::End();
    }
    
    if (m_previewPanel) {
        ImGui::Begin("Runtime Preview");
        m_previewPanel->Render();
        ImGui::End();
    }
}

void Editor::NewProject() {
    Application::Instance().GetProject()->New();
}

void Editor::OpenProject() {
    // TODO: File dialog
}

void Editor::SaveProject() {
    auto* project = Application::Instance().GetProject();
    if (project->GetPath().empty()) {
        SaveProjectAs();
    } else {
        project->Save();
    }
}

void Editor::SaveProjectAs() {
    // TODO: File dialog
}

void Editor::ImportTheme() {
    // TODO: File dialog
}

void Editor::ExportTheme() {
    // TODO: File dialog
}

void Editor::ExportRuntimePackage() {
    // TODO: Folder dialog
    Application::Instance().GetProject()->ExportRuntimePackage("./export");
}

void Editor::Exit() {
    SDL_Event quit_event;
    quit_event.type = SDL_QUIT;
    SDL_PushEvent(&quit_event);
}

void Editor::Undo() {
    Application::Instance().GetCommandManager()->Undo();
}

void Editor::Redo() {
    Application::Instance().GetCommandManager()->Redo();
}

void Editor::Cut() {
    Copy();
    Delete();
}

void Editor::Copy() {
    // TODO: Implement clipboard
}

void Editor::Paste() {
    // TODO: Implement clipboard
}

void Editor::Duplicate() {
    // TODO: Implement duplication
}

void Editor::Delete() {
    // TODO: Implement deletion
}

void Editor::SelectAll() {
    // TODO: Implement select all
}

void Editor::ToggleHierarchyPanel() {
    m_showHierarchy = !m_showHierarchy;
}

void Editor::ToggleInspectorPanel() {
    m_showInspector = !m_showInspector;
}

void Editor::ToggleAssetPanel() {
    m_showAssets = !m_showAssets;
}

void Editor::ToggleGrid() {
    auto* canvas = Application::Instance().GetCanvas();
    canvas->SetGridEnabled(!canvas->IsGridEnabled());
}

void Editor::ToggleSnap() {
    auto* canvas = Application::Instance().GetCanvas();
    canvas->SetSnapEnabled(!canvas->IsSnapEnabled());
}

void Editor::ToggleGizmos() {
    auto* canvas = Application::Instance().GetCanvas();
    canvas->SetGizmosEnabled(!canvas->IsGizmosEnabled());
}

void Editor::ResetLayout() {
    // TODO: Implement layout reset
}

void Editor::ValidateProject() {
    m_showValidation = true;
}

void Editor::OptimizeTheme() {
    m_showOptimizer = true;
}

void Editor::ShowPreferences() {
    m_showPreferences = true;
}

void Editor::ShowShortcuts() {
    m_showShortcuts = true;
}

void Editor::ShowAbout() {
    m_showAbout = true;
}

void Editor::SetSelection(const std::vector<UIElement*>& elements) {
    m_selection = elements;
    if (m_inspectorPanel) {
        m_inspectorPanel->SetSelection(elements);
    }
}

void Editor::AddRecentFile(const std::string& path) {
    // Remove if already exists
    m_recentFiles.erase(
        std::remove(m_recentFiles.begin(), m_recentFiles.end(), path),
        m_recentFiles.end()
    );
    
    // Add to front
    m_recentFiles.insert(m_recentFiles.begin(), path);
    
    // Limit to 10 recent files
    if (m_recentFiles.size() > 10) {
        m_recentFiles.resize(10);
    }
    
    SaveRecentFiles();
}

void Editor::LoadRecentFiles() {
    // TODO: Load from config file
}

void Editor::SaveRecentFiles() {
    // TODO: Save to config file
}

} // namespace SkinningStudio
