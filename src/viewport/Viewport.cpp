#include "Viewport.h"

namespace BrokenArrow {
namespace Viewport {

Viewport::Viewport() : hairDescription_(nullptr), renderMode_(0) {
#ifdef _WIN32
    renderer_ = std::make_unique<DX12Renderer>();
#endif
}

Viewport::~Viewport() {
    Shutdown();
}

bool Viewport::Initialize(void* windowHandle, int width, int height) {
#ifdef _WIN32
    if (renderer_) {
        return renderer_->Initialize(windowHandle, width, height);
    }
#endif
    return false;
}

void Viewport::Shutdown() {
#ifdef _WIN32
    if (renderer_) {
        renderer_->Shutdown();
    }
#endif
}

void Viewport::Render() {
#ifdef _WIN32
    if (!renderer_) return;

    renderer_->SetViewProjection(camera_.GetViewMatrix(), camera_.GetProjectionMatrix());
    
    renderer_->BeginFrame();

    if (!mesh_.positions.empty()) {
        renderer_->RenderMesh(mesh_, Math::Matrix4x4::CreateIdentity());
    }

    if (hairDescription_) {
        if (renderMode_ == 0 || renderMode_ == 1) {
            renderer_->RenderGuides(hairDescription_->GetGuides());
        }
        
        if (renderMode_ == 1 || renderMode_ == 2) {
            renderer_->RenderStrands(hairDescription_->GetStrands());
        }
    }

    renderer_->EndFrame();
    renderer_->Present();
#endif
}

void Viewport::Resize(int width, int height) {
    camera_.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    
#ifdef _WIN32
    if (renderer_) {
        renderer_->Resize(width, height);
    }
#endif
}

void Viewport::SetMesh(const IO::Mesh& mesh) {
    mesh_ = mesh;
}

void Viewport::SetHairDescription(Hair::HairDescription* hairDesc) {
    hairDescription_ = hairDesc;
}

} // namespace Viewport
} // namespace BrokenArrow
