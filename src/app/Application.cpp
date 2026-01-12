#include "Application.h"

#ifdef _WIN32

namespace BrokenArrow {
namespace App {

Application& Application::Instance() {
    static Application instance;
    return instance;
}

Application::Application() : mainWindowHandle_(nullptr) {
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    viewport_ = std::make_unique<Viewport::Viewport>();
    currentProject_ = std::make_unique<IO::ProjectFile>();
    
    return true;
}

void Application::Run() {
}

void Application::Shutdown() {
    if (viewport_) {
        viewport_->Shutdown();
    }
}

bool Application::NewProject() {
    currentProject_ = std::make_unique<IO::ProjectFile>();
    currentHairDescription_.reset();
    scalpMesh_.Clear();
    return true;
}

bool Application::OpenProject(const std::string& filename) {
    if (!currentProject_->Load(filename)) {
        return false;
    }
    return true;
}

bool Application::SaveProject(const std::string& filename) {
    return currentProject_->Save(filename);
}

bool Application::ImportMesh(const std::string& filename) {
    IO::OBJImporter importer;
    if (!importer.Import(filename, scalpMesh_)) {
        return false;
    }

    if (viewport_) {
        viewport_->SetMesh(scalpMesh_);
    }

    return true;
}

bool Application::ExportMesh(const std::string& filename) {
    IO::OBJExporter exporter;
    return exporter.Export(filename, scalpMesh_);
}

void Application::CreateHairDescription(const std::string& name) {
    currentHairDescription_ = std::make_unique<Hair::HairDescription>();
    currentHairDescription_->SetName(name);

    if (viewport_) {
        viewport_->SetHairDescription(currentHairDescription_.get());
    }
}

} // namespace App
} // namespace BrokenArrow

#endif // _WIN32
