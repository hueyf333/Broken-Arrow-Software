# Broken Arrow M900

A production-ready JUCE C++20 audio plugin modeling a Marshall JCM900-style guitar amplifier.

## Features

### DSP Processing Chain
- **Input Stage**: Input trim control and optional noise gate with adjustable threshold
- **Preamp**: Cascaded non-linear tube-like stages with three modes (Clean, Crunch, High Gain)
- **Tone Stack**: Interactive Marshall-style Bass/Mid/Treble/Presence controls
- **Power Amp**: Tube saturation, dynamic compression, and sag control (Authentic/Tight modes)
- **Cabinet Simulator**: Built-in IR loader with 3 factory impulse responses:
  - 4x12 Vintage (darker, midrange-focused)
  - 4x12 Modern (brighter, tighter)
  - 2x12 Combo (open, less bass)
- **Oversampling**: Selectable 1x/2x/4x with click-free switching
- **Noise Generator**: Optional subtle hiss and hum simulation (default off)
- **Output Stage**: Master volume and soft-clip safety limiter

### User Interface
- JCM900-styled front panel with authentic amp controls
- Real-time input/output meters with clip indicator
- Smooth parameter changes with no pops or clicks
- Parameter smoothing throughout the signal chain
- Zero denormal handling

### Plugin Formats
- Standalone application
- VST3
- Audio Unit (AU)
- AAX (with proper SDK integration - see below)

## Build Requirements

### Software Dependencies
- **JUCE Framework**: Version 7.0.5 or later
  - Download from: https://juce.com/download/
  - Extract to a location like `C:\JUCE` (Windows) or `~/JUCE` (macOS/Linux)
  
- **C++20 Compiler**:
  - Windows: Visual Studio 2022 (17.0 or later)
  - macOS: Xcode 14 or later
  - Linux: GCC 11+ or Clang 14+

### Optional: AAX SDK
For AAX plugin builds, you need the AAX SDK from Avid:
1. Sign up for an Avid developer account at https://www.avid.com/alliance-partner-program
2. Download the AAX SDK
3. Extract to a location like `C:\SDKs\AAX` or `~/SDKs/AAX`

## Build Instructions

### Step 1: Clone the Repository
```bash
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
```

### Step 2: Configure JUCE Module Paths

Open `Broken Arrow M900.jucer` in Projucer (JUCE's project manager).

1. Launch Projucer from your JUCE installation
2. Open `Broken Arrow M900.jucer`
3. For each exporter (Visual Studio 2022, Xcode, Linux Makefile):
   - Click on the exporter name
   - Update the module paths to point to your JUCE installation:
     - Change `../../JUCE/modules` to your actual JUCE modules path
     - Example: `C:/JUCE/modules` or `/Users/username/JUCE/modules`

### Step 3: (Optional) Configure AAX SDK Path

If building AAX:
1. In Projucer, go to File → Global Paths
2. Set "AAX SDK Path" to your AAX SDK installation directory
3. Save the settings

### Step 4: Generate Project Files

In Projucer, click "Save Project and Open in IDE" to generate build files.

### Step 5: Build the Plugin

#### Windows (Visual Studio 2022)
```bash
# Open the generated solution
Builds/VisualStudio2022/Broken Arrow M900.sln

# Build from Visual Studio IDE:
# - Select configuration (Debug or Release)
# - Build → Build Solution (F7)

# Or build from command line:
msbuild "Builds/VisualStudio2022/Broken Arrow M900.sln" /p:Configuration=Release
```

#### macOS (Xcode)
```bash
# Open the generated Xcode project
open "Builds/MacOSX/Broken Arrow M900.xcodeproj"

# Build from Xcode IDE or command line:
xcodebuild -project "Builds/MacOSX/Broken Arrow M900.xcodeproj" -configuration Release
```

#### Linux (Makefile)
```bash
cd Builds/LinuxMakefile
make CONFIG=Release
```

### Build Output Locations

After successful build, plugin binaries will be in:

- **Windows**:
  - VST3: `Builds/VisualStudio2022/x64/Release/VST3/Broken Arrow M900.vst3`
  - Standalone: `Builds/VisualStudio2022/x64/Release/Standalone/Broken Arrow M900.exe`
  - AAX: `Builds/VisualStudio2022/x64/Release/AAX/Broken Arrow M900.aaxplugin`

- **macOS**:
  - VST3: `Builds/MacOSX/build/Release/Broken Arrow M900.vst3`
  - AU: `Builds/MacOSX/build/Release/Broken Arrow M900.component`
  - Standalone: `Builds/MacOSX/build/Release/Broken Arrow M900.app`
  - AAX: `Builds/MacOSX/build/Release/Broken Arrow M900.aaxplugin`

- **Linux**:
  - VST3: `Builds/LinuxMakefile/build/Broken Arrow M900.vst3`
  - Standalone: `Builds/LinuxMakefile/build/Broken Arrow M900`

## Installation

### Windows
1. Copy `.vst3` file to: `C:\Program Files\Common Files\VST3\`
2. Copy `.aaxplugin` to: `C:\Program Files\Common Files\Avid\Audio\Plug-Ins\`

### macOS
1. Copy `.vst3` to: `/Library/Audio/Plug-Ins/VST3/`
2. Copy `.component` to: `/Library/Audio/Plug-Ins/Components/`
3. Copy `.aaxplugin` to: `/Library/Application Support/Avid/Audio/Plug-Ins/`

### Linux
1. Copy `.vst3` to: `~/.vst3/` or `/usr/lib/vst3/`

## AAX Integration Details

### Prerequisites for AAX
1. **AAX SDK**: Download from Avid Developer site
2. **PACE iLok**: Required for code signing AAX plugins
3. **Developer Account**: Avid Alliance Partner Program membership

### AAX Build Configuration
The project is pre-configured with AAX wrapper hooks. To complete AAX integration:

1. **Install AAX SDK**:
   ```
   Download AAX SDK from Avid
   Extract to: C:\SDKs\AAX (Windows) or ~/SDKs/AAX (macOS)
   ```

2. **Configure in Projucer**:
   - Open global paths (File → Global Paths)
   - Set AAX SDK path to your installation directory
   - Resave the project

3. **Code Signing** (Required for AAX):
   - macOS: Use `codesign` with your developer certificate
   - Windows: Use PACE Eden or Avid's signing tools
   
   Example (macOS):
   ```bash
   codesign --force --sign "Developer ID Application: Your Name" \
            --timestamp "Broken Arrow M900.aaxplugin"
   ```

4. **AAX Testing**:
   - Use Avid's ProTools or AAX Host for testing
   - Unsigned AAX plugins will not load in release ProTools

### AAX Category Configuration
The plugin is configured as:
- Category: Effect (AAX Category 0)
- Manufacturer: Broken Arrow Software (code: BrAr)
- Plugin ID: M900

## Testing and Validation

### DSP Test Harness
The project includes an offline DSP test harness for validation. To run tests:

1. Enable test harness in your build (add `#define RUN_DSP_TESTS 1` before including PluginProcessor.h)
2. Tests will run on plugin initialization
3. Check console output for test results

Tests validate:
- Stability (no NaN, Inf, or excessive values)
- Proper signal levels through each stage
- All parameter ranges
- Full processing chain integrity

### Real-time Safety
The plugin is designed for real-time safety:
- All allocations happen in `prepareToPlay()`
- No allocations on the audio thread
- Lock-free parameter updates using atomic values
- Pre-allocated buffers for oversampling
- Denormal handling with `ScopedNoDenormals`

## Project Structure

```
Broken-Arrow-Software/
├── Source/
│   ├── PluginProcessor.h/cpp     # Main audio processor
│   ├── PluginEditor.h/cpp        # GUI implementation
│   └── DSP/
│       ├── InputStage.h/cpp      # Input trim and gate
│       ├── PreampStage.h/cpp     # Tube preamp simulation
│       ├── ToneStack.h/cpp       # Marshall-style EQ
│       ├── PowerAmp.h/cpp        # Power amp saturation & sag
│       ├── CabinetSimulator.h/cpp # IR-based cab sim
│       ├── NoiseGenerator.h/cpp   # Noise/hum generator
│       ├── OutputStage.h/cpp      # Output limiter
│       └── DSPTestHarness.h/cpp   # Test framework
├── Resources/
│   └── IRs/                      # (Placeholder for user IRs)
├── Builds/                       # Generated build files
├── Broken Arrow M900.jucer       # JUCE project file
├── LICENSE                       # License file
└── README.md                     # This file
```

## Parameter Reference

### Input
- **Input Trim**: -12 to +12 dB (adjusts input level)
- **Gate**: On/Off toggle
- **Gate Threshold**: -60 to 0 dB (when gate is enabled)

### Preamp
- **Gain**: 0-10 (preamp drive amount)
- **Mode**: Clean / Crunch / High Gain

### Tone Stack
- **Bass**: 0-10 (±15 dB at 100 Hz)
- **Mid**: 0-10 (±15 dB at 650 Hz)
- **Treble**: 0-10 (±15 dB at 3 kHz)
- **Presence**: 0-10 (±10 dB at 5 kHz)

### Power Amp
- **Master**: 0-10 (power amp drive)
- **Sag Mode**: Authentic / Tight

### Cabinet
- **Cab**: On/Off toggle
- **IR Selection**: 4x12 Vintage / 4x12 Modern / 2x12 Combo

### Global
- **Output**: -12 to +12 dB (final output level)
- **Oversampling**: 1x / 2x / 4x
- **Noise**: On/Off toggle
- **Noise Level**: 0-1 (when noise is enabled)

## Troubleshooting

### Build Issues

**"Cannot find JUCE modules"**
- Verify JUCE installation path in Projucer
- Check module paths in each exporter configuration
- Ensure JUCE version 7.0.5 or later is installed

**"C++20 features not supported"**
- Update to Visual Studio 2022 (Windows)
- Update to Xcode 14+ (macOS)
- Update to GCC 11+ or Clang 14+ (Linux)

**AAX build fails**
- Ensure AAX SDK is installed and path is configured
- Check that you have the correct AAX SDK version
- Verify Visual Studio or Xcode supports AAX target

### Runtime Issues

**Plugin doesn't load**
- Check that host supports the plugin format
- Verify plugin is in correct directory
- Check console for error messages
- For AAX: ensure plugin is properly code-signed

**No sound output**
- Check input/output meters in the UI
- Verify Cab is enabled (or disable for direct sound)
- Check host routing and levels
- Ensure input signal is present

**High CPU usage**
- Reduce oversampling (try 1x or 2x instead of 4x)
- Disable cabinet simulator if not needed
- Use Tight sag mode instead of Authentic

## License

Copyright (c) 2026 Broken Arrow Software

See LICENSE file for full license terms.

## Support

For questions, issues, or feature requests:
- Email: support@brokenarrowsoftware.com
- Website: https://brokenarrowsoftware.com

## Credits

Developed by Broken Arrow Software using the JUCE framework.

JUCE is available at: https://juce.com/