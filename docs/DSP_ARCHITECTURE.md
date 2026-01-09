# DSP Architecture Documentation

## Overview

The Broken Arrow M900 audio plugin uses a modular DSP architecture with dedicated processing stages that can be developed and tested independently.

## Signal Flow

```
Input Signal
    ↓
[Input Stage] ← Input Trim, Noise Gate
    ↓
[Preamp Stage] ← Gain, Mode (Clean/Crunch/High Gain)
    ↓
[Tone Stack] ← Bass, Mid, Treble, Presence
    ↓
[Power Amp] ← Master, Sag Mode
    ↓
[Noise Generator] ← Noise Level (if enabled)
    ↓
[Cabinet Simulator] ← IR Selection (if enabled)
    ↓
[Output Stage] ← Output Gain, Soft Limiter
    ↓
Output Signal
```

## Processing Stages

### Input Stage
- Input trim: -12 to +12 dB
- Noise gate with envelope follower
- 1ms attack, 100ms release

### Preamp Stage
- Three modes: Clean, Crunch, High Gain
- Cascaded non-linear stages (3 stages)
- Pre-emphasis at 720 Hz
- DC blocking at 10 Hz

### Tone Stack
- Bass: 100 Hz low shelf (±15 dB)
- Mid: 650 Hz parametric bell (±15 dB)
- Treble: 3 kHz high shelf (±15 dB)
- Presence: 5 kHz high shelf (±10 dB)

### Power Amp
- Saturation with asymmetric clipping
- Dynamic compression
- Sag simulation (Authentic/Tight)

### Cabinet Simulator
- Partitioned FFT convolution
- Three factory IRs
- User IR loading support

### Output Stage
- Output gain control
- Soft-clip safety limiter

## Real-time Safety

- No allocations on audio thread
- Preallocated buffers
- Lock-free parameter updates
- Denormal handling

## Performance

Typical CPU usage (48 kHz, 512 buffer):
- 1x oversampling: 5-8%
- 2x oversampling: 10-15%
- 4x oversampling: 20-30%

For detailed architecture documentation, see full DSP_ARCHITECTURE.md file.
