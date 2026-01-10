# Building SuperBassman

This document provides detailed instructions for building the SuperBassman plugin on different platforms.

## Prerequisites

### All Platforms
- CMake 3.15 or later
- C++20 compatible compiler
- JUCE framework (included in repository)

### Platform-Specific Requirements

#### Linux
```bash
sudo apt-get install -y \
    build-essential \
    cmake \
    libasound2-dev \
    libfreetype6-dev \
    libx11-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxcursor-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev
```

#### macOS
- Xcode 13 or later with Command Line Tools
- Install CMake via Homebrew: `brew install cmake`

#### Windows
- Visual Studio 2022 with C++ development tools
- CMake (included with VS2022 or install separately)

## Quick Build

### Linux / macOS
```bash
chmod +x build.sh
./build.sh
```

### Windows (PowerShell)
```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Manual Build

### Step-by-Step Build Process

1. **Clone the repository** (if not already done):
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Create build directory**:
   ```bash
   mkdir -p build
   cd build
   ```

3. **Configure with CMake**:
   
   **Linux/macOS**:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
   
   **Windows**:
   ```powershell
   cmake .. -G "Visual Studio 17 2022" -A x64
   ```

4. **Build**:
   
   **Linux/macOS**:
   ```bash
   make -j$(nproc)  # Linux
   make -j$(sysctl -n hw.ncpu)  # macOS
   ```
   
   **Windows**:
   ```powershell
   cmake --build . --config Release --parallel
   ```

## Build Outputs

After a successful build, you'll find:

### Linux
- **VST3**: `build/SuperBassman_artefacts/VST3/SuperBassman.vst3/`
- **Standalone**: `build/SuperBassman_artefacts/Standalone/SuperBassman`
- **Installed VST3**: `~/.vst3/SuperBassman.vst3/` (automatically installed)

### macOS
- **VST3**: `build/SuperBassman_artefacts/VST3/SuperBassman.vst3/`
- **AU**: `build/SuperBassman_artefacts/AU/SuperBassman.component/`
- **Standalone**: `build/SuperBassman_artefacts/Standalone/SuperBassman.app/`
- **Installed**: Automatically copied to:
  - VST3: `~/Library/Audio/Plug-Ins/VST3/`
  - AU: `~/Library/Audio/Plug-Ins/Components/`

### Windows
- **VST3**: `build\SuperBassman_artefacts\VST3\SuperBassman.vst3\`
- **Standalone**: `build\SuperBassman_artefacts\Standalone\SuperBassman.exe`
- **Installed VST3**: `%COMMONPROGRAMFILES%\VST3\SuperBassman.vst3\`

## Build Configurations

### Debug Build
Includes debugging symbols and disables optimizations:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

### Release Build (Recommended)
Optimized for performance:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

### Release with Debug Info
Optimized but includes debugging symbols:

```bash
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make
```

## Troubleshooting

### Linux

**Issue**: `ALSA lib ... failed`
- **Solution**: This is normal for headless environments. The plugin will work fine in a DAW.

**Issue**: Missing dependencies
- **Solution**: Run the prerequisite installation command again:
  ```bash
  sudo apt-get install -y libasound2-dev libfreetype6-dev libx11-dev \
       libxinerama-dev libxrandr-dev libxcursor-dev libgl1-mesa-dev libglu1-mesa-dev
  ```

**Issue**: CMake version too old
- **Solution**: Install a newer CMake:
  ```bash
  sudo apt-get install -y cmake
  ```

### macOS

**Issue**: CMake not found
- **Solution**: Install via Homebrew:
  ```bash
  brew install cmake
  ```

**Issue**: Xcode Command Line Tools missing
- **Solution**: Install them:
  ```bash
  xcode-select --install
  ```

**Issue**: Code signing errors
- **Solution**: Disable code signing in CMake (for local builds):
  ```bash
  cmake .. -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY=""
  ```

### Windows

**Issue**: CMake generator not found
- **Solution**: Specify the Visual Studio version explicitly:
  ```powershell
  cmake .. -G "Visual Studio 17 2022"
  ```

**Issue**: MSBuild not found
- **Solution**: Use CMake to build:
  ```powershell
  cmake --build . --config Release
  ```

**Issue**: C++20 not supported
- **Solution**: Ensure you have Visual Studio 2022 or later installed with C++ development tools.

## Build Options

You can customize the build by passing options to CMake:

### Plugin Formats
By default, VST3 and Standalone are built. To build specific formats:

```bash
# Build only VST3
cmake .. -DJUCE_BUILD_VST3=ON -DJUCE_BUILD_STANDALONE=OFF

# Build only Standalone
cmake .. -DJUCE_BUILD_VST3=OFF -DJUCE_BUILD_STANDALONE=ON
```

### Installation

To install the plugin system-wide after building:

```bash
sudo cmake --install .
```

Or to specify an installation prefix:

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
sudo cmake --install .
```

## Using Projucer (Alternative)

If you prefer using Projucer instead of CMake:

1. Download Projucer from https://juce.com/discover/projucer
2. Open `SuperBassman.jucer`
3. Configure your IDE settings (VS2022/Xcode)
4. Click "Save Project" to generate IDE project files
5. Open and build the generated project in your IDE

## Continuous Integration

The project includes CI configurations for:
- GitHub Actions (Linux, macOS, Windows)
- Travis CI
- AppVeyor

See `.github/workflows/` for CI scripts.

## Performance Optimization

For best performance in Release builds:

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-O3 -march=native" \
  -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
```

Note: `-march=native` optimizes for your specific CPU but the binary won't be portable.

## Clean Build

To start fresh:

```bash
rm -rf build
./build.sh
```

Or on Windows:

```powershell
Remove-Item -Recurse -Force build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Additional Resources

- JUCE Documentation: https://docs.juce.com/
- JUCE Forum: https://forum.juce.com/
- CMake Documentation: https://cmake.org/documentation/
- Project Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues
