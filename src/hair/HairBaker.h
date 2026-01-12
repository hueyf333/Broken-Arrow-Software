#pragma once
#include "HairStrand.h"
#include "MeshImporter.h"
#include "MeshExporter.h"
#include <vector>

namespace BrokenArrow {
namespace Hair {

enum class BakeMode {
    HairCards,
    StrandMesh
};

struct BakeParams {
    BakeMode mode = BakeMode::HairCards;
    int cardsPerClump = 2;
    float cardWidth = 0.5f;
    int cardSegments = 4;
    int tubeSides = 8;
    float tubeRadius = 0.01f;
};

class HairBaker {
public:
    HairBaker() = default;

    IO::Mesh BakeToMesh(const std::vector<HairStrand>& strands, const BakeParams& params) {
        if (params.mode == BakeMode::HairCards) {
            return BakeHairCards(strands, params);
        } else {
            return BakeStrandMesh(strands, params);
        }
    }

private:
    IO::Mesh BakeHairCards(const std::vector<HairStrand>& strands, const BakeParams& params) {
        IO::Mesh mesh;
        mesh.name = "HairCards";

        int clumpSize = std::max(1, static_cast<int>(strands.size()) / 50);

        for (size_t i = 0; i < strands.size(); i += clumpSize) {
            size_t clumpEnd = std::min(i + clumpSize, strands.size());
            
            for (int card = 0; card < params.cardsPerClump && i + card < clumpEnd; ++card) {
                const HairStrand& strand = strands[i + card];
                if (strand.GetPointCount() < 2) continue;

                CreateCard(mesh, strand, params);
            }
        }

        mesh.GenerateNormals();
        return mesh;
    }

    void CreateCard(IO::Mesh& mesh, const HairStrand& strand, const BakeParams& params) {
        int baseVertex = static_cast<int>(mesh.positions.size());
        float halfWidth = params.cardWidth * 0.5f;

        for (size_t i = 0; i < strand.GetPointCount(); ++i) {
            Math::Vector3 point = strand.GetPoint(i);
            float t = static_cast<float>(i) / static_cast<float>(strand.GetPointCount() - 1);

            Math::Vector3 tangent = Math::Vector3::UnitY();
            if (i < strand.GetPointCount() - 1) {
                tangent = (strand.GetPoint(i + 1) - point).Normalized();
            }

            Math::Vector3 right = tangent.Cross(Math::Vector3::UnitZ()).Normalized();
            if (right.Length() < 0.1f) {
                right = tangent.Cross(Math::Vector3::UnitX()).Normalized();
            }

            mesh.positions.push_back(point - right * halfWidth);
            mesh.positions.push_back(point + right * halfWidth);

            mesh.uvs.push_back(Math::Vector2(0.0f, t));
            mesh.uvs.push_back(Math::Vector2(1.0f, t));
        }

        for (size_t i = 0; i < strand.GetPointCount() - 1; ++i) {
            int v0 = baseVertex + static_cast<int>(i * 2);
            int v1 = v0 + 1;
            int v2 = v0 + 2;
            int v3 = v0 + 3;

            mesh.indices.push_back(v0);
            mesh.indices.push_back(v2);
            mesh.indices.push_back(v1);

            mesh.indices.push_back(v1);
            mesh.indices.push_back(v2);
            mesh.indices.push_back(v3);
        }
    }

    IO::Mesh BakeStrandMesh(const std::vector<HairStrand>& strands, const BakeParams& params) {
        IO::Mesh mesh;
        mesh.name = "HairStrands";

        for (const auto& strand : strands) {
            if (strand.GetPointCount() < 2) continue;
            CreateTube(mesh, strand, params);
        }

        mesh.GenerateNormals();
        return mesh;
    }

    void CreateTube(IO::Mesh& mesh, const HairStrand& strand, const BakeParams& params) {
        int baseVertex = static_cast<int>(mesh.positions.size());
        int sides = params.tubeSides;
        float radius = params.tubeRadius;

        for (size_t i = 0; i < strand.GetPointCount(); ++i) {
            Math::Vector3 point = strand.GetPoint(i);
            
            Math::Vector3 tangent = Math::Vector3::UnitY();
            if (i < strand.GetPointCount() - 1) {
                tangent = (strand.GetPoint(i + 1) - point).Normalized();
            }

            Math::Vector3 right = tangent.Cross(Math::Vector3::UnitZ()).Normalized();
            if (right.Length() < 0.1f) {
                right = tangent.Cross(Math::Vector3::UnitX()).Normalized();
            }
            Math::Vector3 up = tangent.Cross(right).Normalized();

            for (int s = 0; s < sides; ++s) {
                float angle = (static_cast<float>(s) / static_cast<float>(sides)) * Math::TWO_PI;
                Math::Vector3 offset = (right * std::cos(angle) + up * std::sin(angle)) * radius;
                mesh.positions.push_back(point + offset);
                
                float u = static_cast<float>(s) / static_cast<float>(sides);
                float v = static_cast<float>(i) / static_cast<float>(strand.GetPointCount() - 1);
                mesh.uvs.push_back(Math::Vector2(u, v));
            }
        }

        for (size_t i = 0; i < strand.GetPointCount() - 1; ++i) {
            for (int s = 0; s < sides; ++s) {
                int nextS = (s + 1) % sides;
                
                int v0 = baseVertex + static_cast<int>(i * sides + s);
                int v1 = baseVertex + static_cast<int>(i * sides + nextS);
                int v2 = baseVertex + static_cast<int>((i + 1) * sides + s);
                int v3 = baseVertex + static_cast<int>((i + 1) * sides + nextS);

                mesh.indices.push_back(v0);
                mesh.indices.push_back(v2);
                mesh.indices.push_back(v1);

                mesh.indices.push_back(v1);
                mesh.indices.push_back(v2);
                mesh.indices.push_back(v3);
            }
        }
    }
};

} // namespace Hair
} // namespace BrokenArrow
