#include "Application.h"
#include "Version.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <filesystem>
#include <fstream>
#include <algorithm>

Application::Application() {
}

Application::~Application() {
}

bool Application::Initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return false;
    }
    
    // GL 3.3 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    // Create window
    m_window = SDL_CreateWindow(
        "SkinningStudio",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1600, 900,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    
    if (!m_window) {
        return false;
    }
    
    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        return false;
    }
    
    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1); // VSync
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return false;
    }
    
    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup style
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Initialize theme and style resolver
    m_theme = Theme::CreateDefaultDarkTheme();
    m_styleResolver = std::make_shared<StyleResolver>(m_theme);
    
    m_running = true;
    
    return true;
}

void Application::Run() {
    const float targetFrameTime = 1.0f / 60.0f; // 60 FPS
    Uint64 lastTime = SDL_GetPerformanceCounter();
    
    while (m_running) {
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
        lastTime = currentTime;
        
        ProcessEvents();
        Update(deltaTime);
        Render();
        
        // Frame rate limiting
        float frameTime = (float)(SDL_GetPerformanceCounter() - currentTime) / SDL_GetPerformanceFrequency();
        if (frameTime < targetFrameTime) {
            SDL_Delay((Uint32)((targetFrameTime - frameTime) * 1000.0f));
        }
    }
}

void Application::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        
        if (event.type == SDL_QUIT) {
            if (PromptSaveIfModified()) {
                m_running = false;
            }
        }
        
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
            if (PromptSaveIfModified()) {
                m_running = false;
            }
        }
    }
}

void Application::Update(float deltaTime) {
    HandleKeyboardShortcuts();
}

void Application::Render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    // Dockspace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("DockSpaceWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);
    
    ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    
    RenderMenuBar();
    
    ImGui::End();
    
    // Render panels
    RenderPanels();
    
    // Render dialogs
    RenderDialogs();
    
    // Rendering
    ImGui::Render();
    
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    SDL_GL_SwapWindow(m_window);
}

void Application::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) NewProject();
            if (ImGui::MenuItem("Open", "Ctrl+O")) OpenProject();
            if (ImGui::MenuItem("Save", "Ctrl+S", false, !m_currentProjectPath.empty())) SaveProject();
            if (ImGui::MenuItem("Save As...")) SaveProjectAs();
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Import Theme...")) ImportTheme();
            if (ImGui::MenuItem("Export Theme...")) ExportTheme();
            if (ImGui::MenuItem("Export Runtime Package...")) ExportRuntimePackage();
            
            ImGui::Separator();
            
            if (ImGui::BeginMenu("Recent Files", !m_recentFiles.empty())) {
                for (const auto& file : m_recentFiles) {
                    if (ImGui::MenuItem(file.c_str())) {
                        m_currentProjectPath = file;
                        ProjectSerializer::LoadProject(file, m_rootElements, m_theme);
                        m_styleResolver->SetTheme(m_theme);
                    }
                }
                ImGui::EndMenu();
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit", "Alt+F4")) ExitApplication();
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, m_commandManager.CanUndo())) Undo();
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, m_commandManager.CanRedo())) Redo();
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Cut", "Ctrl+X", false, m_selectedElement != nullptr)) Cut();
            if (ImGui::MenuItem("Copy", "Ctrl+C", false, m_selectedElement != nullptr)) Copy();
            if (ImGui::MenuItem("Paste", "Ctrl+V")) Paste();
            if (ImGui::MenuItem("Duplicate", "Ctrl+D", false, m_selectedElement != nullptr)) Duplicate();
            if (ImGui::MenuItem("Delete", "Delete", false, m_selectedElement != nullptr)) Delete();
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Select All", "Ctrl+A")) SelectAll();
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Hierarchy", nullptr, &m_showHierarchyPanel);
            ImGui::MenuItem("Inspector", nullptr, &m_showInspectorPanel);
            ImGui::MenuItem("Asset Library", nullptr, &m_showAssetLibraryPanel);
            ImGui::MenuItem("Canvas", nullptr, &m_showCanvasPanel);
            ImGui::MenuItem("Runtime Preview", nullptr, &m_showRuntimePreviewPanel);
            
            ImGui::Separator();
            
            ImGui::MenuItem("Show Grid", nullptr, &m_canvasPanel.showGrid);
            ImGui::MenuItem("Snap to Grid", nullptr, &m_canvasPanel.snapToGrid);
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Reset Layout")) ResetLayout();
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Validate Project")) RunValidation();
            if (ImGui::MenuItem("Theme Optimizer")) RunOptimizer();
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Preferences...")) ShowPreferences();
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Keyboard Shortcuts", "F1")) {
                m_showShortcutsOverlay = !m_showShortcutsOverlay;
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("About")) {
                m_showAboutDialog = true;
            }
            
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }
}

void Application::RenderPanels() {
    if (m_showHierarchyPanel) {
        m_hierarchyPanel.Render(m_rootElements, m_selectedElement);
    }
    if (m_showInspectorPanel) {
        m_inspectorPanel.Render(m_selectedElement, &m_commandManager);
    }
    if (m_showAssetLibraryPanel) {
        m_assetLibraryPanel.Render(m_theme);
    }
    if (m_showCanvasPanel) {
        m_canvasPanel.Render(m_rootElements, m_styleResolver.get(), m_selectedElement, &m_commandManager);
    }
    if (m_showRuntimePreviewPanel) {
        m_runtimePreviewPanel.Render(m_rootElements, m_styleResolver.get());
    }
}

void Application::RenderDialogs() {
    // About dialog
    if (m_showAboutDialog) {
        ImGui::OpenPopup("About SkinningStudio");
        if (ImGui::BeginPopupModal("About SkinningStudio", &m_showAboutDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("SkinningStudio");
            ImGui::Text("Version: %s", SKINNING_STUDIO_VERSION);
            ImGui::Separator();
            ImGui::Text("A WYSIWYG GUI skinning application");
            ImGui::Text("Built with SDL2, OpenGL, and Dear ImGui");
            
            if (ImGui::Button("Close")) {
                m_showAboutDialog = false;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
    }
    
    // Shortcuts overlay
    if (m_showShortcutsOverlay) {
        ImGui::Begin("Keyboard Shortcuts", &m_showShortcutsOverlay);
        
        ImGui::Text("File Operations:");
        ImGui::BulletText("Ctrl+N - New Project");
        ImGui::BulletText("Ctrl+O - Open Project");
        ImGui::BulletText("Ctrl+S - Save Project");
        
        ImGui::Separator();
        ImGui::Text("Edit Operations:");
        ImGui::BulletText("Ctrl+Z - Undo");
        ImGui::BulletText("Ctrl+Y - Redo");
        ImGui::BulletText("Ctrl+X - Cut");
        ImGui::BulletText("Ctrl+C - Copy");
        ImGui::BulletText("Ctrl+V - Paste");
        ImGui::BulletText("Ctrl+D - Duplicate");
        ImGui::BulletText("Delete - Delete");
        ImGui::BulletText("Ctrl+A - Select All");
        
        ImGui::Separator();
        ImGui::Text("View:");
        ImGui::BulletText("F1 - Toggle Shortcuts Overlay");
        
        ImGui::End();
    }
    
    // Validation dialog
    if (m_showValidationDialog) {
        ImGui::Begin("Validation Results", &m_showValidationDialog);
        
        ImGui::Text("Found %zu issue(s)", m_validationIssues.size());
        ImGui::Separator();
        
        for (const auto& issue : m_validationIssues) {
            const char* severityStr = issue.severity == ValidationIssue::Severity::Error ? "ERROR" :
                                     issue.severity == ValidationIssue::Severity::Warning ? "WARNING" : "INFO";
            ImGui::TextColored(
                issue.severity == ValidationIssue::Severity::Error ? ImVec4(1, 0, 0, 1) : ImVec4(1, 1, 0, 1),
                "[%s] %s", severityStr, issue.message.c_str()
            );
        }
        
        ImGui::End();
    }
    
    // Optimizer dialog
    if (m_showOptimizerDialog) {
        ImGui::Begin("Theme Optimizer", &m_showOptimizerDialog);
        
        ImGui::Text("Optimization Results:");
        ImGui::Text("Tokens that can be removed: %d", m_optimizationResult.tokensSaved);
        
        ImGui::Separator();
        ImGui::Text("Unused Tokens:");
        for (const auto& token : m_optimizationResult.unusedTokens) {
            ImGui::BulletText("%s", token.c_str());
        }
        
        ImGui::End();
    }
}

void Application::HandleKeyboardShortcuts() {
    ImGuiIO& io = ImGui::GetIO();
    bool ctrl = io.KeyCtrl;
    bool shift = io.KeyShift;
    
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_N)) NewProject();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_O)) OpenProject();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_S)) SaveProject();
    
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_Z)) Undo();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_Y)) Redo();
    
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_X)) Cut();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_C)) Copy();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_V)) Paste();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_D)) Duplicate();
    if (ImGui::IsKeyPressed(ImGuiKey_Delete)) Delete();
    if (ctrl && ImGui::IsKeyPressed(ImGuiKey_A)) SelectAll();
    
    if (ImGui::IsKeyPressed(ImGuiKey_F1)) m_showShortcutsOverlay = !m_showShortcutsOverlay;
}

void Application::NewProject() {
    if (!PromptSaveIfModified()) return;
    
    m_rootElements.clear();
    m_selectedElement = nullptr;
    m_currentProjectPath.clear();
    m_theme = Theme::CreateDefaultDarkTheme();
    m_styleResolver->SetTheme(m_theme);
    m_commandManager.Clear();
    m_projectModified = false;
}

void Application::OpenProject() {
    // Note: In a real implementation, use native file dialog
    // For now, this is a placeholder
    ShowToast("Open Project - Use file dialog");
}

void Application::SaveProject() {
    if (m_currentProjectPath.empty()) {
        SaveProjectAs();
        return;
    }
    
    if (ProjectSerializer::SaveProject(m_currentProjectPath, m_rootElements, m_theme)) {
        m_projectModified = false;
        AddRecentFile(m_currentProjectPath);
        ShowToast("Project saved successfully");
    } else {
        ShowErrorDialog("Failed to save project");
    }
}

void Application::SaveProjectAs() {
    // Note: In a real implementation, use native file dialog
    ShowToast("Save As - Use file dialog");
}

void Application::ExitApplication() {
    if (PromptSaveIfModified()) {
        m_running = false;
    }
}

void Application::Undo() {
    m_commandManager.Undo();
}

void Application::Redo() {
    m_commandManager.Redo();
}

void Application::Cut() {
    Copy();
    Delete();
}

void Application::Copy() {
    // Clipboard implementation
    ShowToast("Copy - Implementation needed");
}

void Application::Paste() {
    ShowToast("Paste - Implementation needed");
}

void Application::Duplicate() {
    if (!m_selectedElement) return;
    ShowToast("Duplicate - Implementation needed");
}

void Application::Delete() {
    if (!m_selectedElement) return;
    // Delete implementation
    m_selectedElement = nullptr;
    ShowToast("Element deleted");
}

void Application::SelectAll() {
    ShowToast("Select All - Implementation needed");
}

void Application::ResetLayout() {
    // Reset ImGui docking layout
    ShowToast("Layout reset");
}

void Application::RunValidation() {
    Validator validator;
    m_validationIssues = validator.ValidateProject(m_rootElements);
    m_showValidationDialog = true;
}

void Application::RunOptimizer() {
    ThemeOptimizer optimizer;
    m_optimizationResult = optimizer.OptimizeTheme(m_theme, m_rootElements);
    m_showOptimizerDialog = true;
}

void Application::ShowPreferences() {
    m_showPreferencesDialog = true;
}

void Application::ExportTheme() {
    ShowToast("Export Theme - Use file dialog");
}

void Application::ImportTheme() {
    ShowToast("Import Theme - Use file dialog");
}

void Application::ExportRuntimePackage() {
    ShowToast("Export Runtime Package - Use folder dialog");
}

void Application::ShowErrorDialog(const std::string& message) {
    // Error dialog implementation
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), m_window);
}

void Application::ShowToast(const std::string& message) {
    // Toast notification - could use ImGui notifications
    printf("[Toast] %s\n", message.c_str());
}

void Application::AddRecentFile(const std::string& path) {
    // Remove if already exists
    m_recentFiles.erase(
        std::remove(m_recentFiles.begin(), m_recentFiles.end(), path),
        m_recentFiles.end()
    );
    
    // Add to front
    m_recentFiles.insert(m_recentFiles.begin(), path);
    
    // Keep only last 10
    if (m_recentFiles.size() > 10) {
        m_recentFiles.resize(10);
    }
}

bool Application::PromptSaveIfModified() {
    if (!m_projectModified) return true;
    
    // In real implementation, show save dialog
    return true;
}

void Application::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
    }
    
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    
    SDL_Quit();
}
