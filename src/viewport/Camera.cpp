#include "Camera.h"

namespace BrokenArrow {
namespace Viewport {

// Constructor initializes view and projection matrices
Camera::Camera() : position_(0, 5, -10), target_(0, 0, 0), up_(0, 1, 0),
                   fov_(Math::Deg2Rad(60.0f)), aspectRatio_(16.0f / 9.0f),
                   nearPlane_(0.1f), farPlane_(1000.0f) {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

} // namespace Viewport
} // namespace BrokenArrow
