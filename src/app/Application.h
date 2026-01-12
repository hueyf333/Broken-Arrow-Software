#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <string>
#include <memory>
#include "Viewport.h"
#include "HairDescription.h"
#include "ProjectFile.h"
#include "OBJImporter.h"
#include "OBJExporter.h"

namespace BrokenArrow {
namespace App {

class Application {
public:
    static Application& Instance();

    bool Initialize();
    void Run();
    void Shutdown();

    Viewport::Viewport& GetViewport() { return *viewport_; }
    Hair::HairDescription* GetCurrentHairDescription() { return currentHairDescription_.get(); }

    bool NewProject();
    bool OpenProject(const std::string& filename);
    bool SaveProject(const std::string& filename);

    bool ImportMesh(const std::string& filename);
    bool ExportMesh(const std::string& filename);

    void CreateHairDescription(const std::string& name);

private:
    Application();
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    std::unique_ptr<Viewport::Viewport> viewport_;
    std::unique_ptr<Hair::HairDescription> currentHairDescription_;
    std::unique_ptr<IO::ProjectFile> currentProject_;
    IO::Mesh scalpMesh_;
    HWND mainWindowHandle_;
};

} // namespace App
} // namespace BrokenArrow

#endif // _WIN32
