#pragma once
#include "MeshImporter.h"
#include <string>

namespace BrokenArrow {
namespace IO {

class MeshExporter {
public:
    virtual ~MeshExporter() = default;
    virtual bool Export(const std::string& filename, const Mesh& mesh) = 0;
    virtual std::string GetLastError() const { return lastError_; }

protected:
    std::string lastError_;
};

} // namespace IO
} // namespace BrokenArrow
