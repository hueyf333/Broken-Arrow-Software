# Ampeg Heritage SVT Bass Amp Plugin - Project Summary

## Overview

This is a complete, fully-functional JUCE-based VST3 bass amplifier plugin modeled after the legendary Ampeg Heritage SVT bass head. The plugin is built using C++20 and JUCE 7.0.9, targeting VST3 and Standalone formats.

## Project Structure

```
Broken-Arrow-Software/
├── Source/                      # Source code
│   ├── DSP/                    # Digital signal processing modules
│   │   ├── InputStage.cpp/h   # Input conditioning with pad, gate
│   │   ├── TubePreamp.cpp/h   # Multi-stage tube preamp
│   │   ├── ToneStack.cpp/h    # SVT-style EQ and voicing switches
│   │   ├── PowerAmp.cpp/h     # Power amp with sag simulation
│   │   ├── CabSimulator.cpp/h # Cabinet simulation with IR support
│   │   └── Oversampler.cpp/h  # 2x oversampling for nonlinear stages
│   ├── Components/             # Custom GUI components
│   │   ├── CustomKnob.cpp/h   # Rotary knob control
│   │   ├── CustomSwitch.cpp/h # Toggle switch
│   │   └── LevelMeter.cpp/h   # VU meter display
│   ├── PluginProcessor.cpp/h   # Main audio processor
│   └── PluginEditor.cpp/h      # Plugin GUI
├── Assets/                      # Resources
│   └── README.md               # Asset documentation
├── Presets/                     # Factory presets
│   ├── Clean.preset
│   ├── ClassicGrind.preset
│   ├── PushedPower.preset
│   └── DeepUltraLo.preset
├── JUCE/                        # JUCE framework (submodule)
├── CMakeLists.txt              # CMake build configuration
├── AmpegSVT.jucer              # Projucer project file
├── README.md                   # Main documentation
├── INSTALL.md                  # Installation guide
└── .gitignore                  # Git ignore rules
```

## Key Features Implemented

### 1. Complete Signal Chain
- **Input Stage**: Pad (-20 to 0 dB), trim (±12 dB), DC blocker, optional gate
- **Tube Preamp**: 3-stage cascaded preamp with asymmetric clipping and bias control
- **Tone Stack**: Bass/Mid/Treble with selectable mid frequencies (220/450/800/1.6k/3k Hz)
- **Voicing Switches**: Bright, Ultra-Lo, Ultra-Hi for additional tonal shaping
- **Power Amp**: Saturation with power supply sag simulation
- **Cabinet Sim**: Resonant filter network or IR loading for bass cab response
- **Output Protection**: Brick-wall limiter at 0 dBFS

### 2. DSP Features
- 2x oversampling on nonlinear stages (preamp, power amp)
- 50ms parameter smoothing on all controls
- Denormal protection throughout
- Supports 44.1, 48, 88.2, and 96 kHz sample rates
- Mono-in/stereo-out signal flow

### 3. Parameters (APVTS)
All parameters exposed via AudioProcessorValueTreeState:
- Input: Pad, Trim, Gate Enable, Gate Threshold
- Preamp: Drive, Bias
- Tone: Bass, Mid, Treble, Mid Frequency Selector
- Voicing: Bright Switch, Ultra-Lo, Ultra-Hi
- Power: Drive, Master
- Cabinet: Mix
- Output: Gain, Bypass

### 4. GUI
- Ampeg-inspired dark aesthetic
- Input/output level meters
- Clip indicator
- Section labels (Input, Preamp, Tone, Power)
- 30 Hz refresh rate for smooth metering

### 5. Preset System
Four factory presets accessible via program change:
1. **Clean**: Low drive, balanced EQ, fingerstyle clarity
2. **Classic Grind**: Medium drive, enhanced mids/treble, rock tone
3. **Pushed Power**: Low preamp/high power drive, compressed warmth
4. **Deep/Ultra-Lo**: Clean with Ultra-Lo engaged, massive low end

## Build System

### Primary: CMake (Modern)
- Cross-platform build system
- Automatic JUCE integration
- Supports VS2022, Xcode, Make
- Generates VST3 and Standalone targets

### Alternative: Projucer (.jucer file)
- Traditional JUCE workflow
- Can regenerate project files for all platforms
- Includes VS2022, Xcode, and Linux Makefile exporters

## Building

### Quick Build (Linux example):
```bash
git clone --recursive https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

See [INSTALL.md](INSTALL.md) for detailed platform-specific instructions.

## Testing Status

### Completed:
- ✅ Linux build (GCC 13.3, CMake)
- ✅ VST3 format generation
- ✅ Standalone application generation
- ✅ Source code compiles cleanly
- ✅ Binary dependencies verified

### Pending:
- ⏳ Windows build test (VS2022)
- ⏳ macOS build test (Xcode)
- ⏳ DAW integration testing
- ⏳ Audio validation (zipper noise, bypass, etc.)
- ⏳ CPU performance profiling

## Technical Details

### Tube Preamp Modeling
- Three cascaded gain stages with individual filtering
- Asymmetric tanh clipping simulates valve behavior
- Bias parameter creates asymmetric harmonic content
- Progressive frequency roll-off (8/6/5 kHz) per stage

### Tone Stack Implementation
- Bass: Low shelf at 100 Hz (±12 dB)
- Mid: Parametric peak at selectable frequency (±12 dB)
- Treble: High shelf at 3 kHz (±12 dB)
- Bright: +6 dB high shelf at 5 kHz
- Ultra-Lo: +8 dB low shelf at 40 Hz
- Ultra-Hi: +6 dB high shelf at 8 kHz

### Power Amp Modeling
- Dynamic power supply sag based on signal level
- Attack/release envelopes (1ms/50ms)
- Asymmetric saturation for push-pull tube characteristics
- Output transformer filtering (7 kHz low-pass)

### Cabinet Simulation
- Default: Procedurally-generated bass cab IR
- Alternative: Resonant filter network with cab resonances at 80 Hz and 200 Hz
- Speaker roll-off at 4.5 kHz
- Sub-bass filtering at 50 Hz
- Supports loading custom WAV IRs

## Dependencies

### Build-time:
- CMake 3.22+
- C++20 compiler (VS2022, GCC 11+, Clang 13+)
- JUCE 7.0.9 (included as submodule)

### Run-time (Linux):
- ALSA (`libasound2`)
- FreeType (`libfreetype6`)
- X11 libraries
- OpenGL/Mesa

## License

See [LICENSE](LICENSE) file.

## Credits

- **Developer**: Broken Arrow Software
- **Framework**: JUCE by ROLI
- **Inspiration**: Ampeg Heritage SVT bass amplifier

## Support

For issues, questions, or contributions:
- GitHub: https://github.com/hueyf333/Broken-Arrow-Software
- Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues

## Future Enhancements

Potential improvements for future versions:
- Custom GUI with knob graphics
- Additional cabinet IR presets
- MIDI CC mapping for automation
- A/B preset comparison
- Undo/redo system
- Preset browser
- Tuner module
- Compressor section
