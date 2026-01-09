#include "AAX_Parameters.h"
#include "VST3Host.h"
#include "ParameterBridge.h"
#include "StateManager.h"
#include <cmath>

namespace Patchwrap {

AAX_Parameters::AAX_Parameters()
    : bypass(false)
    , outputGainDB(0.0f)
    , vst3Host(new VST3Host())
    , paramBridge(new ParameterBridge())
    , stateManager(new StateManager())
{
}

AAX_Parameters::~AAX_Parameters()
{
}

void AAX_Parameters::Initialize()
{
    // Initialize parameter bridge
    paramBridge->initialize(256); // Support up to 256 VST3 parameters
    
    // Connect bridge to VST3 host
    vst3Host->setParameterBridge(paramBridge.get());
    
    // In a real AAX implementation, we would:
    // 1. Call EffectInit()
    // 2. Register parameters using AddParameter()
    // 3. Set up parameter tapers, units, strings, etc.
}

void AAX_Parameters::SetBypass(bool bypass)
{
    this->bypass = bypass;
}

bool AAX_Parameters::GetBypass() const
{
    return bypass;
}

void AAX_Parameters::SetOutputGain(float gainDB)
{
    outputGainDB = gainDB;
}

float AAX_Parameters::GetOutputGain() const
{
    return outputGainDB;
}

bool AAX_Parameters::LoadPlugin(const char* path)
{
    if (!path) return false;
    
    // Load VST3 plugin
    if (!vst3Host->loadPlugin(path)) {
        return false;
    }
    
    // Setup processing (will be configured properly when AAX tells us sample rate)
    vst3Host->setupProcessing(44100.0, 512, true);
    
    return true;
}

void AAX_Parameters::UnloadPlugin()
{
    vst3Host->unloadPlugin();
}

bool AAX_Parameters::GetChunk(void** data, uint32_t* size)
{
    if (!data || !size) return false;
    
    // Collect wrapper parameters
    std::vector<double> wrapperParams;
    wrapperParams.push_back(bypass ? 1.0 : 0.0);
    wrapperParams.push_back(outputGainDB);
    
    // Get VST3 plugin state
    std::vector<uint8_t> pluginState;
    std::string pluginPath;
    
    if (vst3Host->isPluginLoaded()) {
        vst3Host->saveState(pluginState);
        pluginPath = vst3Host->getPluginInfo().path;
    }
    
    // Serialize everything
    static std::vector<uint8_t> chunkData;
    if (!stateManager->serialize(wrapperParams, pluginState, pluginPath, chunkData)) {
        return false;
    }
    
    *data = chunkData.data();
    *size = static_cast<uint32_t>(chunkData.size());
    
    return true;
}

bool AAX_Parameters::SetChunk(const void* data, uint32_t size)
{
    if (!data || size == 0) return false;
    
    std::vector<uint8_t> input(static_cast<const uint8_t*>(data),
                               static_cast<const uint8_t*>(data) + size);
    
    std::vector<double> wrapperParams;
    std::vector<uint8_t> pluginState;
    std::string pluginPath;
    
    if (!stateManager->deserialize(input, wrapperParams, pluginState, pluginPath)) {
        return false;
    }
    
    // Restore wrapper parameters
    if (wrapperParams.size() >= 2) {
        bypass = (wrapperParams[0] > 0.5);
        outputGainDB = static_cast<float>(wrapperParams[1]);
    }
    
    // Load plugin if path is valid
    if (!pluginPath.empty()) {
        if (LoadPlugin(pluginPath.c_str())) {
            // Restore plugin state
            if (!pluginState.empty()) {
                vst3Host->loadState(pluginState);
            }
        }
    }
    
    return true;
}

void AAX_Parameters::SyncParametersToHost()
{
    // In a real implementation, this would notify AAX of parameter changes
    // using TouchParameter() and ReleaseParameter()
}

} // namespace Patchwrap
