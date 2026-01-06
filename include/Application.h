#pragma once

#define SKINNING_STUDIO_VERSION "1.0.0"

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <cstdint>

// Forward declarations
struct SDL_Window;
typedef void* SDL_GLContext;

namespace SkinningStudio {

class Editor;
class Canvas;
class CommandManager;
class Project;

class Application {
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

    static Application& Instance();

    Editor* GetEditor() const { return m_editor.get(); }
    Canvas* GetCanvas() const { return m_canvas.get(); }
    CommandManager* GetCommandManager() const { return m_commandManager.get(); }
    Project* GetProject() const { return m_project.get(); }

private:
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
    
    void SetupImGuiStyle();
    void SetupDocking();

    static Application* s_instance;

    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    bool m_running;

    std::unique_ptr<Editor> m_editor;
    std::unique_ptr<Canvas> m_canvas;
    std::unique_ptr<CommandManager> m_commandManager;
    std::unique_ptr<Project> m_project;

    float m_lastFrameTime;
};

} // namespace SkinningStudio
