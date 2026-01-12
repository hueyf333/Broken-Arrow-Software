#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

namespace BrokenArrow {
namespace Viewport {

class Camera {
public:
    Camera() : position_(0, 5, -10), target_(0, 0, 0), up_(0, 1, 0),
               fov_(Math::Deg2Rad(60.0f)), aspectRatio_(16.0f / 9.0f),
               nearPlane_(0.1f), farPlane_(1000.0f) {}

    void SetPosition(const Math::Vector3& pos) { position_ = pos; UpdateViewMatrix(); }
    void SetTarget(const Math::Vector3& target) { target_ = target; UpdateViewMatrix(); }
    void SetUp(const Math::Vector3& up) { up_ = up; UpdateViewMatrix(); }

    const Math::Vector3& GetPosition() const { return position_; }
    const Math::Vector3& GetTarget() const { return target_; }
    const Math::Vector3& GetUp() const { return up_; }

    void SetFOV(float fov) { fov_ = fov; UpdateProjectionMatrix(); }
    void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; UpdateProjectionMatrix(); }
    void SetNearPlane(float nearPlane) { nearPlane_ = nearPlane; UpdateProjectionMatrix(); }
    void SetFarPlane(float farPlane) { farPlane_ = farPlane; UpdateProjectionMatrix(); }

    const Math::Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
    const Math::Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }

    void Orbit(float deltaX, float deltaY) {
        float distance = Math::Vector3::Distance(position_, target_);
        
        Math::Vector3 dir = (position_ - target_).Normalized();
        
        float azimuth = std::atan2(dir.x, dir.z);
        float elevation = std::asin(dir.y);
        
        azimuth += deltaX;
        elevation += deltaY;
        elevation = Math::Clamp(elevation, -Math::HALF_PI + 0.01f, Math::HALF_PI - 0.01f);
        
        dir.x = std::cos(elevation) * std::sin(azimuth);
        dir.y = std::sin(elevation);
        dir.z = std::cos(elevation) * std::cos(azimuth);
        
        position_ = target_ + dir * distance;
        UpdateViewMatrix();
    }

    void Pan(float deltaX, float deltaY) {
        Math::Vector3 right = (target_ - position_).Cross(up_).Normalized();
        Math::Vector3 panUp = right.Cross(target_ - position_).Normalized();
        
        Math::Vector3 offset = right * deltaX + panUp * deltaY;
        position_ += offset;
        target_ += offset;
        
        UpdateViewMatrix();
    }

    void Zoom(float delta) {
        Math::Vector3 dir = (position_ - target_).Normalized();
        float distance = Math::Vector3::Distance(position_, target_);
        
        distance -= delta;
        distance = std::max(0.1f, distance);
        
        position_ = target_ + dir * distance;
        UpdateViewMatrix();
    }

    void Frame(const Math::Vector3& center, float radius) {
        Math::Vector3 dir = (position_ - target_).Normalized();
        target_ = center;
        position_ = center + dir * (radius * 2.5f);
        UpdateViewMatrix();
    }

private:
    void UpdateViewMatrix() {
        viewMatrix_ = Math::Matrix4x4::CreateLookAt(position_, target_, up_);
    }

    void UpdateProjectionMatrix() {
        projectionMatrix_ = Math::Matrix4x4::CreatePerspective(fov_, aspectRatio_, nearPlane_, farPlane_);
    }

    Math::Vector3 position_;
    Math::Vector3 target_;
    Math::Vector3 up_;
    
    float fov_;
    float aspectRatio_;
    float nearPlane_;
    float farPlane_;
    
    Math::Matrix4x4 viewMatrix_;
    Math::Matrix4x4 projectionMatrix_;
};

} // namespace Viewport
} // namespace BrokenArrow
