# SuperBassman - Fender Super Bassman Bass Head Emulation

A production-ready JUCE C++20 VST3/Standalone plugin that authentically replicates the Fender Super Bassman Bass Head amplifier.

## Table of Contents
- [Overview](#overview)
- [Research Summary](#research-summary)
- [Features](#features)
- [Signal Flow](#signal-flow)
- [Build Instructions](#build-instructions)
- [Control Reference](#control-reference)
- [DSP Architecture](#dsp-architecture)
- [Presets](#presets)
- [System Requirements](#system-requirements)

## Overview

SuperBassman is a detailed emulation of the legendary Fender Super Bassman bass amplifier head. This plugin captures the tonal characteristics, channel structure, and dynamic response of the original hardware using advanced DSP modeling techniques.

## Research Summary

### Fender Super Bassman Bass Head

The Fender Super Bassman is a professional bass amplifier head known for:
- **Power Output**: 300 watts into 4/8 ohms
- **Tube Complement**: Multiple 12AX7 preamp tubes, 6L6GC power tubes
- **Channel Configuration**: 
  - Normal channel: Flat frequency response
  - Bright channel: Enhanced high-frequency response (+3dB @ 2kHz)
  - Overdrive channel: High-gain distortion stage
- **Tone Controls**: Interactive Fender-style EQ (Bass, Mid, Treble, Presence)
- **Cabinet Compatibility**: Designed for 8x10, 4x10, and 1x15 bass cabinets

### Signal Flow (Hardware Reference)

```
Input Jack → Input Stage → Channel Selection → Tube Preamp Stages →
Tone Stack → Overdrive (optional) → Power Amp → Output Transformer →
Speaker Output
```

### Control Set

**Input Section:**
- Input Gain/Trim
- Pad Switch (-10dB attenuation)
- High-Pass Filter (20-200 Hz)

**Preamp:**
- Gain (0-10): Controls preamp tube drive
- Bias (-1 to +1): Adjusts harmonic character

**Tone Stack:**
- Bass (0-10): Low shelf @ 100 Hz, ±10dB
- Mid (0-10): Peaking filter @ 500 Hz, ±7.5dB
- Treble (0-10): High shelf @ 3 kHz, ±10dB
- Presence (0-10): High shelf @ 6 kHz, ±7.5dB

**Overdrive Channel:**
- Drive (0-10): Distortion amount
- Blend (0-100%): Wet/dry mix
- Tone (0-10): Post-distortion tone shaping

**Power Amp:**
- Master Volume (0-10): Output level
- Sag (0-10): Power supply compression
- Compression (0-10): Power amp compression

**Cabinet Simulation:**
- Cabinet Type: 8x10 Sealed, 4x10 Ported, 1x15 Sealed
- Cabinet Mix (0-100%): Wet/dry blend

**Output:**
- Output Gain (-24 to +24 dB)
- Limiter: Soft limiting
- Bypass: True bypass

## Features

### DSP Processing
- **Multi-stage tube preamp modeling** with asymmetric nonlinearities
- **Interactive tone stack** based on Fender topology
- **Overdrive channel** with parallel blend
- **Power amp saturation** with dynamic sag and compression
- **2x oversampling** on nonlinear stages to reduce aliasing
- **Cabinet simulation** with multiple speaker configurations
- **Parameter smoothing** for click-free operation
- **Denormal protection** for CPU efficiency

### Audio Specifications
- Sample rates: 44.1, 48, 96 kHz
- Input: Mono
- Output: Stereo
- Processing: 32-bit floating point
- Latency: Minimal (oversampling compensation)

### User Interface
- **Vintage-inspired faceplate** design
- **Resizable window** (600x400 to 1800x1200)
- **Custom rotary knobs** with orange accent colors
- **Input/output level meters** with clip detection
- **Real-time parameter control** with automation support
- **Preset management** system

## Signal Flow

```
Input (Mono) → Input Gain/Pad → DC Blocker → High-Pass Filter →
Channel Routing (Normal/Bright/OD) → Tube Preamp (2x OS) →
Tone Stack (Bass/Mid/Treble/Presence) → Overdrive (2x OS, if selected) →
Power Amp (2x OS) → Cabinet Simulation → Output Limiter →
Output Gain → Stereo Output
```

### Processing Stages

1. **Input Stage**: Conditioning, DC removal, optional HPF
2. **Channel Routing**: Selects signal path and applies channel-specific EQ
3. **Tube Preamp**: Multi-stage asymmetric saturation with bias control
4. **Tone Stack**: Interactive 4-band EQ
5. **Overdrive**: Optional high-gain stage with blend
6. **Power Amp**: Saturation, compression, and sag
7. **Cabinet Sim**: Speaker frequency response modeling
8. **Output Stage**: Safety limiting and gain

## Build Instructions

### Prerequisites

**Windows (Visual Studio 2022):**
- Visual Studio 2022 with C++20 support
- Windows 10 SDK
- JUCE framework (included in repository)

**macOS (Xcode):**
- Xcode 13 or later
- macOS 11.0 or later
- Command Line Tools

**Linux:**
- GCC 10+ or Clang 12+ with C++20 support
- ALSA development libraries
- X11 development libraries
- FreeType development libraries

### Building

#### Windows (Visual Studio 2022)

1. Clone the repository:
   ```
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. Open the Projucer project:
   ```
   SuperBassman.jucer
   ```
   
3. In Projucer, click "Save Project" to generate Visual Studio solution

4. Open the generated solution:
   ```
   Builds/VisualStudio2022/SuperBassman.sln
   ```

5. Select configuration (Debug/Release) and build:
   - Build → Build Solution (Ctrl+Shift+B)

6. Plugins will be in:
   - VST3: `Builds/VisualStudio2022/x64/Release/VST3/SuperBassman.vst3`
   - Standalone: `Builds/VisualStudio2022/x64/Release/Standalone/SuperBassman.exe`

#### macOS (Xcode)

1. Clone the repository:
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. Open Projucer and save project to generate Xcode project

3. Open the generated project:
   ```bash
   open Builds/MacOSX/SuperBassman.xcodeproj
   ```

4. Select scheme and build:
   - Product → Build (⌘B)

5. Plugins will be in:
   - VST3: `~/Library/Audio/Plug-Ins/VST3/SuperBassman.vst3`
   - AU: `~/Library/Audio/Plug-Ins/Components/SuperBassman.component`
   - Standalone: `Builds/MacOSX/build/Release/SuperBassman.app`

#### Linux (Makefile)

1. Clone the repository:
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. Install dependencies:
   ```bash
   sudo apt-get install libasound2-dev libx11-dev libxinerama-dev \
        libxext-dev libfreetype6-dev libwebkit2gtk-4.0-dev \
        libglu1-mesa-dev
   ```

3. Generate Makefile using Projucer, then build:
   ```bash
   cd Builds/LinuxMakefile
   make CONFIG=Release
   ```

4. Plugins will be in:
   - VST3: `Builds/LinuxMakefile/build/SuperBassman.vst3`
   - Standalone: `Builds/LinuxMakefile/build/SuperBassman`

### Quick Build Script

A build script is provided for convenience:

```bash
chmod +x build.sh
./build.sh
```

## Control Reference

### Parameter IDs and Ranges

| Parameter | ID | Range | Default | Unit |
|-----------|-----|-------|---------|------|
| Input Gain | inputGain | -12 to +12 | 0 | dB |
| Input Pad | inputPad | Off/On | Off | - |
| DC Block | dcBlock | Off/On | On | - |
| HPF Enable | hpfEnable | Off/On | On | - |
| HPF Frequency | hpfFreq | 20-200 | 40 | Hz |
| Channel | channel | 0-2 | 0 | - |
| Preamp Gain | preampGain | 0-10 | 5 | - |
| Preamp Bias | preampBias | -1 to +1 | 0 | - |
| Bass | bass | 0-10 | 5 | - |
| Mid | mid | 0-10 | 5 | - |
| Treble | treble | 0-10 | 5 | - |
| Presence | presence | 0-10 | 5 | - |
| Drive | drive | 0-10 | 0 | - |
| Drive Blend | driveBlend | 0-100 | 0 | % |
| Drive Tone | driveTone | 0-10 | 5 | - |
| Master Volume | masterVolume | 0-10 | 5 | - |
| Power Sag | powerSag | 0-10 | 3 | - |
| Power Compression | powerCompression | 0-10 | 3 | - |
| Cab Enable | cabEnable | Off/On | On | - |
| Cab Type | cabType | 0-2 | 0 | - |
| Cab Mix | cabMix | 0-100 | 100 | % |
| Output Gain | outputGain | -24 to +24 | 0 | dB |
| Limiter Enable | limiterEnable | Off/On | On | - |
| Bypass | bypass | Off/On | Off | - |

### Channel Types
- 0: Normal (flat response)
- 1: Bright (high-frequency boost)
- 2: Overdrive (distortion channel)

### Cabinet Types
- 0: 8x10 Sealed (classic SVT-style, tight and focused)
- 1: 4x10 Ported (modern punch, extended highs)
- 2: 1x15 Sealed (vintage warmth, deep lows)

## DSP Architecture

### Oversampling Strategy
Nonlinear stages (tube preamp, overdrive, power amp) use 2x oversampling with polyphase IIR filters to minimize aliasing artifacts while maintaining CPU efficiency.

### Tube Modeling
Multi-stage asymmetric soft clipping simulates:
- Grid current saturation
- Cathode follower compression
- Bias-dependent harmonic generation

### Tone Stack
Interactive EQ based on Fender passive tone circuit:
- Controls interact realistically
- Frequency-dependent impedance
- Authentic mid scoop behavior

### Power Amp Sag
Envelope-following power supply sag:
- Fast attack, slow release
- Reduces gain during high input levels
- Simulates transformer compression

### Cabinet Simulation
Frequency response modeling:
- Speaker resonance
- Cabinet coloration
- Microphone positioning effects

## Presets

Included presets demonstrate various tonal characteristics:

1. **Classic Rock Bass**: Vintage tone with moderate drive
2. **Modern Slap**: Bright channel, scooped mids, tight lows
3. **Reggae Dub**: Deep 1x15 cabinet, warm lows
4. **Metal Grind**: High gain overdrive, aggressive mids
5. **Jazz Warmth**: Smooth compression, rolled-off highs
6. **Funk Punch**: 4x10 cabinet, enhanced presence
7. **Clean DI**: Bypass cabinet for direct recording
8. **Vintage Thump**: 8x10 cabinet, classic Motown tone

Presets are located in the `Presets/` directory as `.xml` files.

## System Requirements

### Minimum
- CPU: Dual-core 2.0 GHz
- RAM: 2 GB
- OS: Windows 10, macOS 11, or Linux with ALSA

### Recommended
- CPU: Quad-core 3.0 GHz or better
- RAM: 4 GB
- OS: Windows 11, macOS 12+, or recent Linux distribution

### Host DAW Compatibility
- Any VST3-compatible host
- Standalone application included

## License

Copyright 2026 Broken Arrow Software

See LICENSE file for details.

## Credits

**Development**: Broken Arrow Software  
**DSP Design**: Modeled on Fender Super Bassman specifications  
**UI Design**: Vintage amplifier aesthetic

## Support

For issues, feature requests, or questions:
- GitHub Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues

## Version History

### v1.0.0 (2026)
- Initial release
- Full Super Bassman emulation
- VST3 and Standalone support
- 8 factory presets
- Comprehensive documentation