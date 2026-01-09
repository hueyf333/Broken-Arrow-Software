# Changelog

All notable changes to the Broken Arrow M900 plugin will be documented in this file.

## [1.0.0] - 2026-01-09

### Initial Release

#### Features
- Complete Marshall JCM900-style amp modeling
- Three preamp modes: Clean, Crunch, High Gain
- Interactive Marshall-style tone stack (Bass/Mid/Treble/Presence)
- Power amp with saturation and sag control (Authentic/Tight)
- Built-in cabinet simulator with 3 factory IRs
- Selectable oversampling (1x/2x/4x)
- Input trim and noise gate
- Optional noise generator for realism
- Output soft-clip safety limiter
- Real-time input/output meters with clip indicator
- JCM900-styled graphical interface
- Full plugin format support: VST3, AU, AAX, Standalone

#### Technical
- C++20 implementation
- JUCE 7.0.5 framework
- Real-time safe audio processing
- Parameter smoothing throughout
- Zero denormal handling
- AudioProcessorValueTreeState for parameter management
- Partitioned FFT convolution for cabinet simulation
- Comprehensive offline DSP test harness

#### Platforms
- Windows (Visual Studio 2022)
- macOS (Xcode 14+)
- Linux (GCC 11+/Clang 14+)

#### Known Issues
- User IR loading requires code modification (UI for file browser not yet implemented)
- Basic preset management (relies on host)

### Future Roadmap

#### v1.1.0 (Planned)
- [ ] User IR loader with file browser UI
- [ ] Extended preset browser with categorization
- [ ] Visual input/output spectrum analyzer
- [ ] A/B comparison feature
- [ ] MIDI CC mapping interface
- [ ] Undo/redo for parameter changes

#### v1.2.0 (Planned)
- [ ] Additional cabinet models
- [ ] Room/reverb simulation
- [ ] Microphone position control for IRs
- [ ] Advanced metering (RMS, peak hold, etc.)
- [ ] Skin/theme customization

#### v2.0.0 (Future)
- [ ] Multi-amp chaining
- [ ] Built-in effects (delay, reverb, modulation)
- [ ] Advanced power amp modeling options
- [ ] Transformer saturation simulation
- [ ] Speaker breakup simulation
