# Patchwrap Architecture

This document provides a detailed technical overview of the Patchwrap AAX wrapper architecture.

## System Overview

Patchwrap is designed as a two-layer system:

1. **Patchwrap_Core**: Platform-independent VST3 hosting library
2. **Patchwrap_AAX**: AAX-specific wrapper that uses the core library

This separation allows for future expansion (e.g., AU, VST3 wrapper versions) while reusing the core VST3 hosting logic.

## Design Principles

### Real-Time Safety

The audio processing path must be real-time safe:

- **No allocations**: All memory pre-allocated during setup
- **No locks**: Lock-free data structures for cross-thread communication
- **No I/O**: File operations only during init/load/save
- **No blocking**: No system calls that might block

### Thread Architecture

Three primary threads:

1. **UI Thread**: All GUI operations, AAX parameter updates, plugin loading
2. **Audio Thread**: Real-time audio processing via AAX algorithm callback
3. **VST3 Processing Thread**: May be same as audio thread or separate (VST3 SDK manages)

Communication between threads uses lock-free queues.

### Parameter Flow

Parameters flow bidirectionally between AAX host and VST3 plugin:

```
AAX Host ↔ AAX_Parameters ↔ ParameterBridge ↔ VST3Host ↔ VST3 Plugin
```

The ParameterBridge uses lock-free queues to ensure real-time safe communication.

## Core Components

### VST3Host

**Responsibility**: Manage VST3 plugin lifecycle and processing

**Key Operations**:
- `scanPlugins()`: Enumerate VST3 plugins in standard locations
- `loadPlugin()`: Load VST3 module, create component and controller
- `setupProcessing()`: Configure sample rate, buffer size, bus arrangement
- `processAudio()`: Forward audio/MIDI to VST3 plugin (real-time safe)
- `setParameterValue()` / `getParameterValue()`: Parameter access
- `saveState()` / `loadState()`: State serialization
- `createEditor()`: Embed VST3 editor window

**VST3 SDK Integration**:

```cpp
// Simplified integration flow
Module* module = VST3::Hosting::Module::create(path);
IPluginFactory* factory = module->getFactory();

// Find and create component
IComponent* component;
factory->createInstance(classID, IComponent::iid, &component);

// Query interfaces
component->queryInterface(IAudioProcessor::iid, &audioProcessor);
component->queryInterface(IEditController::iid, &editController);

// Initialize
component->initialize(hostContext);
component->setActive(true);
audioProcessor->setProcessing(true);

// Process
ProcessData data;
data.inputs = inputBuses;
data.outputs = outputBuses;
audioProcessor->process(data);
```

**State Management**:
- Component state (processor)
- Controller state (parameters, editor)
- Both saved/restored together

### ParameterBridge

**Responsibility**: Thread-safe bidirectional parameter synchronization

**Design**:
- Two lock-free SPSC queues (single-producer, single-consumer)
- One queue for Host → Plugin updates
- One queue for Plugin → Host updates
- Fixed-size arrays for current parameter values (read without locks)

**Usage Pattern**:

```cpp
// UI thread: User changes parameter
paramBridge->setParameterFromHost(paramID, value);

// Audio thread: Apply queued changes
ParameterUpdate update;
while (paramBridge->getUpdateForPlugin(update)) {
    vst3Host->setParameterValue(update.parameterIndex, update.normalizedValue);
}

// Audio thread: Plugin modulated a parameter
paramBridge->setParameterFromPlugin(paramID, newValue);

// UI thread: Update AAX parameter
ParameterUpdate update;
while (paramBridge->getUpdateForHost(update)) {
    SetParameter(update.parameterIndex, update.normalizedValue);
}
```

**Lock-Free Queue**:
- Template-based for type safety
- Atomic operations for indices
- Memory ordering carefully controlled
- Fixed size (1024 or 2048 entries)
- Drops updates if queue is full (graceful degradation)

### StateManager

**Responsibility**: Serialize/deserialize complete plugin state

**Format**:

```
┌─────────────────────────────────────────┐
│ Header (24 bytes)                       │
│  - Magic: 'PTWP' (0x50545750)           │
│  - Version: uint32                      │
│  - Wrapper param count: uint32          │
│  - Wrapper param size: uint32           │
│  - Plugin state size: uint32            │
│  - Plugin path size: uint32             │
├─────────────────────────────────────────┤
│ Wrapper Parameters (doubles)            │
│  - Bypass (0.0 or 1.0)                  │
│  - Output Gain (dB)                     │
│  - Reserved for future                  │
├─────────────────────────────────────────┤
│ VST3 Plugin State (binary blob)         │
│  - Component state                      │
│  - Controller state                     │
│  - Opaque to wrapper                    │
├─────────────────────────────────────────┤
│ Plugin Path (UTF-8 string)              │
│  - Absolute path to VST3 plugin         │
│  - Used to reload on session open       │
└─────────────────────────────────────────┘
```

**Version Compatibility**:
- Future versions can add fields
- Deserializer checks version
- Graceful handling of older formats

### LockFreeQueue

**Responsibility**: Thread-safe SPSC queue without locks

**Implementation**:
- Circular buffer (fixed size array)
- Two atomic indices: read and write
- Memory ordering: acquire/release semantics
- Never blocks or allocates

**Algorithm**:

```
push(item):
    currentWrite = writeIndex (relaxed)
    nextWrite = (currentWrite + 1) % Size
    if nextWrite == readIndex (acquire):
        return false  // Queue full
    buffer[currentWrite] = item
    writeIndex = nextWrite (release)
    return true

pop(item):
    currentRead = readIndex (relaxed)
    if currentRead == writeIndex (acquire):
        return false  // Queue empty
    item = buffer[currentRead]
    readIndex = (currentRead + 1) % Size (release)
    return true
```

**Memory Ordering**:
- Relaxed: Local reads of owned index
- Acquire: Reads of other thread's index
- Release: Writes to own index (makes data visible)

## AAX Components

### AAX_Parameters

**Responsibility**: AAX parameter model implementation

**Inheritance**: Would inherit from `AAX_CEffectParameters` in real implementation

**Parameters**:
1. **Wrapper Parameters**:
   - Bypass (bool)
   - Output Gain (float, -48 to +12 dB)

2. **VST3 Parameter Proxies**:
   - Up to 256 VST3 parameters
   - Mapped to AAX parameter IDs 100-355
   - Names/units from VST3 plugin

**Chunk Interface**:
- `GetChunk()`: Serialize state using StateManager
- `SetChunk()`: Deserialize state, reload plugin if needed

**Parameter Sync**:
- Changes from AAX → ParameterBridge → VST3
- Changes from VST3 → ParameterBridge → AAX (via TouchParameter)

### AAX_Algorithm

**Responsibility**: Real-time audio processing

**Function Signature**:
```cpp
void AAX_AlgorithmProcessCallback(
    AAX_AlgorithmContext* const inInstancesBegin[],
    const void* inInstancesEnd);
```

**Context Structure**:
```cpp
struct AAX_AlgorithmContext {
    // Buffers
    const float* const* inputBuffers;
    float* const* outputBuffers;
    int32_t numInputChannels;
    int32_t numOutputChannels;
    int32_t numFrames;
    
    // Parameters (from private data)
    bool bypass;
    float outputGain;
    
    // VST3 integration
    VST3Host* vst3Host;
    ParameterBridge* paramBridge;
    
    // MIDI
    const void* midiInput;
};
```

**Processing Flow**:
1. Check bypass → copy input to output if bypassed
2. Get parameter updates from bridge
3. Apply parameter changes to VST3
4. Process audio through VST3 plugin
5. Apply output gain
6. Send plugin parameter changes back to bridge

**Real-Time Safety**:
- No allocations
- No locks (uses lock-free queue)
- No system calls
- No file I/O
- Pre-allocated buffers only

### AAX_GUI

**Responsibility**: User interface

**Layout**:

```
┌─────────────────────────────────────────────────────┐
│ Patchwrap                                      [X]  │
├─────────────────────────────────────────────────────┤
│                                                     │
│ Plugin: [Select VST3 Plugin ▼]  [Load] [Unload]   │
│                                                     │
│ ├─ Wrapper Controls ──────────────────────────────┤│
│ │                                                  ││
│ │  Bypass: [ ]    Output Gain: [====|====] 0.0dB  ││
│ │                                                  ││
│ │  Input:  [====|    ]  Output: [=======|  ]      ││
│ │                                                  ││
│ └──────────────────────────────────────────────────┘│
│                                                     │
│ ├─ VST3 Plugin Editor ─────────────────────────────┤│
│ │                                                  ││
│ │  ┌──────────────────────────────────────────┐   ││
│ │  │                                          │   ││
│ │  │   (Embedded VST3 Plugin GUI)             │   ││
│ │  │   or                                     │   ││
│ │  │   (Generated Parameter Controls)         │   ││
│ │  │                                          │   ││
│ │  └──────────────────────────────────────────┘   ││
│ │                                                  ││
│ └──────────────────────────────────────────────────┘│
│                                                     │
└─────────────────────────────────────────────────────┘
```

**Controls**:
- Plugin selector dropdown (populated from scan)
- Load/Unload buttons
- Bypass checkbox
- Output gain slider with dB label
- Input/output level meters (VU or peak)
- VST3 editor container (embedded HWND on Windows)
- Fallback parameter controls if no VST3 editor

**Windows Implementation**:
- Parent window from AAX
- Child windows for controls
- VST3 editor embedded via `IPlugView::attached()`
- Message handling for control events
- WM_SIZE for resizing
- DPI awareness via SetProcessDpiAwareness

**Resizing**:
- Query VST3 editor for preferred size
- Allow user to resize window
- Notify VST3 editor via `IPlugView::onSize()`
- Save/restore size in plugin state

### AAX_Describe

**Responsibility**: Plugin registration with Pro Tools

**Registration Steps**:

```cpp
void DescribePlugin(ICollection* collection) {
    // 1. Describe plugin properties
    AAX_IEffectDescriptor* desc = collection->NewDescriptor();
    desc->AddName("Patchwrap");
    desc->AddCategory(AAX_ePlugInCategory_None);  // Utility
    
    // 2. Describe parameters
    desc->AddParameter(eParam_Bypass, "Bypass", /* ... */);
    desc->AddParameter(eParam_OutputGain, "Output Gain", /* ... */);
    // + VST3 parameters
    
    // 3. Describe algorithm
    desc->AddProcessProc(
        AAX_AlgorithmProcessCallback,
        eParam_Algorithm_StereoInStereoOut,
        sizeof(AAX_AlgorithmContext));
    
    // 4. Describe meters
    desc->AddMeter(eParam_InputMeterLeft);
    desc->AddMeter(eParam_InputMeterRight);
    desc->AddMeter(eParam_OutputMeterLeft);
    desc->AddMeter(eParam_OutputMeterRight);
    
    // 5. Describe plugin info
    desc->AddResourceInfo(AAX_eResourceType_PageTable, "PatchwrapPages.xml");
}
```

**Product ID**: Must be registered with Avid

**Algorithm Types**:
- Mono in, mono out
- Stereo in, stereo out
- (Future: surround configurations)

## Data Flow

### Audio Processing Path

```
Pro Tools Audio Engine
    ↓
AAX_AlgorithmProcessCallback
    ↓
Check bypass → [if bypassed: copy input to output, return]
    ↓
Read parameter updates from ParameterBridge
    ↓
Apply parameter changes to VST3Host
    ↓
VST3Host::processAudio()
    ↓
Convert buffer format for VST3
    ↓
Add MIDI events
    ↓
IAudioProcessor::process()
    ↓
VST3 Plugin processes audio
    ↓
Convert buffer format back
    ↓
Apply output gain
    ↓
Return to Pro Tools
```

### GUI Update Path

```
User interacts with control
    ↓
Windows message (WM_COMMAND, WM_HSCROLL, etc.)
    ↓
AAX_GUI event handler
    ↓
AAX_Parameters::SetParameter()
    ↓
AAX SetParameter() call
    ↓
ParameterBridge::setParameterFromHost()
    ↓
Lock-free queue to audio thread
    ↓
AAX_Algorithm reads and applies
```

### State Recall Path

```
Pro Tools loads session
    ↓
AAX_Parameters::SetChunk()
    ↓
StateManager::deserialize()
    ↓
Extract wrapper parameters
    ↓
Extract VST3 plugin path
    ↓
Extract VST3 plugin state
    ↓
VST3Host::loadPlugin(path)
    ↓
VST3Host::loadState(state)
    ↓
Update GUI
    ↓
Sync parameters
```

## Memory Management

### Static Allocations

Pre-allocated during initialization:
- Parameter value arrays (256 * sizeof(double))
- Lock-free queue buffers (1024 * sizeof(ParameterUpdate))
- Audio buffer pointers (fixed channel count)

### Dynamic Allocations (Non-Real-Time)

Allowed during:
- Plugin loading (VST3 module, component, controller)
- GUI creation (Windows controls)
- State serialization/deserialization
- String operations (plugin names, paths)

**Never** in audio thread.

### RAII

Use RAII for resource management:
- `std::unique_ptr` for owned objects
- Automatic cleanup in destructors
- No manual `delete` calls

## Error Handling

### Audio Thread

- **No exceptions**: Audio thread must not throw
- **Graceful degradation**: If VST3 plugin fails, bypass
- **Validation**: Check pointers before dereferencing
- **Silent failure**: Log errors but continue processing

### UI Thread

- **User feedback**: Show error dialogs for failures
- **Retry logic**: Allow user to retry failed operations
- **Recovery**: Unload failed plugin, return to safe state

### State Loading

- **Version checking**: Verify state version before deserializing
- **Validation**: Check magic number, sizes
- **Fallback**: If state invalid, use defaults
- **Missing plugin**: If VST3 not found, notify user but don't crash

## Performance Considerations

### Latency

Total latency = AAX buffer size + VST3 plugin latency

- Query VST3 plugin latency via `IAudioProcessor::getLatencySamples()`
- Report to AAX via algorithm descriptor
- Pro Tools automatically compensates

### CPU Usage

- VST3 processing dominates CPU time
- Wrapper overhead should be < 1%
- Lock-free queues are very fast (atomic operations only)
- Bypass mode has minimal overhead (memcpy only)

### Memory Usage

- Base wrapper: ~100 KB
- VST3 plugin: Varies (plugin-dependent)
- State storage: Wrapper ~10 KB + VST3 state (varies)

### Optimization

- Compiler: Release mode with `/O2` optimization
- SIMD: Let VST3 plugin handle (don't interfere)
- Inlining: Let compiler decide
- Profile: Use VTune or similar if needed

## Future Enhancements

### Phase 2

- VST3 preset management
- Multiple plugin instances in one wrapper
- Modulation matrix
- Extended MIDI support (MPE, poly aftertouch)

### Phase 3

- macOS support (AAX for Mac)
- Surround configurations (5.1, 7.1, Atmos)
- Offline processing (AudioSuite)
- Sidechain support

### Phase 4

- Plugin sandboxing (crash protection)
- Performance monitoring
- Automatic gain compensation
- Mix/dry control

## References

- [AAX SDK Documentation](https://developer.avid.com/)
- [VST3 SDK Documentation](https://steinbergmedia.github.io/vst3_doc/)
- [Lock-Free Algorithms](https://preshing.com/20120612/an-introduction-to-lock-free-programming/)
- [Real-Time Audio Programming](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing)
