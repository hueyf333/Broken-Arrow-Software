# Broken Arrow Skin Lab

A WinUI 3-based GUI application for creating, editing, and previewing layered rotary knobs and control skins, inspired by KnobMan and SkinMan.

## Features

### Core Capabilities
- **Layer Management**: Add, remove, reorder, and duplicate layers
- **Transform Controls**: Adjust position, scale, and rotation for each layer
- **Shape Tools**: Create circles, rectangles, rounded rectangles, and ellipses
- **Color & Gradients**: Full color picker and gradient editor support
- **Effects**: Apply shadows, reflections, and blend modes
- **Texture Import**: Import external images as texture layers
- **Animation Support**: Create multi-frame animations with frame scrubber
- **Live Preview**: Real-time canvas preview with zoom and pan controls
- **Export Options**: Export to PNG (single frame or sprite sheet)
- **Project Persistence**: Save and load projects in .basl format

### User Interface
- **Left Panel**: Layer list with visibility toggles and management controls
- **Center Panel**: Live preview canvas with zoom controls and frame scrubber
- **Right Panel**: Property editor for selected layer transforms and appearance
- **Menu Bar**: File, Edit, and View menus with keyboard shortcuts
- **Status Bar**: Display project info, canvas dimensions, and save status

## System Requirements

- **Operating System**: Windows 10 version 1809 (Build 17763) or later
- **Development Environment**: Visual Studio 2022 or later
- **.NET SDK**: .NET 8.0 or later
- **Windows App SDK**: 1.6 or later

## Building the Project

### Prerequisites

1. Install Visual Studio 2022 (or later) with the following workloads:
   - .NET Desktop Development
   - Universal Windows Platform development
   - Windows App SDK C# Templates

2. Install the Windows App SDK:
   ```powershell
   dotnet workload install microsoft-windowsapp-sdk
   ```

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. Open the solution in Visual Studio:
   ```
   BrokenArrowSkinLab.sln
   ```

3. Restore NuGet packages:
   - Visual Studio will automatically restore packages on first load
   - Or manually: Right-click solution → Restore NuGet Packages

4. Select your target platform (x64, x86, or ARM64)

5. Build the solution:
   - Press `F6` or select Build → Build Solution
   - Or from command line:
     ```powershell
     dotnet build BrokenArrowSkinLab.sln -c Release
     ```

### Running the Application

**Option 1: From Visual Studio**
- Press `F5` to run with debugging
- Or `Ctrl+F5` to run without debugging

**Option 2: From Command Line**
```powershell
dotnet run --project BrokenArrowSkinLab/BrokenArrowSkinLab.csproj
```

**Option 3: Run the Executable**
After building, the executable is located at:
```
BrokenArrowSkinLab\bin\<Platform>\<Configuration>\net8.0-windows10.0.19041.0\BrokenArrowSkinLab.exe
```

## Running Tests

Run the unit tests using:

**From Visual Studio:**
- Open Test Explorer: Test → Test Explorer
- Click "Run All Tests"

**From Command Line:**
```powershell
dotnet test BrokenArrowSkinLab.Tests/BrokenArrowSkinLab.Tests.csproj
```

## Usage Guide

### Creating a New Project
1. Launch Broken Arrow Skin Lab
2. File → New Project (Ctrl+N)
3. The default project includes a canvas and one background layer

### Working with Layers
- **Add Layer**: Click the "+" button in the Layers panel or Edit → Add Layer (Ctrl+L)
- **Remove Layer**: Select a layer and click the "-" button or press Delete
- **Duplicate Layer**: Select a layer and click the "Copy" button or Edit → Duplicate Layer (Ctrl+D)
- **Reorder Layers**: Use the Up/Down buttons to change layer order
- **Toggle Visibility**: Click the checkbox next to each layer name

### Editing Layer Properties
1. Select a layer from the Layers panel
2. Use the Properties panel (right side) to adjust:
   - **Position**: X and Y coordinates on the canvas
   - **Scale**: Uniform or non-uniform scaling
   - **Rotation**: Angle in degrees
   - **Opacity**: Transparency level (0-1)
   - **Shape Type**: Circle, Rectangle, Rounded Rectangle, or Ellipse
   - **Size**: Width and height dimensions

### Creating Animations
1. Add frames using the "Add Frame" button in the frame scrubber
2. Use the slider to navigate between frames
3. Edit layers independently for each frame
4. Export as a sprite sheet for use in applications

### Exporting Your Work

**Single Frame Export:**
- File → Export Single Frame...
- Choose the destination and filename
- Saves the current frame as a PNG image

**Sprite Sheet Export:**
- File → Export Sprite Sheet...
- Exports all frames in a grid layout
- Useful for game engines and animation systems

### Saving and Loading Projects
- **Save**: File → Save Project (Ctrl+S)
- **Save As**: File → Save Project As... (Ctrl+Shift+S)
- **Open**: File → Open Project... (Ctrl+O)
- Projects are saved with the `.basl` extension

## Project Structure

```
BrokenArrowSkinLab/
├── Models/              # Data models (Layer, SkinProject, etc.)
├── ViewModels/          # MVVM view models
├── Views/               # XAML views and windows
├── Services/            # Business logic services
├── Helpers/             # Utility classes and converters
├── Assets/              # Application assets
│   ├── Templates/       # Default project templates
│   └── Samples/         # Sample assets
└── Package.appxmanifest # Application manifest

BrokenArrowSkinLab.Tests/
└── *.cs                 # Unit tests
```

## Sample Templates

The application includes several pre-built templates in `Assets/Templates/`:

1. **basic-knob.basl**: A simple rotary knob with background, highlight, and pointer
2. **animated-knob.basl**: Template for creating 64-frame animated knobs

To use a template:
1. File → Open Project
2. Navigate to `BrokenArrowSkinLab/Assets/Templates/`
3. Select a template file

## Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| New Project | Ctrl+N |
| Open Project | Ctrl+O |
| Save Project | Ctrl+S |
| Save Project As | Ctrl+Shift+S |
| Add Layer | Ctrl+L |
| Remove Layer | Delete |
| Duplicate Layer | Ctrl+D |
| Zoom In | Ctrl++ |
| Zoom Out | Ctrl+- |
| Reset Zoom | Ctrl+0 |

## Technologies Used

- **WinUI 3**: Modern Windows UI framework
- **Windows App SDK**: Windows platform features
- **Win2D**: Hardware-accelerated 2D graphics
- **CommunityToolkit.Mvvm**: MVVM helpers and commands
- **System.Text.Json**: Project serialization
- **MSTest**: Unit testing framework

## Troubleshooting

### Build Errors

**Issue**: "Windows App SDK not found"
- **Solution**: Install the Windows App SDK workload:
  ```powershell
  dotnet workload install microsoft-windowsapp-sdk
  ```

**Issue**: "Unable to restore NuGet packages"
- **Solution**: Clear NuGet cache and restore:
  ```powershell
  dotnet nuget locals all --clear
  dotnet restore
  ```

### Runtime Errors

**Issue**: Application doesn't launch
- **Solution**: Ensure you're running on Windows 10 version 1809 or later
- Check that all NuGet packages are properly restored

**Issue**: Missing asset files
- **Solution**: Create placeholder PNG files in the `Assets/` directory with the required dimensions (see Assets/README.md)

## Known Limitations

1. **Platform Support**: Windows only (WinUI 3 is Windows-specific)
2. **Asset Files**: Placeholder assets need to be replaced with actual PNG files for packaging
3. **Texture Loading**: Texture layer support is implemented but requires file picker integration
4. **Gradient Rendering**: Gradient effects are defined in the model but require Win2D integration for rendering

## Contributing

This is a demonstration project. For production use, consider:
- Adding comprehensive error handling
- Implementing undo/redo functionality
- Adding more shape types and effects
- Enhancing the gradient editor with visual preview
- Adding brush and pen tools for freehand drawing
- Implementing layer groups and masking

## License

See the LICENSE file in the repository root.

## Contact

For issues or questions, please open an issue on the GitHub repository:
https://github.com/hueyf333/Broken-Arrow-Software

---

**Version**: 1.0.0  
**Last Updated**: January 2026