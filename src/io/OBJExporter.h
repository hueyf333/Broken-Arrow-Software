#pragma once
#include "MeshExporter.h"
#include <fstream>
#include <iomanip>

namespace BrokenArrow {
namespace IO {

class OBJExporter : public MeshExporter {
public:
    bool Export(const std::string& filename, const Mesh& mesh) override {
        std::ofstream file(filename);
        if (!file.is_open()) {
            lastError_ = "Failed to create file: " + filename;
            return false;
        }

        file << std::fixed << std::setprecision(6);

        file << "# Broken Arrow Hair Studio OBJ Export\n";
        file << "# Vertices: " << mesh.positions.size() << "\n";
        file << "# Faces: " << (mesh.indices.size() / 3) << "\n\n";

        if (!mesh.name.empty()) {
            file << "o " << mesh.name << "\n\n";
        }

        for (const auto& pos : mesh.positions) {
            file << "v " << pos.x << " " << pos.y << " " << pos.z << "\n";
        }

        if (!mesh.uvs.empty()) {
            file << "\n";
            for (const auto& uv : mesh.uvs) {
                file << "vt " << uv.x << " " << uv.y << "\n";
            }
        }

        if (!mesh.normals.empty()) {
            file << "\n";
            for (const auto& normal : mesh.normals) {
                file << "vn " << normal.x << " " << normal.y << " " << normal.z << "\n";
            }
        }

        file << "\n";
        bool hasUVs = !mesh.uvs.empty();
        bool hasNormals = !mesh.normals.empty();

        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            file << "f";
            for (int j = 0; j < 3; ++j) {
                int idx = mesh.indices[i + j] + 1;
                file << " " << idx;
                if (hasUVs || hasNormals) {
                    file << "/";
                    if (hasUVs) file << idx;
                    if (hasNormals) {
                        file << "/" << idx;
                    }
                }
            }
            file << "\n";
        }

        file.close();
        return true;
    }
};

} // namespace IO
} // namespace BrokenArrow
