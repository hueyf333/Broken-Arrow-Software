# Ampeg SVT Bass Amp Plugin - Quick Reference

## Quick Start

```bash
# Clone with submodules
git clone --recursive https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Build (Linux/Mac)
./validate_build.sh

# Or manually:
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Controls Reference

### Input Section
| Control | Range | Default | Description |
|---------|-------|---------|-------------|
| Input Pad | -20 to 0 dB | 0 dB | Attenuate hot input signals |
| Input Trim | -12 to +12 dB | 0 dB | Fine-tune input gain |
| Gate Enable | On/Off | Off | Toggle noise gate |
| Gate Threshold | -80 to -20 dB | -60 dB | Gate opening threshold |

### Preamp Section
| Control | Range | Default | Description |
|---------|-------|---------|-------------|
| Drive | 0-100% | 30% | Tube preamp overdrive amount |
| Bias | -100 to +100% | 0% | Asymmetric clipping characteristic |

### Tone Stack
| Control | Range | Default | Description |
|---------|-------|---------|-------------|
| Bass | 0-100% | 50% | Low frequency (100 Hz shelf, ±12 dB) |
| Mid | 0-100% | 50% | Mid frequency (parametric peak, ±12 dB) |
| Treble | 0-100% | 50% | High frequency (3 kHz shelf, ±12 dB) |
| Mid Freq | 5 positions | 800 Hz | Select mid center: 220/450/800/1.6k/3k Hz |

### Voicing Switches
| Control | Description |
|---------|-------------|
| Bright | +6 dB high shelf at 5 kHz for added clarity |
| Ultra-Lo | +8 dB low shelf at 40 Hz for extended bass |
| Ultra-Hi | +6 dB high shelf at 8 kHz for enhanced presence |

### Power Amp Section
| Control | Range | Default | Description |
|---------|-------|---------|-------------|
| Drive | 0-100% | 50% | Power tube saturation and sag |
| Master | 0-100% | 70% | Output level control |

### Output Section
| Control | Range | Default | Description |
|---------|-------|---------|-------------|
| Cab Mix | 0-100% | 100% | Dry/Wet blend for cabinet simulation |
| Output Gain | -12 to +12 dB | 0 dB | Final output trim |
| Bypass | On/Off | Off | True bypass |

## Preset Recall

Use your DAW's preset browser or program changes 0-3:

| # | Preset | Description |
|---|--------|-------------|
| 0 | Clean | Transparent vintage SVT (low drive, balanced EQ) |
| 1 | Classic Grind | SVT overdrive character (medium drive, enhanced mids) |
| 2 | Pushed Power | Power amp saturation focus (low pre, high power) |
| 3 | Deep/Ultra-Lo | Extended low-end (clean + Ultra-Lo engaged) |

## Signal Flow

```
Input → Pad → DC Block → Gate → Trim
  ↓
Preamp Stage 1 → Clip → Filter (8 kHz)
  ↓
Preamp Stage 2 → Clip → Filter (6 kHz)
  ↓
Preamp Stage 3 → Clip → Filter (5 kHz)
  ↓
Bass EQ → Mid EQ → Treble EQ
  ↓
Bright → Ultra-Lo → Ultra-Hi (if enabled)
  ↓
Power Amp Sag → Saturation → Transformer Filter
  ↓
Master Volume
  ↓
Cabinet Simulation (Mono→Stereo)
  ↓
Output Gain → Limiter → Output
```

## CPU Optimization Tips

- Reduce buffer size in DAW for lower latency
- Disable cabinet simulation if not needed (set Cab Mix to 0%)
- Use fewer instances simultaneously
- Freeze/bounce tracks using the plugin

## Troubleshooting

### No audio output
1. Check Master and Output Gain levels
2. Ensure Bypass is OFF
3. Verify input routing (mono in, stereo out)

### Harsh/digital sound
1. Reduce Drive controls (Preamp and Power)
2. Increase buffer size in DAW
3. Check that oversampling is working (should be automatic)

### Plugin not loading
1. Verify correct installation path for your OS
2. Rescan plugins in DAW
3. Check DAW's plugin blacklist

### High CPU usage
- Expected: ~2-5% per instance on modern CPU
- If higher: Check sample rate, reduce buffer size

## File Locations

### VST3 Install Paths
- **Linux**: `~/.vst3/`
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/`

### Custom IR Files
Place `.wav` files in `Assets/` directory (mono or stereo, 44.1-96 kHz)

### Preset Files
Factory presets: `Presets/*.preset` (XML format)

## DAW Setup Example (Reaper)

1. Insert "Ampeg SVT Bass Amp" on bass track
2. Set track input to mono
3. Set track output to stereo
4. Load preset (e.g., "Classic Grind")
5. Adjust Drive and Master to taste
6. Record or process!

## Support

- Documentation: [README.md](README.md)
- Build guide: [INSTALL.md](INSTALL.md)
- Project details: [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)
- Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues
