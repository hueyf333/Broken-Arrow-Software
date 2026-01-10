# Ampeg Heritage SVT Bass Amp Plugin

A high-quality VST3 bass amplifier plugin modeled on the legendary Ampeg Heritage SVT bass head, built with JUCE and C++20.

## Features

- **Authentic SVT modeling** with multi-stage tube preamp simulation
- **Complete tone stack** with Bass, Mid, and Treble controls
- **Mid frequency selector** with Heritage SVT centers (220 Hz, 450 Hz, 800 Hz, 1.6 kHz, 3 kHz)
- **Voicing switches**: Bright, Ultra-Lo, Ultra-Hi
- **Power amp saturation** with realistic sag and compression
- **Cabinet simulation** with impulse response support
- **2x oversampling** on nonlinear stages for reduced aliasing
- **Parameter smoothing** to prevent zipper noise
- **Output protection limiter** for safe operation
- **Factory presets**: Clean, Classic Grind, Pushed Power, Deep/Ultra-Lo
- **Mono input, stereo output** support

## Build Instructions

### Prerequisites

- **CMake** 3.22 or higher
- **C++20 compatible compiler**:
  - Windows: Visual Studio 2022 or later
  - macOS: Xcode 13 or later
  - Linux: GCC 11+ or Clang 13+
- **JUCE Framework** (included as submodule)

### Building on Windows (Visual Studio 2022)

```bash
# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake .. -G "Visual Studio 17 2022" -A x64

# Build the project
cmake --build . --config Release

# The VST3 will be in: build/AmpegSVT_artefacts/Release/VST3/
```

### Building on macOS (Xcode)

```bash
# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Create build directory
mkdir build
cd build

# Generate Xcode project
cmake .. -G Xcode

# Build the project
cmake --build . --config Release

# The VST3 will be in: build/AmpegSVT_artefacts/Release/VST3/
# The AU will be in: build/AmpegSVT_artefacts/Release/AU/ (if enabled)
```

### Building on Linux

```bash
# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential cmake libasound2-dev libfreetype6-dev libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev

# Create build directory
mkdir build
cd build

# Generate makefiles
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build .

# The VST3 will be in: build/AmpegSVT_artefacts/Release/VST3/
```

## Installation

### Windows
Copy the `.vst3` folder to:
- `C:\Program Files\Common Files\VST3\`

### macOS
Copy the `.vst3` bundle to:
- `~/Library/Audio/Plug-Ins/VST3/` (user)
- `/Library/Audio/Plug-Ins/VST3/` (system)

### Linux
Copy the `.vst3` folder to:
- `~/.vst3/` (user)
- `/usr/lib/vst3/` (system)

## Controls

### Input Section
- **Input Pad**: -20 dB to 0 dB attenuation for hot signals
- **Input Trim**: ±12 dB gain adjustment
- **Gate Enable**: Toggle noise gate on/off
- **Gate Threshold**: -80 dB to -20 dB threshold

### Preamp Section
- **Drive**: 0-100% tube preamp overdrive
- **Bias**: Asymmetric clipping characteristic (-100% to +100%)

### Tone Stack
- **Bass**: ±12 dB shelf at 100 Hz
- **Mid**: ±12 dB peak at selected frequency
- **Treble**: ±12 dB shelf at 3 kHz
- **Mid Frequency Selector**: Choose from 220 Hz, 450 Hz, 800 Hz, 1.6 kHz, or 3 kHz

### Voicing Switches
- **Bright**: +6 dB high shelf at 5 kHz for added clarity
- **Ultra-Lo**: +8 dB low shelf at 40 Hz for extended bass
- **Ultra-Hi**: +6 dB high shelf at 8 kHz for enhanced presence

### Power Amp Section
- **Drive**: Power tube saturation amount (0-100%)
- **Master**: Output level control (0-100%)

### Cabinet Simulation
- **Cab Mix**: Blend between dry signal and cabinet simulation (0-100%)

### Output Section
- **Output Gain**: ±12 dB final output trim
- **Bypass**: True bypass switch

## DSP Overview

### Signal Flow

1. **Input Conditioning**
   - Pad attenuation for level matching
   - DC blocking filter (high-pass at 20 Hz)
   - Optional noise gate with smooth envelope
   - Input trim gain

2. **Multi-Stage Tube Preamp**
   - Three cascaded gain stages with individual characteristics
   - Asymmetric soft clipping using tanh functions
   - Bias control for asymmetric distortion
   - Low-pass filtering at each stage (8 kHz, 6 kHz, 5 kHz)
   - Level-dependent response

3. **SVT-Style Tone Stack**
   - Bass: Low shelf filter at 100 Hz
   - Mid: Parametric peak with selectable frequency
   - Treble: High shelf filter at 3 kHz
   - Bright switch: High shelf boost at 5 kHz
   - Ultra-Lo switch: Sub-bass boost at 40 Hz
   - Ultra-Hi switch: High frequency boost at 8 kHz

4. **Power Amp Stage**
   - Power supply sag simulation (dynamic compression)
   - Asymmetric saturation characteristic
   - Output transformer filtering (low-pass at 7 kHz)
   - Master volume control

5. **Cabinet Simulation**
   - Impulse response convolution OR resonant filter network
   - Bass cab resonances at 80 Hz and 200 Hz
   - Speaker roll-off (low-pass at 4.5 kHz)
   - Sub-bass filtering (high-pass at 50 Hz)
   - Dry/wet mix control

6. **Output Protection**
   - Brick-wall limiter at 0 dBFS
   - Fast release (50 ms) for transparency
   - Denormal protection throughout

### Technical Details

- **Oversampling**: 2x on nonlinear stages (preamp and power amp) to reduce aliasing
- **Parameter Smoothing**: 50 ms ramp time on all gain and EQ parameters
- **Sample Rates**: 44.1 kHz, 48 kHz, 88.2 kHz, 96 kHz supported
- **Latency**: Minimal (oversampling filter latency only)
- **CPU Usage**: Optimized for real-time performance

## Modeling Assumptions

### Tube Preamp
- Modeled after 12AX7/ECC83 triode stages
- Asymmetric clipping reflects cathode bias and component tolerances
- Progressive gain staging mimics cascaded tube stages
- Frequency response matches typical tube amp behavior

### Tone Stack
- Based on James-Baxandall active tone control topology
- Interactive EQ behavior similar to Heritage SVT
- Mid frequency centers approximate Heritage SVT switch positions
- Voicing switches modeled after SVT circuit modifications

### Power Amp
- Simulates 6550/KT88 output tubes in push-pull configuration
- Sag effect models power supply compression under load
- Asymmetric saturation reflects output transformer characteristics
- Output filtering represents speaker load interaction

### Cabinet Simulation
- Default IR emulates Ampeg 8x10 cabinet response
- Resonances tuned to typical sealed bass cabinet behavior
- Supports loading custom IRs for flexibility

## Factory Presets

1. **Clean**: Transparent, vintage SVT tone
   - Low drive on preamp and power amp
   - Balanced EQ with slight bass boost
   - Ideal for fingerstyle and clarity

2. **Classic Grind**: SVT overdrive character
   - Medium drive on both stages
   - Enhanced mids and treble
   - Great for rock and aggressive playing

3. **Pushed Power**: Power amp saturation focus
   - Low preamp drive, high power drive
   - Warm, compressed tone with sag
   - Perfect for dynamic playing and tube compression

4. **Deep/Ultra-Lo**: Extended low-end response
   - Clean with Ultra-Lo switch engaged
   - 220 Hz mid frequency selected
   - Massive bass for modern styles

## Validation Steps

### Audio Quality Checks

1. **No Zipper Noise**
   - Sweep all knobs while playing back audio
   - Verify smooth parameter changes with no clicks or zippers
   - Test preset switching during playback

2. **Click-Free Preset Switching**
   - Load different presets while audio is playing
   - Confirm smooth transitions without clicks
   - Parameter ramping should prevent artifacts

3. **Level-Safe Operation**
   - Bypass should maintain consistent levels
   - Output protection limiter prevents clipping
   - Output trim allows gain staging

4. **Mono-In/Stereo-Out**
   - Verify mono input sums correctly if stereo source
   - Confirm stereo output maintains imaging
   - Cabinet simulation creates natural stereo field

### Performance Checks

1. **CPU Usage**
   - Monitor CPU usage in DAW
   - Should remain under 5% on modern systems (single instance)
   - Oversampling can be adjusted if needed

2. **Latency**
   - Check reported latency in DAW
   - Minimal latency from oversampling filters
   - Suitable for real-time monitoring

3. **Sample Rate Support**
   - Test at 44.1, 48, 88.2, and 96 kHz
   - Verify consistent behavior across rates
   - Confirm filter frequencies scale correctly

### Functional Checks

1. **Parameter Persistence**
   - Save and load project
   - Verify all parameters recall correctly
   - Check preset recall accuracy

2. **Bypass Functionality**
   - Engage bypass, confirm true bypass
   - No processing artifacts
   - Clean signal path

3. **Metering**
   - Verify input and output meters respond
   - Clip indicator lights when appropriate
   - Meters update smoothly

## Troubleshooting

### Build Issues

**CMake Error: Could not find JUCE**
- Ensure JUCE submodule is initialized: `git submodule update --init --recursive`

**Compiler Error: C++20 features not available**
- Update your compiler to a C++20-compatible version
- On Windows: Use Visual Studio 2022 or later
- On macOS: Use Xcode 13 or later

### Runtime Issues

**Plugin not appearing in DAW**
- Verify installation path is correct for your OS
- Rescan plugins in your DAW
- Check DAW's plugin blacklist

**No audio output**
- Check routing in DAW (mono in, stereo out)
- Verify master and output gain are not at minimum
- Ensure bypass is not engaged

**High CPU usage**
- Disable oversampling if CPU is limited
- Check buffer size settings in DAW
- Update to latest plugin version

## License

See LICENSE file for details.

## Credits

- **Development**: Broken Arrow Software
- **JUCE Framework**: JUCE by Roli
- **Inspired by**: Ampeg Heritage SVT bass amplifier

## Support

For issues and feature requests, please visit:
https://github.com/hueyf333/Broken-Arrow-Software/issues