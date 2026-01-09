# Patchwrap Quick Reference

Quick reference for common tasks and code patterns.

## Building

```bash
# Setup (first time only)
cd scripts
.\fetch_vst3sdk.ps1

# Set AAX SDK path (Windows)
set AAX_SDK_PATH=C:\SDKs\AAX_SDK

# Build Debug
msbuild Patchwrap.sln /p:Configuration=Debug /p:Platform=x64

# Build Release
msbuild Patchwrap.sln /p:Configuration=Release /p:Platform=x64
```

## Project Structure

```
Patchwrap_Core/         → Static library (.lib)
├── include/            → Public headers
└── src/                → Implementation

Patchwrap_AAX/          → AAX plugin (.aaxplugin)
├── include/            → AAX headers
└── src/                → Implementation

Build/Release/          → Output directory
└── Patchwrap.aaxplugin → Final bundle
```

## Key Classes

### VST3Host (Core)
```cpp
VST3Host host;
host.loadPlugin("C:\\...\\MyPlugin.vst3");
host.setupProcessing(48000.0, 512, true);
host.setProcessing(true);
host.processAudio(input, output, midiEvents);
```

### ParameterBridge (Core)
```cpp
ParameterBridge bridge;
bridge.initialize(256);

// UI thread
bridge.setParameterFromHost(paramID, value);

// Audio thread
ParameterUpdate update;
while (bridge.getUpdateForPlugin(update)) {
    // Apply update
}
```

### StateManager (Core)
```cpp
StateManager mgr;

// Save
std::vector<uint8_t> data;
mgr.serialize(wrapperParams, pluginState, pluginPath, data);

// Load
mgr.deserialize(data, wrapperParams, pluginState, pluginPath);
```

### AAX_Parameters (AAX)
```cpp
AAX_Parameters params;
params.Initialize();
params.LoadPlugin("C:\\...\\plugin.vst3");
params.GetChunk(&data, &size);
params.SetChunk(data, size);
```

## Real-Time Safety Rules

✅ **Allowed in Audio Thread**
```cpp
// Stack allocation
float buffer[512];

// Lock-free operations
queue.push(value);
atomic.load();

// Simple math
float gain = powf(10.0f, gainDB / 20.0f);

// Pointer dereference
*buffer = value;
```

❌ **Never in Audio Thread**
```cpp
// NO heap allocation
new Object();
vector.push_back(item);

// NO locks
mutex.lock();
critical_section.enter();

// NO I/O
fopen(), printf(), std::cout

// NO blocking calls
Sleep(), WaitForSingleObject()
```

## Parameter Update Pattern

### From Host to Plugin
```cpp
// UI Thread (AAX_Parameters)
void SetParameter(int id, double value) {
    paramBridge->setParameterFromHost(id, value);
}

// Audio Thread (AAX_Algorithm)
ParameterUpdate update;
while (paramBridge->getUpdateForPlugin(update)) {
    vst3Host->setParameterValue(
        update.parameterIndex,
        update.normalizedValue);
}
```

### From Plugin to Host
```cpp
// Audio Thread (VST3Host)
void OnPluginParameterChange(int id, double value) {
    paramBridge->setParameterFromPlugin(id, value);
}

// UI Thread (AAX_Parameters)
ParameterUpdate update;
while (paramBridge->getUpdateForHost(update)) {
    // Notify AAX
    TouchParameter(update.parameterIndex);
    SetParameter(update.parameterIndex,
                 update.normalizedValue);
    ReleaseParameter(update.parameterIndex);
}
```

## Audio Processing Flow

```cpp
void AAX_AlgorithmProcessCallback(
    AAX_AlgorithmContext* const inInstancesBegin[],
    const void* inInstancesEnd)
{
    auto* ctx = inInstancesBegin[0];
    
    // 1. Check bypass
    if (ctx->bypass) {
        // Copy input to output
        return;
    }
    
    // 2. Get parameter updates
    ParameterUpdate update;
    while (ctx->paramBridge->getUpdateForPlugin(update)) {
        ctx->vst3Host->setParameterValue(
            update.parameterIndex,
            update.normalizedValue);
    }
    
    // 3. Process audio
    AudioBuffer input = { ctx->inputBuffers,
                         ctx->numInputChannels,
                         ctx->numFrames };
    AudioBuffer output = { ctx->outputBuffers,
                          ctx->numOutputChannels,
                          ctx->numFrames };
    
    ctx->vst3Host->processAudio(input, output, midiEvents);
    
    // 4. Apply output gain
    float linearGain = powf(10.0f, ctx->outputGain / 20.0f);
    for (int ch = 0; ch < ctx->numOutputChannels; ++ch) {
        for (int i = 0; i < ctx->numFrames; ++i) {
            ctx->outputBuffers[ch][i] *= linearGain;
        }
    }
}
```

## State Serialization

```cpp
// Save state
void SaveState() {
    std::vector<double> wrapperParams;
    wrapperParams.push_back(bypass ? 1.0 : 0.0);
    wrapperParams.push_back(outputGainDB);
    
    std::vector<uint8_t> pluginState;
    vst3Host->saveState(pluginState);
    
    std::string pluginPath = vst3Host->getPluginInfo().path;
    
    std::vector<uint8_t> chunkData;
    stateManager->serialize(wrapperParams,
                           pluginState,
                           pluginPath,
                           chunkData);
}

// Load state
void LoadState(const std::vector<uint8_t>& chunkData) {
    std::vector<double> wrapperParams;
    std::vector<uint8_t> pluginState;
    std::string pluginPath;
    
    stateManager->deserialize(chunkData,
                             wrapperParams,
                             pluginState,
                             pluginPath);
    
    if (!pluginPath.empty()) {
        vst3Host->loadPlugin(pluginPath);
        vst3Host->loadState(pluginState);
    }
    
    bypass = (wrapperParams[0] > 0.5);
    outputGainDB = wrapperParams[1];
}
```

## Common Patterns

### RAII Resource Management
```cpp
class MyClass {
public:
    MyClass() : resource(new Resource()) {}
    ~MyClass() { /* automatic cleanup */ }
    
private:
    std::unique_ptr<Resource> resource;
};
```

### Lock-Free Queue Usage
```cpp
// Producer thread
if (!queue.push(item)) {
    // Queue full, drop or handle
}

// Consumer thread
Item item;
if (queue.pop(item)) {
    // Process item
}
```

### Error Handling

```cpp
// UI Thread - can throw/show errors
bool LoadPlugin(const char* path) {
    if (!path) {
        ShowError("Invalid path");
        return false;
    }
    
    if (!vst3Host->loadPlugin(path)) {
        ShowError("Failed to load plugin");
        return false;
    }
    
    return true;
}

// Audio Thread - silent failure
void ProcessAudio(...) {
    if (!ctx || !ctx->vst3Host) {
        // Silent bypass
        return;
    }
    
    // Process...
}
```

## Debugging

```cpp
// Debug-only logging (removed in Release)
#ifdef _DEBUG
    #define LOG(msg) OutputDebugStringA(msg)
#else
    #define LOG(msg) ((void)0)
#endif

// Never in audio thread!
LOG("Loading plugin\n");
```

## Compiler Flags

```
/W3          Warning level 3
/std:c++17   C++17 standard
/MT          Static runtime (Release)
/MTd         Static runtime (Debug)
/O2          Optimize for speed (Release)
```

## Installation

```powershell
# Copy to Pro Tools plugins folder
xcopy /E /I "Build\Release\Patchwrap.aaxplugin" ^
  "C:\Program Files\Common Files\Avid\Audio\Plug-Ins\Patchwrap.aaxplugin"
```

## Useful Links

- [BUILDING.md](BUILDING.md) - Full build instructions
- [ARCHITECTURE.md](ARCHITECTURE.md) - Design details
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guide
- [FAQ.md](FAQ.md) - Common questions
- [AAX SDK Docs](https://developer.avid.com/)
- [VST3 SDK Docs](https://steinbergmedia.github.io/vst3_doc/)
