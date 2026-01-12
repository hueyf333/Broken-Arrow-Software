#pragma once
#include "MeshImporter.h"
#include <sstream>
#include <fstream>

namespace BrokenArrow {
namespace IO {

class OBJImporter : public MeshImporter {
public:
    bool Import(const std::string& filename, Mesh& mesh) override {
        std::ifstream file(filename);
        if (!file.is_open()) {
            lastError_ = "Failed to open file: " + filename;
            return false;
        }

        mesh.Clear();
        std::string line;
        std::vector<Math::Vector3> tempPositions;
        std::vector<Math::Vector2> tempUVs;
        std::vector<Math::Vector3> tempNormals;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                Math::Vector3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                tempPositions.push_back(pos);
            }
            else if (prefix == "vt") {
                Math::Vector2 uv;
                iss >> uv.x >> uv.y;
                tempUVs.push_back(uv);
            }
            else if (prefix == "vn") {
                Math::Vector3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                tempNormals.push_back(normal);
            }
            else if (prefix == "f") {
                ParseFace(iss, mesh, tempPositions, tempUVs, tempNormals);
            }
            else if (prefix == "o" || prefix == "g") {
                iss >> mesh.name;
            }
        }

        if (mesh.normals.empty() && !mesh.positions.empty()) {
            mesh.GenerateNormals();
        }

        return true;
    }

private:
    void ParseFace(std::istringstream& iss, Mesh& mesh,
                   const std::vector<Math::Vector3>& tempPositions,
                   const std::vector<Math::Vector2>& tempUVs,
                   const std::vector<Math::Vector3>& tempNormals) {
        
        std::vector<int> faceIndices;
        std::string vertex;

        while (iss >> vertex) {
            int posIdx = -1, uvIdx = -1, normalIdx = -1;
            ParseVertexString(vertex, posIdx, uvIdx, normalIdx);

            int vertexIndex = FindOrCreateVertex(mesh, tempPositions, tempUVs, tempNormals,
                                                  posIdx, uvIdx, normalIdx);
            faceIndices.push_back(vertexIndex);
        }

        if (faceIndices.size() >= 3) {
            for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                mesh.indices.push_back(faceIndices[0]);
                mesh.indices.push_back(faceIndices[i]);
                mesh.indices.push_back(faceIndices[i + 1]);
            }
        }
    }

    void ParseVertexString(const std::string& vertex, int& posIdx, int& uvIdx, int& normalIdx) {
        size_t slash1 = vertex.find('/');
        if (slash1 == std::string::npos) {
            posIdx = std::stoi(vertex) - 1;
            return;
        }

        posIdx = std::stoi(vertex.substr(0, slash1)) - 1;

        size_t slash2 = vertex.find('/', slash1 + 1);
        if (slash2 == std::string::npos) {
            if (slash1 + 1 < vertex.length()) {
                uvIdx = std::stoi(vertex.substr(slash1 + 1)) - 1;
            }
            return;
        }

        if (slash2 > slash1 + 1) {
            uvIdx = std::stoi(vertex.substr(slash1 + 1, slash2 - slash1 - 1)) - 1;
        }

        if (slash2 + 1 < vertex.length()) {
            normalIdx = std::stoi(vertex.substr(slash2 + 1)) - 1;
        }
    }

    int FindOrCreateVertex(Mesh& mesh,
                           const std::vector<Math::Vector3>& tempPositions,
                           const std::vector<Math::Vector2>& tempUVs,
                           const std::vector<Math::Vector3>& tempNormals,
                           int posIdx, int uvIdx, int normalIdx) {
        
        if (posIdx >= 0 && posIdx < static_cast<int>(tempPositions.size())) {
            mesh.positions.push_back(tempPositions[posIdx]);
        } else {
            mesh.positions.push_back(Math::Vector3::Zero());
        }

        if (uvIdx >= 0 && uvIdx < static_cast<int>(tempUVs.size())) {
            if (mesh.uvs.size() < mesh.positions.size()) {
                mesh.uvs.resize(mesh.positions.size() - 1, Math::Vector2(0, 0));
            }
            mesh.uvs.push_back(tempUVs[uvIdx]);
        } else if (!tempUVs.empty()) {
            mesh.uvs.push_back(Math::Vector2(0, 0));
        }

        if (normalIdx >= 0 && normalIdx < static_cast<int>(tempNormals.size())) {
            if (mesh.normals.size() < mesh.positions.size()) {
                mesh.normals.resize(mesh.positions.size() - 1, Math::Vector3::UnitY());
            }
            mesh.normals.push_back(tempNormals[normalIdx]);
        } else if (!tempNormals.empty()) {
            mesh.normals.push_back(Math::Vector3::UnitY());
        }

        return static_cast<int>(mesh.positions.size()) - 1;
    }
};

} // namespace IO
} // namespace BrokenArrow
