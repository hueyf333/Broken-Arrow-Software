# SuperBassman Architecture

This document describes the internal architecture and signal flow of the SuperBassman plugin.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                          SuperBassman Plugin                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  ┌──────────────────┐      ┌──────────────────┐                    │
│  │                  │      │                  │                    │
│  │  PluginEditor    │◄────►│ PluginProcessor  │                    │
│  │  (UI/GUI)        │      │  (Audio Engine)  │                    │
│  │                  │      │                  │                    │
│  └──────────────────┘      └────────┬─────────┘                    │
│                                     │                               │
│                                     │                               │
│                            ┌────────▼─────────┐                    │
│                            │                  │                    │
│                            │  APVTS           │                    │
│                            │  (Parameters)    │                    │
│                            │                  │                    │
│                            └──────────────────┘                    │
│                                                                       │
└─────────────────────────────────────────────────────────────────────┘
```

## Signal Flow

```
Audio Input (Mono)
    │
    ├──► InputStage
    │      • Input Gain/Pad
    │      • DC Blocker
    │      • High-Pass Filter
    │
    ├──► ChannelRouting
    │      • Normal/Bright/Overdrive selection
    │      • Channel-specific EQ
    │
    ├──► TubePreamp (2x Oversampled)
    │      • Multi-stage saturation
    │      • Bias control
    │      • Asymmetric clipping
    │
    ├──► ToneStack
    │      • Bass (100 Hz)
    │      • Mid (500 Hz)
    │      • Treble (3 kHz)
    │      • Presence (6 kHz)
    │
    ├──► OverdriveChannel (2x Oversampled, if selected)
    │      • Drive stage
    │      • Parallel blend
    │      • Tone shaping
    │
    ├──► PowerAmp (2x Oversampled)
    │      • Power supply sag
    │      • Compression
    │      • Power tube saturation
    │
    ├──► CabinetSim
    │      • 8x10/4x10/1x15 models
    │      • Wet/dry mix
    │
    ├──► OutputStage
    │      • Soft limiter
    │      • Output gain
    │      • Safety clipping
    │
    └──► Audio Output (Stereo)
```

## DSP Module Details

### 1. InputStage
**Purpose**: Condition input signal before processing

**Components**:
- Input gain with smoothing (-12 to +12 dB)
- -10 dB pad switch
- DC blocking high-pass filter (5 Hz)
- Optional high-pass filter (20-200 Hz)

**Implementation**:
```cpp
class InputStage {
    juce::dsp::ProcessorDuplicator<IIR::Filter> dcBlocker;
    juce::dsp::ProcessorDuplicator<IIR::Filter> highPassFilter;
    juce::SmoothedValue<float> inputGainSmooth;
};
```

### 2. ChannelRouting
**Purpose**: Select and apply channel-specific characteristics

**Channels**:
- **Normal**: Flat frequency response
- **Bright**: +3dB high-shelf @ 2 kHz
- **Overdrive**: Routes to high-gain path

**Implementation**:
```cpp
class ChannelRouting {
    juce::dsp::ProcessorDuplicator<IIR::Filter> brightFilter;
    int currentChannel;
};
```

### 3. TubePreamp
**Purpose**: Emulate tube preamp stages

**Features**:
- Multi-stage processing (3 stages)
- Asymmetric soft clipping
- Bias control for harmonic shaping
- Gain staging with compensation

**Saturation Function**:
```
f(x) = {
    x,                                    |x| < threshold
    sign(x) * (threshold + tanh(excess)), |x| ≥ threshold
}
```

**Implementation**:
```cpp
class TubePreamp {
    float tubeSaturation(float input, float asymmetry);
    SmoothedValue<float> gainSmooth, biasSmooth;
};
```

### 4. ToneStack
**Purpose**: Interactive EQ based on Fender topology

**Bands**:
- **Bass**: Low shelf @ 100 Hz (±10 dB)
- **Mid**: Peaking @ 500 Hz (±7.5 dB)
- **Treble**: High shelf @ 3 kHz (±10 dB)
- **Presence**: High shelf @ 6 kHz (±7.5 dB)

**Characteristics**:
- Interactive controls (non-parametric)
- Smooth filter coefficient updates
- Authentic mid scoop behavior

### 5. OverdriveChannel
**Purpose**: High-gain distortion stage

**Features**:
- Variable drive (1-50x gain)
- Hard clipping with soft knee
- Parallel blend (0-100%)
- Post-distortion tone control (500-15000 Hz LPF)

**Distortion Function**:
```
f(x) = {
    x * gain,                             |x*gain| < threshold
    sign(x) * smooth_clip(excess),        |x*gain| ≥ threshold
}
```

### 6. PowerAmp
**Purpose**: Emulate power amp characteristics

**Features**:
- **Sag**: Envelope-following gain reduction
  - Fast attack (0.01)
  - Slow release (0.001)
  - Reduces headroom during transients
  
- **Compression**: Soft-knee compression
  - Threshold: 0.5
  - Ratio: 1:1 to 4:1
  
- **Saturation**: Soft tanh clipping

**Sag Algorithm**:
```cpp
envelope = attack_if(|input| > envelope, release_otherwise)
sag_gain = 1.0 - (envelope * sag_amount * 0.3)
output = input * sag_gain
```

### 7. CabinetSim
**Purpose**: Speaker cabinet frequency response

**Models**:
- **8x10 Sealed**: Tight, focused, classic (LPF @ 5 kHz, Q=0.5)
- **4x10 Ported**: Modern, punchy (LPF @ 6 kHz, Q=0.707)
- **1x15 Sealed**: Vintage, warm (LPF @ 3 kHz, Q=0.4)

**Processing**:
- Frequency response modeling via IIR filters
- Wet/dry blend (0-100%)
- Zero latency

### 8. OutputStage
**Purpose**: Final gain and safety limiting

**Features**:
- Output gain (-24 to +24 dB)
- Soft limiter (threshold @ 0.9, gentle knee)
- Hard clip at ±1.0
- Denormal protection

## Oversampling Strategy

### Purpose
Reduce aliasing artifacts from nonlinear processing

### Implementation
```cpp
class OversamplingEngine {
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    // 2x oversampling with polyphase IIR filters
};
```

### Applied To
1. TubePreamp (asymmetric clipping)
2. OverdriveChannel (hard clipping)
3. PowerAmp (saturation and compression)

### Trade-offs
- **Benefits**: Cleaner high-frequency content, reduced aliasing
- **Cost**: ~2x CPU usage on oversampled stages
- **Latency**: Minimal (polyphase IIR has near-zero latency)

## Parameter System

### AudioProcessorValueTreeState
Central parameter management system:

```cpp
AudioProcessorValueTreeState apvts;
```

**Features**:
- Automatic parameter serialization
- Host automation support
- Thread-safe parameter access
- Undo/redo support (future)

### Parameter Smoothing
All continuous parameters use smoothing to prevent clicks:

```cpp
SmoothedValue<float> paramSmooth;
paramSmooth.reset(sampleRate, rampTime);
paramSmooth.setTargetValue(newValue);
float smoothed = paramSmooth.getNextValue();
```

**Ramp Times**:
- Gain controls: 50ms
- Tone controls: 50ms
- Channel switching: 20ms (crossfade)
- Bypass: Instant with safety gain ramping

## UI Architecture

### Component Hierarchy
```
PluginEditor
├── CustomLookAndFeel
├── Input Section
│   ├── inputGainSlider
│   ├── inputPadButton
│   └── hpfFreqSlider
├── Channel Section
│   └── channelSelector
├── Preamp Section
│   ├── preampGainSlider
│   └── preampBiasSlider
├── Tone Section
│   ├── bassSlider
│   ├── midSlider
│   ├── trebleSlider
│   └── presenceSlider
├── Overdrive Section
│   ├── driveSlider
│   ├── driveBlendSlider
│   └── driveToneSlider
├── Power Section
│   ├── masterVolumeSlider
│   └── powerSagSlider
├── Cabinet Section
│   ├── cabEnableButton
│   ├── cabTypeSelector
│   └── cabMixSlider
├── Output Section
│   ├── outputGainSlider
│   ├── limiterEnableButton
│   └── bypassButton
└── Metering
    ├── inputMeter
    └── outputMeter
```

### LevelMeter
Real-time audio level display:

```cpp
class LevelMeter : public Component, private Timer {
    std::atomic<float> currentLevel;
    std::atomic<bool> isClipping;
    float peakLevel;  // Peak hold
};
```

**Features**:
- 30 Hz update rate
- Color coding (green/yellow/orange/red)
- Peak hold (1 second)
- Clip indicator

### CustomLookAndFeel
Vintage amplifier aesthetic:

**Colors**:
- Primary accent: Orange (#FFA500)
- Background: Dark grey (#2A2A2A)
- Panel: Very dark grey (#1A1A1A)
- Text: Light grey (#D3D3D3)

**Knob Style**:
- Rotary with arc indicator
- Center pointer
- Value display below

## Performance Considerations

### CPU Optimization
1. **Denormal Protection**: ScopedNoDenormals in processBlock
2. **SIMD**: JUCE DSP uses SIMD when available
3. **Efficient Filtering**: Polyphase IIR for oversampling
4. **Selective Oversampling**: Only nonlinear stages
5. **Lazy Updates**: Filters updated only when parameters change

### Memory Layout
- Stack allocation for temporary buffers where possible
- SmoothedValue uses minimal memory overhead
- ProcessSpec shared across modules

### Threading
- Audio processing: Real-time thread
- UI updates: Timer-based (30 Hz for meters)
- Parameter changes: Atomic loads/stores

## State Management

### Save/Load
Parameters saved as XML via ValueTree:

```cpp
void getStateInformation(MemoryBlock& destData) {
    auto state = apvts.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}
```

### Preset Format
XML structure:
```xml
<Parameters>
    <PARAM id="inputGain" value="0.0"/>
    <PARAM id="channel" value="0.0"/>
    <!-- ... all parameters ... -->
</Parameters>
```

## Testing Strategy

### Unit Testing (Future)
- DSP module tests
- Parameter range validation
- State save/load integrity

### Integration Testing
- Manual DAW testing
- Preset loading verification
- Automation testing

### Performance Testing
- CPU usage profiling
- Memory leak detection
- Real-time safety verification

## Future Enhancements

### Planned Features
1. **IR Loading**: Convolution-based cabinet simulation
2. **MIDI Learn**: Map MIDI CC to parameters
3. **Preset Browser**: Built-in preset management
4. **A/B Comparison**: Quick setting comparison
5. **Spectrum Analyzer**: Visual frequency response
6. **Stereo Input**: Full stereo signal path
7. **Additional Cabinets**: More speaker models

### Architecture Extensions
- Plugin wrapper abstraction for multiple formats
- Modular DSP component system
- Advanced metering (RMS, LUFS)
- Sample-accurate automation

## References

### DSP Literature
- Tube modeling: "Virtual Analog Modeling" (Välimäki)
- Tone stacks: "Designing Audio Effect Plugins in C++" (Pirkle)
- Oversampling: "The Art of VA Filter Design" (Välimäki & Huovilainen)

### JUCE Resources
- JUCE Documentation: https://docs.juce.com/
- JUCE DSP Module: https://docs.juce.com/master/group__juce__dsp.html
- JUCE Forum: https://forum.juce.com/
