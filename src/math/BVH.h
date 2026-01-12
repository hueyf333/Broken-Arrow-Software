#pragma once
#include "Vector3.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>

namespace BrokenArrow {
namespace Math {

struct AABB {
    Vector3 min;
    Vector3 max;

    AABB() : min(Vector3(std::numeric_limits<float>::max())), 
             max(Vector3(std::numeric_limits<float>::lowest())) {}
    
    AABB(const Vector3& min, const Vector3& max) : min(min), max(max) {}

    void Expand(const Vector3& point) {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }

    void Expand(const AABB& other) {
        min.x = std::min(min.x, other.min.x);
        min.y = std::min(min.y, other.min.y);
        min.z = std::min(min.z, other.min.z);
        max.x = std::max(max.x, other.max.x);
        max.y = std::max(max.y, other.max.y);
        max.z = std::max(max.z, other.max.z);
    }

    Vector3 Center() const {
        return (min + max) * 0.5f;
    }

    Vector3 Size() const {
        return max - min;
    }

    float SurfaceArea() const {
        Vector3 size = Size();
        return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
    }

    bool Contains(const Vector3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }

    bool Intersects(const AABB& other) const {
        return min.x <= other.max.x && max.x >= other.min.x &&
               min.y <= other.max.y && max.y >= other.min.y &&
               min.z <= other.max.z && max.z >= other.min.z;
    }

    bool IntersectRay(const Vector3& origin, const Vector3& direction, float& tMin, float& tMax) const {
        float t0 = 0.0f;
        float t1 = std::numeric_limits<float>::max();

        for (int i = 0; i < 3; ++i) {
            float invDir = 1.0f / ((float*)&direction)[i];
            float tNear = (((float*)&min)[i] - ((float*)&origin)[i]) * invDir;
            float tFar = (((float*)&max)[i] - ((float*)&origin)[i]) * invDir;

            if (tNear > tFar) std::swap(tNear, tFar);

            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar < t1 ? tFar : t1;

            if (t0 > t1) return false;
        }

        tMin = t0;
        tMax = t1;
        return true;
    }
};

struct Triangle {
    Vector3 v0, v1, v2;
    int index;

    Triangle() : index(-1) {}
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, int index)
        : v0(v0), v1(v1), v2(v2), index(index) {}

    AABB GetBounds() const {
        AABB bounds;
        bounds.Expand(v0);
        bounds.Expand(v1);
        bounds.Expand(v2);
        return bounds;
    }

    Vector3 Center() const {
        return (v0 + v1 + v2) / 3.0f;
    }

    bool IntersectRay(const Vector3& origin, const Vector3& direction, float& t, Vector3& barycentric) const {
        const float EPSILON = 1e-6f;

        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;
        Vector3 h = direction.Cross(edge2);
        float a = edge1.Dot(h);

        if (a > -EPSILON && a < EPSILON)
            return false;

        float f = 1.0f / a;
        Vector3 s = origin - v0;
        float u = f * s.Dot(h);

        if (u < 0.0f || u > 1.0f)
            return false;

        Vector3 q = s.Cross(edge1);
        float v = f * direction.Dot(q);

        if (v < 0.0f || u + v > 1.0f)
            return false;

        t = f * edge2.Dot(q);

        if (t > EPSILON) {
            barycentric = Vector3(1.0f - u - v, u, v);
            return true;
        }

        return false;
    }
};

struct BVHNode {
    AABB bounds;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<Triangle> triangles;

    bool IsLeaf() const { return left == nullptr && right == nullptr; }
};

class BVH {
public:
    BVH() = default;

    void Build(const std::vector<Triangle>& triangles, int maxTrianglesPerLeaf = 4) {
        if (triangles.empty()) {
            root_ = nullptr;
            return;
        }

        maxTrianglesPerLeaf_ = maxTrianglesPerLeaf;
        root_ = BuildRecursive(triangles);
    }

    struct HitInfo {
        bool hit = false;
        float t = std::numeric_limits<float>::max();
        int triangleIndex = -1;
        Vector3 barycentric;
        Vector3 point;
        Vector3 normal;
    };

    HitInfo IntersectRay(const Vector3& origin, const Vector3& direction) const {
        HitInfo result;
        if (!root_) return result;

        IntersectNode(root_.get(), origin, direction, result);
        return result;
    }

    const BVHNode* GetRoot() const { return root_.get(); }

private:
    std::unique_ptr<BVHNode> BuildRecursive(const std::vector<Triangle>& triangles) {
        auto node = std::make_unique<BVHNode>();

        for (const auto& tri : triangles) {
            node->bounds.Expand(tri.GetBounds());
        }

        if (triangles.size() <= static_cast<size_t>(maxTrianglesPerLeaf_)) {
            node->triangles = triangles;
            return node;
        }

        Vector3 extent = node->bounds.Size();
        int axis = 0;
        if (extent.y > extent.x) axis = 1;
        if (extent.z > ((float*)&extent)[axis]) axis = 2;

        std::vector<Triangle> sortedTriangles = triangles;
        std::sort(sortedTriangles.begin(), sortedTriangles.end(),
            [axis](const Triangle& a, const Triangle& b) {
                return ((float*)&a.Center())[axis] < ((float*)&b.Center())[axis];
            });

        size_t mid = sortedTriangles.size() / 2;
        std::vector<Triangle> leftTriangles(sortedTriangles.begin(), sortedTriangles.begin() + mid);
        std::vector<Triangle> rightTriangles(sortedTriangles.begin() + mid, sortedTriangles.end());

        node->left = BuildRecursive(leftTriangles);
        node->right = BuildRecursive(rightTriangles);

        return node;
    }

    void IntersectNode(const BVHNode* node, const Vector3& origin, const Vector3& direction, HitInfo& result) const {
        float tMin, tMax;
        if (!node->bounds.IntersectRay(origin, direction, tMin, tMax)) {
            return;
        }

        if (tMin > result.t) {
            return;
        }

        if (node->IsLeaf()) {
            for (const auto& tri : node->triangles) {
                float t;
                Vector3 barycentric;
                if (tri.IntersectRay(origin, direction, t, barycentric) && t < result.t) {
                    result.hit = true;
                    result.t = t;
                    result.triangleIndex = tri.index;
                    result.barycentric = barycentric;
                    result.point = origin + direction * t;
                    
                    Vector3 edge1 = tri.v1 - tri.v0;
                    Vector3 edge2 = tri.v2 - tri.v0;
                    result.normal = edge1.Cross(edge2).Normalized();
                }
            }
        } else {
            if (node->left) IntersectNode(node->left.get(), origin, direction, result);
            if (node->right) IntersectNode(node->right.get(), origin, direction, result);
        }
    }

    std::unique_ptr<BVHNode> root_;
    int maxTrianglesPerLeaf_ = 4;
};

} // namespace Math
} // namespace BrokenArrow
