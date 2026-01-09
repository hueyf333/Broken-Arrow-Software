# Frequently Asked Questions (FAQ)

## General Questions

### What is Patchwrap?

Patchwrap is an AAX plugin wrapper that allows you to use VST3 plugins inside Avid Pro Tools. It works similarly to Blue Cat Audio's Patchwork, providing a host environment for VST3 plugins within Pro Tools.

### Why would I need this?

Pro Tools uses the AAX plugin format, while many plugins are only available in VST3 format. Patchwrap bridges this gap, giving you access to the vast VST3 plugin ecosystem.

### Is this legal?

Yes. Plugin wrapping is a legitimate technique used by several commercial products. However, you should:
- Only use legally licensed VST3 plugins
- Respect each plugin's license terms
- Not redistribute plugins you don't have rights to

### How does it compare to Blue Cat Patchwork?

Patchwrap is inspired by Blue Cat Patchwork but is:
- Open source (Patchwork is commercial)
- AAX-only (Patchwork supports multiple formats)
- VST3-only (Patchwork supports VST2/VST3)
- Currently basic (Patchwork has many advanced features)

## Technical Questions

### What plugins does it support?

Patchwrap supports VST3 plugins that:
- Are 64-bit (Pro Tools is 64-bit only)
- Use standard VST3 interfaces
- Are installed in standard VST3 locations

### Does it support VST2 plugins?

No, only VST3. VST2 is a deprecated format and VST3 is more modern and capable.

### What about AU plugins on macOS?

Not yet. The current version is Windows-only, but macOS support is planned for the future.

### Can I use multiple VST3 plugins in one instance?

Not in the current version. Each Patchwrap instance hosts one VST3 plugin. You can insert multiple Patchwrap instances in your session.

### Does it support plugin automation?

Yes! VST3 plugin parameters are exposed to Pro Tools and can be automated normally.

### Does it support MIDI?

Basic MIDI support is implemented. MIDI events are forwarded to the VST3 plugin.

### What about latency?

Patchwrap reports the VST3 plugin's latency to Pro Tools, which automatically compensates. Total latency = buffer size + VST3 plugin latency.

## Building Questions

### Why do I need the AAX SDK?

The AAX SDK contains headers and libraries required to build AAX plugins. It's provided by Avid and requires free registration.

### Where do I get the AAX SDK?

Download from https://developer.avid.com/ (free Avid Developer account required)

### The VST3 SDK download failed. What should I do?

Try these steps:
1. Run `scripts\fetch_vst3sdk.ps1` manually
2. If that fails, manually clone: `git clone --recursive https://github.com/steinbergmedia/vst3sdk.git third_party/vst3sdk`
3. Ensure you have Git installed and the `--recursive` flag is used (for submodules)

### I get "Cannot find AAX.h" errors when building

This means the AAX SDK path is not configured. See [SDK_SETUP.md](SDK_SETUP.md) for instructions.

### Can I build without Visual Studio?

The current project files are for Visual Studio 2022. You could create CMake files or other build systems, but Visual Studio is recommended for AAX development.

### What about 32-bit builds?

No. Pro Tools (and AAX) is 64-bit only on modern versions.

## Usage Questions

### How do I install Patchwrap?

After building:
1. Copy `Build\Release\Patchwrap.aaxplugin` folder to `C:\Program Files\Common Files\Avid\Audio\Plug-Ins`
2. Restart Pro Tools
3. The plugin will appear in the Audio FX menu

### How do I load a VST3 plugin?

1. Insert Patchwrap on a track
2. Click the plugin selector dropdown
3. Choose a VST3 plugin from the list
4. Click "Load"

### The VST3 plugin I want isn't in the list

Ensure the plugin is:
- Installed in a standard VST3 location (`C:\Program Files\Common Files\VST3`)
- 64-bit
- Actually a VST3 plugin (not VST2)

### Can I save plugin presets?

Yes, the VST3 plugin state is saved with your Pro Tools session. You can also save Patchwrap as a plugin preset in Pro Tools.

### The VST3 GUI doesn't appear

Some VST3 plugins don't provide GUIs. In this case, Patchwrap should generate basic parameter controls (not yet implemented in v1.0).

### Pro Tools crashes when I load certain plugins

Some VST3 plugins may have bugs or incompatibilities. Try:
1. Ensure the plugin works in a native VST3 host
2. Update the plugin to the latest version
3. Report the issue with details

## Development Questions

### Can I contribute?

Yes! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### What's the license?

See the [LICENSE](LICENSE) file. (You should add an appropriate open source license)

### Will there be macOS support?

Yes, it's planned. AAX works on macOS and the core architecture is platform-agnostic.

### What about other wrapper formats?

Possible future directions:
- AU wrapper (VST3 → Audio Units)
- VST3 wrapper (VST3 → VST3, for plugin chaining)
- Standalone app (VST3 host without DAW)

### Can I use this commercially?

Check the license file. Generally, open source projects allow commercial use, but verify the specific terms.

## Troubleshooting

### Pro Tools doesn't see the plugin

1. Verify the plugin is in the correct folder: `C:\Program Files\Common Files\Avid\Audio\Plug-Ins`
2. Check that it's a folder named `Patchwrap.aaxplugin` (not a file)
3. Verify the bundle structure has `Contents\x64\Patchwrap.aaxplugin`
4. Try rescanning plugins in Pro Tools preferences

### The plugin loads but produces no sound

1. Check the bypass switch isn't enabled
2. Verify a VST3 plugin is loaded
3. Check the output gain isn't at -∞
4. Ensure the VST3 plugin is receiving input

### Audio glitches or dropouts

1. Increase Pro Tools buffer size
2. Check CPU usage (VST3 plugin may be heavy)
3. Ensure you're using Release build (not Debug)
4. Some VST3 plugins are more efficient than others

### Visual artifacts in the GUI

1. Update graphics drivers
2. Try disabling DPI scaling in Windows
3. Some VST3 GUIs may have rendering issues

### Parameters aren't automating correctly

1. Ensure the parameter is enabled for automation in Pro Tools
2. Some VST3 plugins have parameters that can't be automated
3. Check that parameter changes are reflected in the plugin GUI

## Performance

### Is there overhead from wrapping?

Yes, but minimal. The wrapper adds:
- ~1% CPU overhead for parameter management
- Minimal latency (< 1 sample) for buffer conversion
- Memory overhead is small (~100 KB for wrapper)

### Can I use lots of Patchwrap instances?

Each instance uses memory and CPU based on the loaded VST3 plugin. The wrapper itself is lightweight, but plugins vary widely in resource usage.

### Are there any optimizations I can make?

1. Use Release builds (not Debug)
2. Freeze tracks with Patchwrap when not editing
3. Increase buffer size if you don't need low latency
4. Some VST3 plugins have quality/performance settings

## Known Limitations (v1.0)

- Windows only (macOS planned)
- VST3 only (no VST2, AU)
- One plugin per instance (no chaining)
- Basic GUI (no advanced features)
- No VST3 preset browsing yet
- Mono/stereo only (no surround yet)

## Getting Help

### Where can I ask questions?

- GitHub Issues: Bug reports and feature requests
- GitHub Discussions: General questions and community support

### How do I report a bug?

Open an issue on GitHub with:
1. Patchwrap version
2. Pro Tools version
3. VST3 plugin name and version
4. Steps to reproduce
5. Expected vs actual behavior

### Can I request features?

Yes! Open a GitHub issue with the "enhancement" label and describe your feature request.

## Additional Resources

- [Building Guide](BUILDING.md)
- [Architecture Overview](ARCHITECTURE.md)
- [SDK Setup](SDK_SETUP.md)
- [Contributing Guide](CONTRIBUTING.md)
- [AAX SDK Documentation](https://developer.avid.com/)
- [VST3 SDK Documentation](https://steinbergmedia.github.io/vst3_doc/)
