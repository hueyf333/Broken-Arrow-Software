# Changelog

All notable changes to the SuperBassman plugin will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-01-10

### Added
- Initial release of SuperBassman plugin
- Complete Fender Super Bassman Bass Head emulation
- VST3 plugin format support
- Standalone application
- Three channel modes (Normal, Bright, Overdrive)
- Full tube preamp modeling with bias control
- Interactive tone stack (Bass, Mid, Treble, Presence)
- Overdrive channel with drive, blend, and tone controls
- Power amp simulation with sag and compression
- Three cabinet simulations (8x10 Sealed, 4x10 Ported, 1x15 Sealed)
- Input stage with pad, DC blocker, and high-pass filter
- Output stage with limiter and gain control
- Real-time level metering with clip detection
- Parameter automation support
- State save/recall functionality
- 8 factory presets:
  - Classic Rock Bass
  - Modern Slap
  - Reggae Dub
  - Metal Grind
  - Jazz Warmth
  - Funk Punch
  - Clean DI
  - Vintage Thump
- Resizable GUI (600x400 to 1800x1200)
- Custom vintage-inspired UI with orange accent colors
- 2x oversampling on nonlinear stages
- Parameter smoothing for click-free operation
- Denormal protection for CPU efficiency
- Support for 44.1, 48, and 96 kHz sample rates
- Mono input to stereo output
- CMake build system
- Comprehensive documentation:
  - README with research summary and control reference
  - BUILDING guide for all platforms
  - Detailed DSP architecture documentation
- Build script for Linux/macOS
- JUCE 7.0.12 integration
- C++20 codebase

### Technical Details
- Multi-stage asymmetric tube saturation
- Interactive Fender-style tone stack
- Envelope-following power supply sag
- Soft-knee compression
- Cabinet frequency response modeling
- Efficient DSP processing

### Platform Support
- Linux (tested on Ubuntu 24.04)
- macOS (11.0+)
- Windows (10+)

### Build Systems
- CMake 3.15+
- Projucer (.jucer project included)
- Visual Studio 2022
- Xcode 13+
- Linux Makefile

## [Unreleased]

### Planned Features
- Impulse response loading for cabinets
- Additional presets
- MIDI learn functionality
- Preset browser
- A/B comparison
- Undo/redo
- Additional cabinet types
- Stereo input option
- AAX plugin format
- AU plugin format (macOS)

### Known Issues
- None reported yet

---

## Version History

- **1.0.0** (2026-01-10) - Initial release

---

## Upgrade Notes

### From Pre-release to 1.0.0
- First official release
- No upgrade path required

---

## Support

For bug reports and feature requests, please visit:
https://github.com/hueyf333/Broken-Arrow-Software/issues
