# Installation Guide

## Quick Start

### Prerequisites

Before building the Ampeg SVT Bass Amp Plugin, ensure you have:

1. **CMake** (version 3.22 or higher)
2. **C++20 compatible compiler**:
   - Windows: Visual Studio 2022 or later
   - macOS: Xcode 13 or later  
   - Linux: GCC 11+ or Clang 13+

### Linux Dependencies

On Ubuntu/Debian-based systems:

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libasound2-dev \
    libfreetype6-dev \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libgl1-mesa-dev
```

### Clone and Build

1. **Clone the repository with submodules:**

```bash
git clone --recursive https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
```

If you already cloned without `--recursive`, initialize the submodule:

```bash
git submodule update --init --recursive
```

2. **Create build directory:**

```bash
mkdir build
cd build
```

3. **Generate build files:**

**Linux/macOS:**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

**Windows (Visual Studio 2022):**
```bash
cmake .. -G "Visual Studio 17 2022" -A x64
```

**macOS (Xcode):**
```bash
cmake .. -G Xcode
```

4. **Build the project:**

```bash
cmake --build . --config Release
```

5. **Locate the built plugin:**

The VST3 plugin will be in:
- Linux: `build/AmpegSVT_artefacts/VST3/Ampeg SVT Bass Amp.vst3/`
- Windows: `build\AmpegSVT_artefacts\Release\VST3\Ampeg SVT Bass Amp.vst3\`
- macOS: `build/AmpegSVT_artefacts/Release/VST3/Ampeg SVT Bass Amp.vst3/`

The Standalone application will be in:
- Linux: `build/AmpegSVT_artefacts/Standalone/Ampeg SVT Bass Amp`
- Windows: `build\AmpegSVT_artefacts\Release\Standalone\Ampeg SVT Bass Amp.exe`
- macOS: `build/AmpegSVT_artefacts/Release/Standalone/Ampeg SVT Bass Amp.app`

### Installing the Plugin

**Linux:**
```bash
cp -r "build/AmpegSVT_artefacts/VST3/Ampeg SVT Bass Amp.vst3" ~/.vst3/
```

**Windows:**
```powershell
xcopy /E /I "build\AmpegSVT_artefacts\Release\VST3\Ampeg SVT Bass Amp.vst3" "%CommonProgramFiles%\VST3\Ampeg SVT Bass Amp.vst3"
```

**macOS:**
```bash
cp -r "build/AmpegSVT_artefacts/Release/VST3/Ampeg SVT Bass Amp.vst3" ~/Library/Audio/Plug-Ins/VST3/
```

### Verify Installation

1. Open your DAW (Reaper, Ableton, Logic, etc.)
2. Rescan for plugins
3. Look for "Ampeg SVT Bass Amp" in your plugin list
4. Load it on a bass track and enjoy!

### Troubleshooting

**Build fails with "JUCE not found":**
- Make sure you cloned with `--recursive` or ran `git submodule update --init --recursive`

**Plugin doesn't appear in DAW:**
- Check that you copied it to the correct VST3 directory
- Rescan plugins in your DAW
- Check DAW's plugin blacklist/failed plugins

**Compile errors on older compilers:**
- Ensure you're using a C++20-compatible compiler
- Update to Visual Studio 2022+ on Windows
- Update to Xcode 13+ on macOS

For more help, see the main [README.md](README.md) or open an issue on GitHub.
