#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace Patchwrap {

// State serialization for both wrapper and hosted plugin
class StateManager {
public:
    StateManager();
    ~StateManager();

    // Serialize complete state (wrapper params + VST3 plugin state)
    bool serialize(const std::vector<double>& wrapperParams,
                  const std::vector<uint8_t>& pluginState,
                  const std::string& pluginPath,
                  std::vector<uint8_t>& output);

    // Deserialize complete state
    bool deserialize(const std::vector<uint8_t>& input,
                    std::vector<double>& wrapperParams,
                    std::vector<uint8_t>& pluginState,
                    std::string& pluginPath);

    // Get version for compatibility checking
    static uint32_t getVersion() { return 1; }

private:
    struct Header {
        uint32_t magic;        // 'PTWP' = Patchwrap
        uint32_t version;
        uint32_t wrapperParamCount;
        uint32_t wrapperParamSize;
        uint32_t pluginStateSize;
        uint32_t pluginPathSize;
    };
};

} // namespace Patchwrap
