# Quick Start Guide

## For Users

### Installation

1. **Download** the plugin for your platform
2. **Install** to your plugin directory:
   - Windows VST3: `C:\Program Files\Common Files\VST3\`
   - macOS VST3: `/Library/Audio/Plug-Ins/VST3/`
   - macOS AU: `/Library/Audio/Plug-Ins/Components/`
3. **Rescan** plugins in your DAW
4. **Load** "Broken Arrow M900" in an insert slot

### First Sound

1. **Insert** on a guitar track
2. **Set Mode** to "Crunch"
3. **Adjust Preamp Gain** to 6-7
4. **Set Master** to 5-6
5. **Enable Cab** (should be on by default)
6. **Play** and listen!

### Recommended Settings

**Clean Rhythm**:
- Mode: Clean
- Gain: 3-4
- Bass: 5, Mid: 5, Treble: 6
- Master: 5-6
- Cab: 4x12 Vintage

**Crunch Lead**:
- Mode: Crunch
- Gain: 7-8
- Bass: 6, Mid: 6, Treble: 7, Presence: 7
- Master: 6-7
- Cab: 4x12 Modern

**High Gain**:
- Mode: High Gain
- Gain: 8-9
- Bass: 7, Mid: 4, Treble: 7, Presence: 8
- Master: 7
- Sag: Tight
- Cab: 4x12 Modern

## For Developers

### Build from Source

```bash
# Clone
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Open in Projucer
# - Set JUCE module paths
# - Save and generate

# Build (example for macOS)
cd Builds/MacOSX
xcodebuild -configuration Release
```

### Run Tests

```cpp
// In main()
#include "DSP/DSPTestHarness.h"

DSPTestHarness harness;
harness.runAllTests();
```

### Add a Parameter

1. Add to `createParameterLayout()` in PluginProcessor.cpp
2. Add UI control in PluginEditor.cpp
3. Create attachment
4. Update DSP module

See CONTRIBUTING.md for detailed guidelines.
