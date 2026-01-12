#pragma once
#include "Serialization.h"
#include <string>
#include <chrono>
#include <ctime>

namespace BrokenArrow {
namespace IO {

class ProjectFile {
public:
    struct ProjectMetadata {
        std::string name;
        std::string version = "1.0";
        std::string created;
        std::string modified;
    };

    ProjectFile() {
        metadata_.version = "1.0";
        UpdateTimestamp();
    }

    bool Save(const std::string& filename) {
        Serialization::JSONWriter writer;
        
        writer.BeginObject();
        writer.WriteString("version", metadata_.version);
        writer.WriteString("name", metadata_.name);
        writer.WriteString("created", metadata_.created);
        writer.WriteString("modified", GetCurrentTimestamp());
        
        writer.BeginArray("scene");
        writer.EndArray();
        writer.EndObject();

        return writer.SaveToFile(filename);
    }

    bool Load(const std::string& filename) {
        Serialization::JSONReader reader;
        if (!reader.LoadFromFile(filename)) {
            return false;
        }

        reader.GetString("version", metadata_.version);
        reader.GetString("name", metadata_.name);
        reader.GetString("created", metadata_.created);
        reader.GetString("modified", metadata_.modified);

        return true;
    }

    void SetProjectName(const std::string& name) {
        metadata_.name = name;
    }

    const ProjectMetadata& GetMetadata() const {
        return metadata_;
    }

private:
    void UpdateTimestamp() {
        metadata_.created = GetCurrentTimestamp();
        metadata_.modified = metadata_.created;
    }

    std::string GetCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&time));
        return std::string(buffer);
    }

    ProjectMetadata metadata_;
};

} // namespace IO
} // namespace BrokenArrow
