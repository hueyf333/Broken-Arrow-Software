#include "StateManager.h"
#include <cstring>

namespace Patchwrap {

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
}

bool StateManager::serialize(const std::vector<double>& wrapperParams,
                            const std::vector<uint8_t>& pluginState,
                            const std::string& pluginPath,
                            std::vector<uint8_t>& output)
{
    // Calculate total size
    Header header;
    header.magic = 0x50545750; // 'PTWP'
    header.version = getVersion();
    header.wrapperParamCount = static_cast<uint32_t>(wrapperParams.size());
    header.wrapperParamSize = static_cast<uint32_t>(wrapperParams.size() * sizeof(double));
    header.pluginStateSize = static_cast<uint32_t>(pluginState.size());
    header.pluginPathSize = static_cast<uint32_t>(pluginPath.size());

    size_t totalSize = sizeof(Header) + 
                      header.wrapperParamSize + 
                      header.pluginStateSize + 
                      header.pluginPathSize;

    output.resize(totalSize);
    
    uint8_t* ptr = output.data();
    
    // Write header
    memcpy(ptr, &header, sizeof(Header));
    ptr += sizeof(Header);
    
    // Write wrapper parameters
    if (header.wrapperParamSize > 0) {
        memcpy(ptr, wrapperParams.data(), header.wrapperParamSize);
        ptr += header.wrapperParamSize;
    }
    
    // Write plugin state
    if (header.pluginStateSize > 0) {
        memcpy(ptr, pluginState.data(), header.pluginStateSize);
        ptr += header.pluginStateSize;
    }
    
    // Write plugin path
    if (header.pluginPathSize > 0) {
        memcpy(ptr, pluginPath.data(), header.pluginPathSize);
        ptr += header.pluginPathSize;
    }
    
    return true;
}

bool StateManager::deserialize(const std::vector<uint8_t>& input,
                               std::vector<double>& wrapperParams,
                               std::vector<uint8_t>& pluginState,
                               std::string& pluginPath)
{
    if (input.size() < sizeof(Header)) {
        return false;
    }
    
    const uint8_t* ptr = input.data();
    
    // Read header
    Header header;
    memcpy(&header, ptr, sizeof(Header));
    ptr += sizeof(Header);
    
    // Verify magic
    if (header.magic != 0x50545750) {
        return false;
    }
    
    // Verify version
    if (header.version > getVersion()) {
        return false; // Future version not supported
    }
    
    // Verify size
    size_t expectedSize = sizeof(Header) + 
                         header.wrapperParamSize + 
                         header.pluginStateSize + 
                         header.pluginPathSize;
    
    if (input.size() < expectedSize) {
        return false;
    }
    
    // Read wrapper parameters
    wrapperParams.resize(header.wrapperParamCount);
    if (header.wrapperParamSize > 0) {
        memcpy(wrapperParams.data(), ptr, header.wrapperParamSize);
        ptr += header.wrapperParamSize;
    }
    
    // Read plugin state
    pluginState.resize(header.pluginStateSize);
    if (header.pluginStateSize > 0) {
        memcpy(pluginState.data(), ptr, header.pluginStateSize);
        ptr += header.pluginStateSize;
    }
    
    // Read plugin path
    pluginPath.resize(header.pluginPathSize);
    if (header.pluginPathSize > 0) {
        memcpy(pluginPath.data(), ptr, header.pluginPathSize);
        ptr += header.pluginPathSize;
    }
    
    return true;
}

} // namespace Patchwrap
