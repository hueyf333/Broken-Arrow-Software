#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <vector>
#include <string>

namespace BrokenArrow {
namespace IO {

struct Mesh {
    std::vector<Math::Vector3> positions;
    std::vector<Math::Vector3> normals;
    std::vector<Math::Vector2> uvs;
    std::vector<int> indices;
    std::string name;

    void Clear() {
        positions.clear();
        normals.clear();
        uvs.clear();
        indices.clear();
    }

    void GenerateNormals() {
        normals.resize(positions.size(), Math::Vector3::Zero());
        std::vector<int> counts(positions.size(), 0);

        for (size_t i = 0; i < indices.size(); i += 3) {
            int i0 = indices[i];
            int i1 = indices[i + 1];
            int i2 = indices[i + 2];

            Math::Vector3 v0 = positions[i0];
            Math::Vector3 v1 = positions[i1];
            Math::Vector3 v2 = positions[i2];

            Math::Vector3 edge1 = v1 - v0;
            Math::Vector3 edge2 = v2 - v0;
            Math::Vector3 normal = edge1.Cross(edge2);

            normals[i0] += normal;
            normals[i1] += normal;
            normals[i2] += normal;
            counts[i0]++;
            counts[i1]++;
            counts[i2]++;
        }

        for (size_t i = 0; i < normals.size(); ++i) {
            if (counts[i] > 0) {
                normals[i] /= static_cast<float>(counts[i]);
                normals[i].Normalize();
            }
        }
    }
};

class MeshImporter {
public:
    virtual ~MeshImporter() = default;
    virtual bool Import(const std::string& filename, Mesh& mesh) = 0;
    virtual std::string GetLastError() const { return lastError_; }

protected:
    std::string lastError_;
};

} // namespace IO
} // namespace BrokenArrow
