# Testing and Validation Guide

## DSP Test Harness

The Broken Arrow M900 plugin includes a comprehensive offline DSP test harness for validating the audio processing chain.

### Running Tests

The test harness can be invoked programmatically. To enable it during development:

#### Option 1: Command-line Test Application

Create a simple standalone test application:

```cpp
// File: TestRunner.cpp
#include "DSP/DSPTestHarness.h"

int main()
{
    DSPTestHarness harness;
    bool allTestsPassed = harness.runAllTests();
    return allTestsPassed ? 0 : 1;
}
```

#### Option 2: Run on Plugin Initialization

Add to PluginProcessor constructor:

```cpp
#if JUCE_DEBUG
    DSPTestHarness testHarness;
    testHarness.runAllTests();
#endif
```

### Test Coverage

The test harness validates:

1. **Input Stage**
   - Trim control functionality
   - Noise gate operation
   - Signal stability
   - No NaN or Inf values

2. **Preamp Stage**
   - All three modes (Clean, Crunch, High Gain)
   - Gain control range
   - Cascaded distortion stability
   - DC offset blocking

3. **Tone Stack**
   - Bass/Mid/Treble/Presence controls
   - Filter stability
   - Interactive EQ behavior
   - No excessive phase shifts

4. **Power Amp**
   - Saturation characteristics
   - Dynamic compression
   - Both sag modes
   - Output level control

5. **Cabinet Simulator**
   - All factory IR selections
   - Convolution stability
   - Latency compensation
   - No artifacts

6. **Noise Generator**
   - Pink noise generation
   - Hum simulation
   - Level control
   - Proper mixing with signal

7. **Output Stage**
   - Gain control
   - Soft-clip limiter
   - No digital clipping
   - Output level compliance

8. **Full Chain Integration**
   - End-to-end signal flow
   - No cumulative instabilities
   - Proper gain staging
   - Real-world parameter settings

### Test Results

All tests check for:
- **Stability**: No NaN, Inf, or excessive values (>100.0)
- **Level compliance**: Output within expected ranges
- **Processing integrity**: Signal maintains quality through chain

Success output:
```
=== Starting DSP Test Harness ===

--- Testing Input Stage ---
Input stage test passed

--- Testing Preamp Stage ---
Preamp stage test passed

...

--- Testing Full DSP Chain ---
Full chain test passed

=== ALL TESTS PASSED ===
```

### Manual Testing Checklist

Use this checklist when testing the plugin manually:

#### Functional Tests

- [ ] Plugin loads in host (DAW)
- [ ] All parameters respond to changes
- [ ] Preset system saves/restores state
- [ ] Input/output meters display correctly
- [ ] Clip indicator triggers appropriately
- [ ] All three preamp modes sound different
- [ ] Tone stack affects frequency response
- [ ] Cabinet simulator changes tone when enabled/disabled
- [ ] All three IR selections sound different
- [ ] Oversampling modes switch without clicks
- [ ] Noise generator adds audible noise when enabled
- [ ] Gate effectively cuts low-level signal
- [ ] No pops or clicks on parameter changes
- [ ] UI updates smoothly

#### Audio Quality Tests

- [ ] Clean mode sounds clear at low gain
- [ ] Crunch mode has tight, focused distortion
- [ ] High Gain mode is aggressive but controlled
- [ ] Bass control affects low frequencies
- [ ] Mid control affects midrange clarity
- [ ] Treble control affects high frequencies
- [ ] Presence adds air and sparkle
- [ ] Power amp sag creates dynamic response
- [ ] Cabinet simulator sounds realistic
- [ ] No digital artifacts or aliasing
- [ ] Output limiter prevents harsh clipping
- [ ] Overall tone is "Marshall-like"

#### Performance Tests

- [ ] CPU usage acceptable at 1x oversampling
- [ ] CPU usage acceptable at 2x oversampling
- [ ] CPU usage acceptable at 4x oversampling
- [ ] No audio dropouts or glitches
- [ ] Latency reported correctly
- [ ] Multi-instance capability works
- [ ] Works at 44.1 kHz sample rate
- [ ] Works at 48 kHz sample rate
- [ ] Works at 88.2 kHz sample rate
- [ ] Works at 96 kHz sample rate

#### Compatibility Tests

- [ ] VST3 loads in multiple hosts
- [ ] AU loads in Logic/GarageBand (macOS)
- [ ] AAX loads in ProTools
- [ ] Standalone app launches and runs
- [ ] Automation works correctly
- [ ] MIDI control mapping works
- [ ] Session recall maintains settings
- [ ] No crashes with extreme parameters

### Performance Benchmarks

Expected CPU usage (on modern CPU, 48 kHz, 512 buffer):

- **1x oversampling**: 5-8%
- **2x oversampling**: 10-15%
- **4x oversampling**: 20-30%
- **Cab simulator off**: -3-5%

Latency:
- **1x oversampling**: 0 samples
- **2x oversampling**: ~10-20 samples
- **4x oversampling**: ~20-40 samples

### Known Limitations

Current implementation:
- User IR loading requires code modification (UI not yet implemented)
- Preset browser basic (relies on host preset management)
- No visual spectrum analyzer
- No A/B comparison feature
- Meters update at 30 Hz (fixed)

### Reporting Issues

When reporting issues, include:
1. Plugin version and build date
2. Host DAW name and version
3. Operating system and version
4. Sample rate and buffer size
5. Steps to reproduce
6. Expected vs actual behavior
7. Console/log output if available

### Future Test Additions

Planned test expansions:
- Frequency response analysis
- THD (Total Harmonic Distortion) measurements
- SNR (Signal-to-Noise Ratio) testing
- Impulse response verification
- Phase response validation
- Automation parameter stress testing
- Multi-threading safety verification
- Long-duration stability testing
