# BrokenArrow SkinLab

A comprehensive WinUI 3-based GUI skinning application modeled after KnobMan and SkinMan for creating professional audio plugin interfaces and control skins.

## Features

### Knob Designer Workspace
- Generate knob filmstrips and frame sequences with configurable layers
- Support for multiple layer types:
  - Background with gradients
  - Ring overlays
  - Tick marks
  - Animated pointer
  - Gloss effects
  - Drop shadows
  - Text overlays
- Per-layer properties:
  - Color and gradient controls
  - Opacity settings
  - Blur effects
  - Blend modes (Normal, Multiply, Screen)
- Animation across N frames from configurable start/end angles
- Interactive preview with real-time rendering
- Export options:
  - PNG filmstrip (vertical/horizontal)
  - PNG sequence (numbered frames)
  - Texture atlas with JSON metadata

### Skin Layout Designer Workspace
- Visual canvas-based layout editor
- Drag-and-drop control placement
- Supported control types:
  - Rotary knobs
  - Horizontal/Vertical sliders
  - Toggle/Momentary buttons
  - Multi-state switches
  - LED meters
- Advanced editing features:
  - Resize handles
  - Grid snapping
  - Alignment guides
  - Zoom and pan
  - Background image support
- Properties panel for detailed configuration
- Export to JSON/CSS skin descriptors with scale factors (1x, 2x)

### Application Features
- Full MVVM architecture for maintainability
- Undo/Redo support
- Copy/Cut/Paste/Duplicate operations
- Multi-select capability
- Autosave functionality
- Recent files list
- Project explorer with document tabs
- Status bar with real-time feedback
- Comprehensive menu system (File, Edit, View, Tools, Help)

### File Formats
- `.baslproj` - Complete project files (JSON)
- `.baslknob` - Knob preset files (JSON)
- `.basllayout` - Layout template files (JSON)
- Import: PNG, JPG, BMP images
- Export: PNG filmstrips, sequences, atlases, JSON metadata

### Quality Assurance
- Built-in self-test tool (Tools → Run Self Test)
- Deterministic rendering engine using System.Windows.Media
- Anti-aliasing and high-quality graphics
- Unit tests for JSON round-trip, rendering, and export operations

## Requirements

- Windows 10 or later
- .NET 8.0 Runtime
- Visual Studio 2022 or later (for development)

## Building from Source

### Prerequisites
- .NET 8.0 SDK or later
- Windows operating system (required for WPF)

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
```

2. Restore NuGet packages:
```bash
dotnet restore BrokenArrowSkinLab.sln
```

3. Build the solution:
```bash
dotnet build BrokenArrowSkinLab.sln --configuration Release
```

4. Run the application:
```bash
dotnet run --project BrokenArrowSkinLab.App/BrokenArrowSkinLab.App.csproj
```

Or open `BrokenArrowSkinLab.sln` in Visual Studio and build/run from there.

## Usage

### Getting Started

1. **Launch the application**
   - Run `BrokenArrowSkinLab.App.exe` from the build output directory
   - Or use `dotnet run --project BrokenArrowSkinLab.App`

2. **Create a new project**
   - File → New Project (Ctrl+N)
   - Or open the sample project: File → Open → Browse to `Samples/SampleProject.baslproj`

3. **Design a knob**
   - Click "New Knob" in the toolbar or Tools menu
   - Configure layers in the properties panel
   - Adjust colors, gradients, and effects
   - Preview animation in real-time
   - Export using File → Export or the Export Wizard

4. **Create a layout**
   - Click "New Layout" in the toolbar or Tools menu
   - Add controls by dragging from the control palette
   - Position and resize using mouse or properties panel
   - Set control properties and metadata
   - Export skin descriptor for your target platform

### Keyboard Shortcuts

- `Ctrl+N` - New Project
- `Ctrl+O` - Open Project
- `Ctrl+S` - Save Project
- `Ctrl+Shift+S` - Save Project As
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+X` - Cut
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste
- `Del` - Delete selected items

### Sample Project

A sample project is included in the `Samples` directory:
- `Samples/SampleProject.baslproj` - Example audio plugin skin with knobs, sliders, buttons, and meters

Load this project to see example configurations and learn the workflow.

### Self-Test

To verify the installation and functionality:
1. Tools → Run Self Test
2. The self-test will:
   - Create a sample knob
   - Render frames
   - Export to various formats
   - Verify file outputs
   - Test JSON serialization
3. A report will be displayed showing pass/fail status

## Project Structure

```
BrokenArrowSkinLab/
├── BrokenArrowSkinLab.sln          # Visual Studio solution
├── BrokenArrowSkinLab.App/         # WPF application project
│   ├── ViewModels/                 # MVVM view models
│   ├── Views/                      # XAML views
│   ├── Commands/                   # Command implementations
│   ├── Services/                   # Application services
│   └── Infrastructure/             # MVVM base classes
├── BrokenArrowSkinLab.Core/        # Core business logic library
│   ├── Models/                     # Data models
│   ├── Rendering/                  # Rendering engine
│   ├── IO/                         # File I/O operations
│   └── Services/                   # Core services
├── BrokenArrowSkinLab.Tests/       # Unit tests
└── Samples/                        # Sample projects
```

## Architecture

- **MVVM Pattern**: Clean separation of concerns with ViewModels, Views, and Models
- **Rendering Engine**: System.Windows.Media for deterministic, high-quality graphics
- **JSON Serialization**: Newtonsoft.Json for all file formats
- **Async/Await**: Responsive UI with background rendering and export operations
- **Command Pattern**: ICommand implementations for all user actions
- **Undo/Redo**: Stack-based undo/redo system for all modifications

## Development

### Running Tests

```bash
dotnet test BrokenArrowSkinLab.Tests/BrokenArrowSkinLab.Tests.csproj
```

Note: Tests require Windows as they use WPF components.

### Adding New Features

1. Add models to `BrokenArrowSkinLab.Core/Models/`
2. Implement logic in `BrokenArrowSkinLab.Core/Services/`
3. Create ViewModels in `BrokenArrowSkinLab.App/ViewModels/`
4. Design XAML views in `BrokenArrowSkinLab.App/Views/`
5. Add unit tests in `BrokenArrowSkinLab.Tests/`

## License

See [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please ensure:
- Code follows existing patterns and conventions
- All tests pass
- New features include unit tests
- UI changes are documented

## Support

For issues, questions, or feature requests, please use the GitHub issue tracker.

## Version History

- **1.0.0** (2026-01-07)
  - Initial release
  - Knob Designer workspace
  - Skin Layout Designer workspace
  - Export to PNG filmstrip, sequence, and atlas
  - JSON-based project files
  - Self-test functionality
  - Complete MVVM WPF application

---

© 2026 Broken Arrow Software
