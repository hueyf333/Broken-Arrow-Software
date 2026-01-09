# Building Patchwrap AAX Plugin

This document describes how to build the Patchwrap AAX wrapper plugin for Pro Tools.

## Overview

Patchwrap is a Blue Cat Patchwork-style AAX wrapper plugin that allows hosting VST3 plugins inside Pro Tools. The project consists of two main components:

- **Patchwrap_Core**: Static library providing VST3 hosting and bridging functionality
- **Patchwrap_AAX**: AAX plugin that wraps the core library for use in Pro Tools

## Prerequisites

### Required Software

1. **Visual Studio 2022**
   - Windows 10 SDK
   - C++ Desktop Development workload
   - Platform Toolset v143

2. **AAX SDK**
   - Download from: https://developer.avid.com/
   - Requires Avid Developer account (free registration)
   - Recommended version: AAX SDK 2.4.0 or later

3. **VST3 SDK**
   - Automatically fetched via script (see Setup section)
   - Or manually download from: https://github.com/steinbergmedia/vst3sdk

4. **Git** (optional, for VST3 SDK download)

### System Requirements

- Windows 10 or later (64-bit)
- At least 4 GB RAM
- 2 GB free disk space

## Setup

### 1. Clone the Repository

```bash
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
```

### 2. Set Up AAX SDK

The AAX SDK must be downloaded separately from Avid:

1. Go to https://developer.avid.com/
2. Register for a free developer account
3. Download the AAX SDK
4. Extract to a location on your system (e.g., `C:\SDKs\AAX_SDK`)

**Important**: Note the installation path - you'll need it in the next step.

### 3. Configure AAX SDK Path

You must set the `AAX_SDK_PATH` environment variable to point to your AAX SDK installation:

**Option A: System Environment Variable (Recommended)**

1. Open Windows Settings → System → About → Advanced system settings
2. Click "Environment Variables"
3. Under "User variables", click "New"
4. Variable name: `AAX_SDK_PATH`
5. Variable value: `C:\SDKs\AAX_SDK` (or your actual path)
6. Click OK and restart Visual Studio

**Option B: Visual Studio Property Sheet**

1. Create a file `AAX_SDK.props` in the solution directory:

```xml
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup>
    <AAX_SDK_PATH>C:\SDKs\AAX_SDK</AAX_SDK_PATH>
  </PropertyGroup>
</Project>
```

2. Import this property sheet in both project files

### 4. Set Up VST3 SDK

Run the provided PowerShell script to download the VST3 SDK:

```powershell
cd scripts
.\fetch_vst3sdk.ps1
```

This will clone the VST3 SDK repository into `third_party/vst3sdk`.

**Manual Setup** (if script fails):

```bash
cd third_party
git clone --recursive https://github.com/steinbergmedia/vst3sdk.git
```

Or download manually from https://github.com/steinbergmedia/vst3sdk and extract to `third_party/vst3sdk`.

## Building

### Visual Studio GUI

1. Open `Patchwrap.sln` in Visual Studio 2022
2. Select build configuration:
   - **Debug**: For development and testing
   - **Release**: For distribution
3. Select platform: **x64**
4. Build → Build Solution (or press F7)

### Command Line

Using MSBuild from Developer Command Prompt:

```batch
# Debug build
msbuild Patchwrap.sln /p:Configuration=Debug /p:Platform=x64

# Release build
msbuild Patchwrap.sln /p:Configuration=Release /p:Platform=x64
```

### Build Output

After a successful build, you'll find the output in:

```
Build/
  Debug/   or   Release/
    Patchwrap_Core.lib          # Static library
    Patchwrap.aaxplugin/        # AAX plugin bundle
      Contents/
        x64/
          Patchwrap.aaxplugin   # Plugin DLL
        Info.plist              # Bundle metadata
        PkgInfo                 # Bundle type info
```

## Post-Build Packaging

The build process automatically creates the `.aaxplugin` bundle structure required by Pro Tools:

- The `package_aax.ps1` script runs after building Patchwrap_AAX
- Creates the proper bundle directory structure
- Copies the plugin DLL to the correct location
- Generates `Info.plist` and `PkgInfo` files

## Installation

### For Development/Testing

1. Copy the `Patchwrap.aaxplugin` folder to:
   ```
   C:\Program Files\Common Files\Avid\Audio\Plug-Ins
   ```

2. Launch Pro Tools
3. The plugin should appear in the Audio FX menu

### For Distribution

1. Sign the plugin with your Avid Developer certificate
2. Create an installer package
3. Include any required redistributables

**Note**: Production plugins must be signed by Avid. Contact Avid for PACE signing information.

## Project Structure

```
Broken-Arrow-Software/
├── Patchwrap.sln               # Visual Studio solution
├── BUILDING.md                 # This file
├── README.md                   # Project overview
├── .gitignore                  # Git ignore rules
│
├── Patchwrap_Core/             # Core library project
│   ├── include/                # Public headers
│   │   ├── Patchwrap_Core.h    # Main header
│   │   ├── VST3Host.h          # VST3 hosting
│   │   ├── ParameterBridge.h   # Parameter mapping
│   │   ├── LockFreeQueue.h     # Lock-free queue
│   │   └── StateManager.h      # State serialization
│   ├── src/                    # Implementation files
│   │   ├── VST3Host.cpp
│   │   ├── ParameterBridge.cpp
│   │   └── StateManager.cpp
│   └── Patchwrap_Core.vcxproj  # VS project file
│
├── Patchwrap_AAX/              # AAX plugin project
│   ├── include/                # AAX headers
│   │   ├── Patchwrap_AAX.h     # Main AAX header
│   │   ├── AAX_Parameters.h    # Parameter management
│   │   ├── AAX_Algorithm.h     # Audio processing
│   │   └── AAX_GUI.h           # User interface
│   ├── src/                    # Implementation files
│   │   ├── Patchwrap_AAX.cpp
│   │   ├── AAX_Parameters.cpp
│   │   ├── AAX_Algorithm.cpp
│   │   ├── AAX_GUI.cpp
│   │   └── AAX_Describe.cpp    # Plugin description
│   └── Patchwrap_AAX.vcxproj   # VS project file
│
├── scripts/                    # Build scripts
│   ├── package_aax.ps1         # AAX bundle packaging
│   └── fetch_vst3sdk.ps1       # VST3 SDK download
│
└── third_party/                # External dependencies
    └── vst3sdk/                # VST3 SDK (downloaded)
```

## Architecture

### Patchwrap_Core (Static Library)

**VST3Host** (`VST3Host.h/cpp`)
- Scans for VST3 plugins
- Loads/unloads VST3 plugins
- Manages VST3 plugin lifecycle
- Handles audio processing through VST3
- Bus negotiation (mono/stereo)
- MIDI event handling
- Editor embedding support

**ParameterBridge** (`ParameterBridge.h/cpp`)
- Bidirectional parameter synchronization between AAX and VST3
- Lock-free queue for real-time safe parameter updates
- Supports up to 256 VST3 parameters

**StateManager** (`StateManager.h/cpp`)
- Serializes wrapper state + VST3 plugin state
- Chunk-based format for Pro Tools compatibility
- Version-safe deserialization

**LockFreeQueue** (`LockFreeQueue.h`)
- Single-producer, single-consumer lock-free queue
- Template-based for type safety
- Used for real-time parameter updates

### Patchwrap_AAX (AAX Plugin)

**AAX_Parameters** (`AAX_Parameters.h/cpp`)
- AAX parameter model implementation
- Wrapper parameters: Bypass, Output Gain
- Proxy for VST3 parameters
- Chunk-based state management

**AAX_Algorithm** (`AAX_Algorithm.h/cpp`)
- Real-time audio processing callback
- **Real-time safe**: No allocations, locks, or I/O
- Processes audio through hosted VST3 plugin
- Applies output gain
- Handles bypass

**AAX_GUI** (`AAX_GUI.h/cpp`)
- Custom AAX user interface
- Plugin selector and loader
- Bypass and output gain controls
- Input/output level meters
- Embedded VST3 editor (if plugin provides one)
- Generated parameter controls (if no VST3 editor)
- Resizable and DPI-aware

**AAX_Describe** (`AAX_Describe.cpp`)
- Plugin registration with Pro Tools
- Defines plugin metadata
- Registers algorithm and parameters

## Features

### Core Features

✅ **VST3 Plugin Hosting**
- Load and run VST3 plugins inside Pro Tools
- Automatic bus configuration (mono/stereo)
- Full audio processing support
- MIDI event forwarding

✅ **Parameter Bridging**
- Bidirectional parameter synchronization
- Lock-free real-time updates
- Automation support
- Up to 256 VST3 parameters

✅ **State Management**
- Save/recall wrapper + plugin state
- Chunk-based format
- Preset compatibility
- Version-safe loading

✅ **User Interface**
- Plugin browser and selector
- Load/unload controls
- Bypass switch
- Output gain control
- Level meters
- VST3 editor embedding
- Resizable window
- DPI awareness

✅ **Real-Time Safety**
- No allocations in audio thread
- Lock-free parameter updates
- No file I/O in processing
- No blocking operations

### Latency Reporting

The plugin reports latency to Pro Tools based on the loaded VST3 plugin:
- Queries VST3 plugin for latency
- Reports to AAX host
- Pro Tools automatically compensates

## Troubleshooting

### Build Errors

**Error: Cannot find AAX SDK headers**
- Ensure `AAX_SDK_PATH` environment variable is set correctly
- Restart Visual Studio after setting environment variable
- Verify AAX SDK is extracted properly

**Error: Cannot find VST3 SDK**
- Run `scripts/fetch_vst3sdk.ps1` to download VST3 SDK
- Or manually clone to `third_party/vst3sdk`
- Ensure `--recursive` flag was used (for submodules)

**Error: Platform toolset not found**
- Install Visual Studio 2022 with C++ Desktop Development
- Update Visual Studio to latest version
- Verify Windows 10 SDK is installed

### Runtime Issues

**Plugin doesn't appear in Pro Tools**
- Verify plugin is in correct location: `C:\Program Files\Common Files\Avid\Audio\Plug-Ins`
- Check Pro Tools version (requires Pro Tools 11 or later)
- Ensure plugin is 64-bit
- Check Pro Tools console for error messages

**Plugin crashes on load**
- Verify all dependencies are present
- Check AAX SDK version compatibility
- Ensure plugin is built with matching runtime library (Multi-threaded)

**VST3 plugin doesn't load**
- Verify VST3 plugin path is correct
- Check VST3 plugin is compatible (VST3 format)
- Ensure VST3 plugin is 64-bit
- Check VST3 plugin is installed correctly

## Development

### Extending the Plugin

To add new features:

1. **Add wrapper parameters**: Edit `AAX_Parameters.h` and add to parameter enum
2. **Modify processing**: Edit `AAX_Algorithm.cpp` audio callback
3. **Update GUI**: Edit `AAX_GUI.cpp` to add new controls
4. **Add VST3 features**: Extend `VST3Host.cpp` implementation

### Debugging

1. Set Patchwrap_AAX as startup project
2. Project Properties → Debugging:
   - Command: `C:\Program Files\Avid\Pro Tools\ProTools.exe`
   - Working Directory: `$(OutDir)`
3. Set breakpoints and press F5

### Code Style

- C++17 standard
- Follow existing naming conventions
- Real-time code must be RT-safe (no allocations, locks, I/O)
- Comment complex algorithms
- Use meaningful variable names

## Version History

### Version 1.0.0 (Current)
- Initial implementation
- VST3 plugin hosting
- Basic parameter bridging
- State management
- GUI with plugin selector
- Real-time safe processing

## License

See LICENSE file in repository root.

## Support

For issues and questions:
- GitHub Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues
- Documentation: See README.md

## References

- AAX SDK Documentation: https://developer.avid.com/
- VST3 SDK: https://github.com/steinbergmedia/vst3sdk
- Pro Tools Developer: https://www.avid.com/pro-tools
