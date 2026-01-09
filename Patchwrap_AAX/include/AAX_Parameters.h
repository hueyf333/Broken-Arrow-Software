#pragma once

#include <cstdint>
#include <memory>

// AAX parameter IDs
enum EPatchwrapParam {
    eParam_PluginSelector = 0,
    eParam_Bypass,
    eParam_OutputGain,
    
    // VST3 plugin parameters start here
    eParam_VST3_Start = 100,
    eParam_VST3_End = 356  // Support up to 256 VST3 parameters
};

namespace Patchwrap {

class VST3Host;
class ParameterBridge;
class StateManager;

// AAX Parameters implementation
// This would inherit from AAX_CEffectParameters in a real implementation
class AAX_Parameters {
public:
    AAX_Parameters();
    virtual ~AAX_Parameters();

    // Initialize parameters
    void Initialize();

    // Parameter accessors
    void SetBypass(bool bypass);
    bool GetBypass() const;
    
    void SetOutputGain(float gainDB);
    float GetOutputGain() const;

    // Load/unload VST3 plugin
    bool LoadPlugin(const char* path);
    void UnloadPlugin();

    // State management
    bool GetChunk(void** data, uint32_t* size);
    bool SetChunk(const void* data, uint32_t size);

    // Get instances
    VST3Host* GetVST3Host() { return vst3Host.get(); }
    ParameterBridge* GetParameterBridge() { return paramBridge.get(); }

private:
    bool bypass;
    float outputGainDB;
    
    std::unique_ptr<VST3Host> vst3Host;
    std::unique_ptr<ParameterBridge> paramBridge;
    std::unique_ptr<StateManager> stateManager;
    
    void SyncParametersToHost();
};

} // namespace Patchwrap
