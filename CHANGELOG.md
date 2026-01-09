# Changelog

All notable changes to Patchwrap will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Full VST3 SDK integration in VST3Host implementation
- Complete AAX SDK integration in AAX_Describe
- Windows GUI implementation with native controls
- VST3 editor embedding support
- Plugin scanning and enumeration
- macOS support
- VST3 preset management
- Surround sound configurations

## [1.0.0] - TBD

### Added
- Initial project structure
- Visual Studio 2022 solution and project files
- Patchwrap_Core static library for VST3 hosting
  - VST3Host class with plugin lifecycle management
  - ParameterBridge for bidirectional parameter synchronization
  - Lock-free queue implementation for real-time safety
  - StateManager for chunk-based state serialization
- Patchwrap_AAX plugin wrapper
  - AAX_Parameters class for parameter model
  - AAX_Algorithm with real-time audio processing callback
  - AAX_GUI for user interface
  - AAX_Describe for plugin registration
- Build system
  - PowerShell script for AAX bundle packaging
  - PowerShell script for VST3 SDK download
  - Proper .aaxplugin bundle structure generation
- Comprehensive documentation
  - BUILDING.md with complete build instructions
  - ARCHITECTURE.md with technical design details
  - SDK_SETUP.md for SDK configuration
  - CONTRIBUTING.md for contribution guidelines
  - FAQ.md for common questions
  - VST3 integration reference
  - README.md with project overview
- Configuration templates
  - User.props.example for AAX SDK path
  - .gitignore for build artifacts
  - .gitmodules for VST3 SDK submodule

### Architecture
- Two-layer design (Core library + AAX wrapper)
- Real-time safe audio processing
- Lock-free parameter communication
- Thread-safe state management
- DPI-aware GUI support
- Resizable window support
- Latency reporting
- MIDI event forwarding

### Requirements
- Visual Studio 2022
- Windows 10 or later (64-bit)
- AAX SDK from Avid
- VST3 SDK (auto-downloaded)
- Pro Tools 11 or later

### Known Limitations
- Windows only (macOS planned)
- VST3 only (no VST2)
- Mono/stereo configurations only
- Single plugin per instance
- Requires AAX SDK to build (stub implementations provided)
- Requires full VST3 SDK integration for production use

## Project Status

**Current Status**: Framework Complete, SDK Integration Pending

The project structure, architecture, and framework are complete. To use in production:

1. **AAX SDK Integration**: Replace stub implementations in AAX_Describe.cpp with actual AAX SDK calls
2. **VST3 SDK Integration**: Replace stub implementations in VST3Host.cpp with VST3 SDK calls (see VST3_Integration_Reference.h)
3. **GUI Implementation**: Implement Windows controls and VST3 editor embedding in AAX_GUI.cpp
4. **Testing**: Comprehensive testing with various VST3 plugins in Pro Tools

The framework is designed to make these integrations straightforward by providing clear separation of concerns and well-documented interfaces.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for how to contribute to this project.

## License

See [LICENSE](LICENSE) file for license information.

---

[Unreleased]: https://github.com/hueyf333/Broken-Arrow-Software/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/hueyf333/Broken-Arrow-Software/releases/tag/v1.0.0
