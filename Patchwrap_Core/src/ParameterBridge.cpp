#include "ParameterBridge.h"
#include <algorithm>

namespace Patchwrap {

ParameterBridge::ParameterBridge()
    : numParameters(0)
{
}

ParameterBridge::~ParameterBridge()
{
}

void ParameterBridge::initialize(uint32_t numParams)
{
    numParameters = numParams;
    currentValues.resize(numParams, 0.0);
}

void ParameterBridge::setParameterFromHost(uint32_t index, double normalizedValue)
{
    if (index >= numParameters) return;
    
    // Clamp to valid range
    normalizedValue = std::max(0.0, std::min(1.0, normalizedValue));
    
    // Update current value
    currentValues[index] = normalizedValue;
    
    // Queue update for plugin
    ParameterUpdate update;
    update.parameterIndex = index;
    update.normalizedValue = normalizedValue;
    update.fromHost = true;
    
    toPluginQueue.push(update);
}

void ParameterBridge::setParameterFromPlugin(uint32_t index, double normalizedValue)
{
    if (index >= numParameters) return;
    
    // Clamp to valid range
    normalizedValue = std::max(0.0, std::min(1.0, normalizedValue));
    
    // Update current value
    currentValues[index] = normalizedValue;
    
    // Queue update for host
    ParameterUpdate update;
    update.parameterIndex = index;
    update.normalizedValue = normalizedValue;
    update.fromHost = false;
    
    toHostQueue.push(update);
}

double ParameterBridge::getParameterForHost(uint32_t index) const
{
    if (index >= numParameters) return 0.0;
    return currentValues[index];
}

double ParameterBridge::getParameterForPlugin(uint32_t index) const
{
    if (index >= numParameters) return 0.0;
    return currentValues[index];
}

void ParameterBridge::processUpdates()
{
    // This method can be used to batch process updates if needed
    // For now, the queues handle real-time communication directly
}

bool ParameterBridge::getUpdateForPlugin(ParameterUpdate& update)
{
    return toPluginQueue.pop(update);
}

bool ParameterBridge::getUpdateForHost(ParameterUpdate& update)
{
    return toHostQueue.pop(update);
}

} // namespace Patchwrap
