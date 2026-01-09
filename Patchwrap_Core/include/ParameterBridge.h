#pragma once

#include "LockFreeQueue.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>

namespace Patchwrap {

// Parameter update message for lock-free communication
struct ParameterUpdate {
    uint32_t parameterIndex;
    double normalizedValue;
    bool fromHost; // true = from AAX host, false = from VST3 plugin
};

// Bidirectional parameter bridge between AAX and VST3
class ParameterBridge {
public:
    ParameterBridge();
    ~ParameterBridge();

    // Initialize with number of parameters
    void initialize(uint32_t numParameters);

    // Set parameter from AAX host (non-realtime safe)
    void setParameterFromHost(uint32_t index, double normalizedValue);

    // Set parameter from VST3 plugin (realtime safe)
    void setParameterFromPlugin(uint32_t index, double normalizedValue);

    // Get parameter value for AAX host (realtime safe)
    double getParameterForHost(uint32_t index) const;

    // Get parameter value for VST3 plugin (realtime safe)
    double getParameterForPlugin(uint32_t index) const;

    // Process queued parameter updates (call from audio thread)
    void processUpdates();

    // Get pending updates for VST3 plugin
    bool getUpdateForPlugin(ParameterUpdate& update);

    // Get pending updates for AAX host
    bool getUpdateForHost(ParameterUpdate& update);

    uint32_t getNumParameters() const { return numParameters; }

private:
    uint32_t numParameters;
    std::vector<double> currentValues; // Current parameter values
    
    // Lock-free queues for bidirectional communication
    LockFreeQueue<ParameterUpdate, 2048> toPluginQueue;
    LockFreeQueue<ParameterUpdate, 2048> toHostQueue;
};

} // namespace Patchwrap
