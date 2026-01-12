#pragma once
#include "DX12Renderer.h"
#include "Camera.h"
#include "HairDescription.h"
#include "MeshImporter.h"
#include <memory>

namespace BrokenArrow {
namespace Viewport {

class Viewport {
public:
    Viewport();
    ~Viewport();

    bool Initialize(void* windowHandle, int width, int height);
    void Shutdown();

    void Render();
    void Resize(int width, int height);

    Camera& GetCamera() { return camera_; }
    const Camera& GetCamera() const { return camera_; }

    void SetMesh(const IO::Mesh& mesh);
    void SetHairDescription(Hair::HairDescription* hairDesc);

    void SetRenderMode(int mode) { renderMode_ = mode; }
    int GetRenderMode() const { return renderMode_; }

private:
#ifdef _WIN32
    std::unique_ptr<DX12Renderer> renderer_;
#endif
    Camera camera_;
    IO::Mesh mesh_;
    Hair::HairDescription* hairDescription_;
    int renderMode_;
};

} // namespace Viewport
} // namespace BrokenArrow
