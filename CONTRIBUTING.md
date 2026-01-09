# Contributing to Patchwrap

Thank you for your interest in contributing to Patchwrap!

## Getting Started

1. Fork the repository
2. Clone your fork
3. Follow the setup instructions in [BUILDING.md](BUILDING.md)
4. Create a feature branch: `git checkout -b feature/your-feature-name`

## Development Guidelines

### Code Style

- **C++ Standard**: C++17
- **Naming Conventions**:
  - Classes: PascalCase (e.g., `VST3Host`)
  - Functions/Methods: camelCase (e.g., `loadPlugin()`)
  - Variables: camelCase (e.g., `sampleRate`)
  - Constants: UPPER_CASE (e.g., `MAX_PARAMETERS`)
  - Private members: camelCase (e.g., `currentValue`)
- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening brace on same line for functions/classes
- **Comments**: Use `//` for single-line, `/* */` for multi-line

### Real-Time Safety

Code in the audio thread (AAX_Algorithm.cpp, VST3Host::processAudio) **MUST** be real-time safe:

❌ **Never do in audio thread**:
- Memory allocation (`new`, `malloc`, `std::vector::push_back`)
- File I/O (reading/writing files)
- Mutex locks or blocking operations
- System calls that might block
- Console output (`printf`, `std::cout`)

✅ **Safe in audio thread**:
- Stack allocation (local variables)
- Lock-free data structures
- Simple arithmetic
- Pointer dereferencing
- Pre-allocated buffer access

### Testing

Before submitting a pull request:

1. **Build both Debug and Release configurations**
   ```bash
   msbuild Patchwrap.sln /p:Configuration=Debug /p:Platform=x64
   msbuild Patchwrap.sln /p:Configuration=Release /p:Platform=x64
   ```

2. **Test in Pro Tools** (if you have AAX SDK and Pro Tools installed)
   - Load a VST3 plugin
   - Process audio
   - Automate parameters
   - Save and recall state
   - Test GUI functionality

3. **Check for compiler warnings**
   - Fix all warnings before submitting
   - Enable `/W3` or `/W4` warning level

## Pull Request Process

1. **Update documentation** if you changed APIs or behavior
2. **Add comments** to complex code sections
3. **Keep commits focused** - one logical change per commit
4. **Write clear commit messages**:
   ```
   Add support for VST3 preset loading
   
   - Implement preset scanning in VST3Host
   - Add preset selector to GUI
   - Update state management for presets
   ```

5. **Submit PR** with description of changes and motivation

## Areas for Contribution

### High Priority

- **Full VST3 SDK integration** in VST3Host.cpp
- **AAX SDK integration** in AAX_Describe.cpp
- **GUI implementation** - Windows controls, VST3 editor embedding
- **Testing** - Unit tests, integration tests
- **macOS support** - AAX works on macOS too

### Medium Priority

- **VST3 preset management**
- **Parameter automation recording**
- **MIDI processing enhancements**
- **Performance optimizations**
- **Additional bus configurations** (5.1, 7.1 surround)

### Low Priority

- **Plugin favoriting/organization**
- **Skin/theme support**
- **Additional metering** (spectrum analyzer, etc.)
- **Undo/redo for parameter changes**

## Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                      Pro Tools (AAX Host)                 │
└─────────────────────────┬───────────────────────────────┘
                          │
                          │ AAX API
                          │
┌─────────────────────────▼───────────────────────────────┐
│                   Patchwrap_AAX                          │
│  ┌────────────────────────────────────────────────────┐ │
│  │ AAX_Parameters (Parameter Model)                   │ │
│  │  - Wrapper parameters (bypass, gain)               │ │
│  │  - VST3 parameter proxies                          │ │
│  │  - State management (chunks)                       │ │
│  └──────────────┬─────────────────────────────────────┘ │
│                 │                                         │
│  ┌──────────────▼─────────────────────────────────────┐ │
│  │ AAX_Algorithm (Real-Time Audio Processing)         │ │
│  │  - Real-time safe audio callback                   │ │
│  │  - Bypass handling                                  │ │
│  │  - Output gain application                          │ │
│  └──────────────┬─────────────────────────────────────┘ │
│                 │                                         │
│  ┌──────────────▼─────────────────────────────────────┐ │
│  │ AAX_GUI (User Interface)                           │ │
│  │  - Plugin selector                                  │ │
│  │  - Wrapper controls                                 │ │
│  │  - VST3 editor embedding                            │ │
│  │  - DPI-aware rendering                              │ │
│  └─────────────────────────────────────────────────────┘ │
└────────────┬────────────────────────────────────────────┘
             │
             │ Uses
             │
┌────────────▼────────────────────────────────────────────┐
│                   Patchwrap_Core                         │
│  ┌────────────────────────────────────────────────────┐ │
│  │ VST3Host (VST3 Plugin Hosting)                     │ │
│  │  - Scan/load/unload VST3 plugins                   │ │
│  │  - Audio processing delegation                     │ │
│  │  - Parameter management                             │ │
│  │  - State save/restore                               │ │
│  │  - Editor management                                │ │
│  └──────────────┬─────────────────────────────────────┘ │
│                 │                                         │
│  ┌──────────────▼─────────────────────────────────────┐ │
│  │ ParameterBridge (Bidirectional Sync)               │ │
│  │  - Lock-free queues                                 │ │
│  │  - AAX ↔ VST3 parameter mapping                    │ │
│  │  - Real-time safe updates                           │ │
│  └─────────────────────────────────────────────────────┘ │
│                                                           │
│  ┌────────────────────────────────────────────────────┐ │
│  │ StateManager (Serialization)                        │ │
│  │  - Chunk format for Pro Tools                       │ │
│  │  - Wrapper + VST3 state                             │ │
│  └─────────────────────────────────────────────────────┘ │
└────────────┬────────────────────────────────────────────┘
             │
             │ Uses
             │
┌────────────▼────────────────────────────────────────────┐
│                     VST3 SDK                              │
│  - Plugin loading (Module)                                │
│  - Interfaces (IComponent, IAudioProcessor, etc.)         │
│  - Host implementation                                    │
└───────────────────────────────────────────────────────────┘
```

## Communication Flow

### Parameter Changes (UI → Plugin)

```
User adjusts parameter in GUI
    ↓
AAX_GUI updates AAX parameter
    ↓
AAX_Parameters::setParameter()
    ↓
ParameterBridge::setParameterFromHost()
    ↓
Lock-free queue to audio thread
    ↓
AAX_Algorithm reads from queue
    ↓
VST3Host::setParameterValue()
    ↓
VST3 Plugin receives parameter change
```

### Parameter Changes (Plugin → UI)

```
VST3 Plugin automation/modulation
    ↓
VST3Host detects parameter change
    ↓
ParameterBridge::setParameterFromPlugin()
    ↓
Lock-free queue to UI thread
    ↓
AAX_Parameters reads from queue
    ↓
AAX updates host parameter
    ↓
AAX_GUI reflects change
```

## Questions?

- Open an issue for bugs or feature requests
- Use discussions for questions
- Check existing issues before creating new ones

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).
