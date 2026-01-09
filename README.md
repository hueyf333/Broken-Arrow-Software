# SVT-CL Bass Amplifier Plugin

A production-ready Ampeg SVT-CL style bass amplifier plugin built with JUCE and C++20.

## Features

### Audio/DSP
- **Multi-stage tube preamp**: Cascaded nonlinear stages with asymmetric waveshaping for authentic tube behavior
- **SVT-style interactive tone stack**: Bass, Mid, and Treble controls with 5-position Mid frequency selector (220Hz, 450Hz, 800Hz, 1.6kHz, 3kHz)
- **Ultra Lo/Ultra Hi filters**: Additional tone shaping controls
- **Power amplifier saturation**: Compression and saturation stage with output limiting
- **Cabinet simulator**: Optional 8x10 cab simulation using convolution
- **Oversampling**: 4x oversampling on nonlinear stages to minimize aliasing
- **Parameter smoothing**: Smooth parameter changes to avoid clicks and pops
- **Denormal handling**: Robust DSP that handles denormals properly
- **Multi-sample rate support**: Works at 44.1kHz, 48kHz, 96kHz, and beyond

### Parameters
- **Gain**: Input gain (0-60 dB)
- **Pad**: Input attenuation switch (-6 dB)
- **Bass**: Low frequency shelf (±12 dB at 80 Hz)
- **Mid**: Parametric peak at selected frequency (±12 dB)
- **Mid Frequency**: 5-position selector (220/450/800/1600/3000 Hz)
- **Treble**: High frequency shelf (±12 dB at 5 kHz)
- **Ultra Lo**: Low-pass filter (40-200 Hz)
- **Ultra Hi**: High-pass filter (3-8 kHz)
- **Master**: Output level (-40 to +10 dB)
- **Cab Sim**: Toggle cabinet simulation on/off
- **Standby**: Mute with warm-up behavior on re-engagement

### Factory Presets
1. **Default**: Balanced starting point
2. **Bright**: Enhanced highs and presence
3. **Deep**: Enhanced lows with warmth
4. **Growl**: Driven midrange for aggressive tones
5. **Clean**: Low-gain transparent sound

### User Interface
- Classic SVT-CL faceplate design with rotary knobs
- Input and output level meters with clip indicators
- Resizable window (600x400 to 1200x800)
- Real-time parameter visualization
- Standalone: Full audio device configuration

## Building

### Prerequisites
- CMake 3.22 or higher
- C++20 compatible compiler
  - Visual Studio 2022+ (Windows)
  - Xcode 13+ (macOS)
  - GCC 10+ or Clang 12+ (Linux)
- JUCE 7.0.12 (included as submodule)

### Build Instructions

#### Windows (Visual Studio 2022+)

```bash
# Configure CMake
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Release

# Outputs will be in:
# - VST3: build/SVT-CL_artefacts/Release/VST3/SVT-CL.vst3
# - Standalone: build/SVT-CL_artefacts/Release/Standalone/SVT-CL.exe
```

#### macOS

```bash
# Configure CMake
cmake -B build -G Xcode

# Build
cmake --build build --config Release

# Outputs will be in:
# - VST3: build/SVT-CL_artefacts/Release/VST3/SVT-CL.vst3
# - Standalone: build/SVT-CL_artefacts/Release/Standalone/SVT-CL.app
```

#### Linux

```bash
# Configure CMake
cmake -B build

# Build
cmake --build build --config Release

# Outputs will be in:
# - VST3: build/SVT-CL_artefacts/Release/VST3/SVT-CL.vst3
# - Standalone: build/SVT-CL_artefacts/Release/Standalone/SVT-CL
```

### Installation

#### Windows
Copy the VST3 folder to: `C:\Program Files\Common Files\VST3\`

#### macOS
Copy the VST3 bundle to: `~/Library/Audio/Plug-Ins/VST3/`

#### Linux
Copy the VST3 folder to: `~/.vst3/`

## Usage

### Standalone Application
The standalone version includes:
- Audio device selector (Settings menu)
- Input/output routing configuration
- Preset load/save functionality
- Full parameter control

### DAW Plugin (VST3)
Load the plugin on a bass track or bus. All parameters are automatable and support host preset management.

### Tips for Best Results
- Start with a factory preset and adjust to taste
- Use the Pad switch for hot input signals to prevent excessive distortion in the first preamp stage
- The Mid Frequency selector dramatically changes the character - experiment with different positions
- Ultra Lo and Ultra Hi are subtle but powerful tools for final tone shaping
- Enable Cab Sim for direct recording or disable when using your own cabinet impulse responses

## Project Structure

```
SVT-CL/
├── Source/
│   ├── DSP/
│   │   ├── TubePreamp.h/cpp      # Multi-stage tube preamp simulation
│   │   ├── ToneStack.h/cpp       # SVT-style EQ and filters
│   │   ├── PowerAmp.h/cpp        # Power amp saturation and limiting
│   │   ├── CabSimulator.h/cpp    # Cabinet impulse response
│   │   └── SVTProcessor.h/cpp    # Main DSP processor
│   ├── PluginProcessor.h/cpp     # JUCE plugin wrapper
│   └── PluginEditor.h/cpp        # GUI implementation
└── Resources/                     # (Future: graphics, IRs, etc.)
```

## Technical Details

### Signal Chain
```
Input → Pad → Multi-stage Tube Preamp → Tone Stack (Bass/Mid/Treble) → 
Ultra Lo/Hi Filters → Power Amp Saturation → Compressor → Limiter → 
Optional Cab Sim → Output
```

### DSP Implementation
- **Oversampling**: 4x polyphase IIR oversampling on all nonlinear stages
- **Tube Simulation**: Asymmetric waveshaping with cascaded stages
- **Tone Stack**: Butterworth IIR filters (shelving and parametric)
- **Compression**: Feed-forward compressor with 4:1 ratio
- **Limiting**: Brick-wall limiter at 0.95 full scale
- **Cab Sim**: Synthetic impulse response with multiple resonant modes

## License

See LICENSE file for details.

## Credits

Developed by Broken Arrow Software
Based on the legendary Ampeg SVT-CL bass amplifier