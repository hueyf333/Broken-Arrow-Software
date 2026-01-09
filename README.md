# Patchwrap - AAX Wrapper for VST3 Plugins

A Blue Cat Patchwork-style AAX wrapper plugin that enables hosting VST3 plugins inside Avid Pro Tools.

## Overview

Patchwrap allows you to use VST3 plugins in Pro Tools by wrapping them in an AAX plugin shell. This provides Pro Tools users access to the vast ecosystem of VST3 plugins.

## Features

- **VST3 Plugin Hosting**: Load and run VST3 plugins inside Pro Tools
- **Full Parameter Support**: Bidirectional parameter synchronization with automation
- **State Management**: Save and recall complete plugin state in sessions and presets
- **Custom GUI**: Plugin selector, bypass, output gain, meters, and embedded VST3 editors
- **Real-Time Safe**: Lock-free audio processing optimized for low latency
- **MIDI Support**: Forward MIDI events to hosted VST3 plugins
- **Latency Reporting**: Automatic latency compensation
- **DPI Aware**: Scales properly on high-DPI displays

## Quick Start

1. Download and install the AAX SDK from [Avid Developer](https://developer.avid.com/)
2. Set `AAX_SDK_PATH` environment variable to your AAX SDK location
3. Run `scripts\fetch_vst3sdk.ps1` to download VST3 SDK
4. Open `Patchwrap.sln` in Visual Studio 2022
5. Build the solution (x64, Release)
6. Copy `Build\Release\Patchwrap.aaxplugin` to Pro Tools plugins folder

For detailed build instructions, see [BUILDING.md](BUILDING.md).

## Requirements

- Visual Studio 2022
- Windows 10 or later (64-bit)
- AAX SDK (free from Avid)
- VST3 SDK (auto-downloaded)

## Project Structure

- **Patchwrap_Core**: Static library for VST3 hosting and parameter bridging
- **Patchwrap_AAX**: AAX plugin wrapper for Pro Tools integration

## Building

See [BUILDING.md](BUILDING.md) for comprehensive build instructions.

## License

See [LICENSE](LICENSE) for details.

## Contributing

Issues and pull requests are welcome!

## Support

For questions and issues, please use the [GitHub Issues](https://github.com/hueyf333/Broken-Arrow-Software/issues) page.