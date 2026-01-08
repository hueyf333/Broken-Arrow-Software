# BASL Designer - Broken Arrow Software Layer Designer

A professional Qt-based GUI application for designing rotary knobs and UI skins, modeled after KnobMan and SkinMan.

## Features

### Knob Designer
- **Layered Rendering System**: Create complex knobs with multiple layers
- **Per-Layer Controls**: Color, gradient, opacity, visibility, and layer ordering
- **Flexible Parameters**: Customizable canvas size, frame count, angle range
- **Knob Components**: Inner/outer radius, tick count/style, pointer styles
- **Visual Effects**: Shadow, blur, and opacity controls
- **Live Preview**: Interactive preview with drag-to-rotate showing real-time frame/value feedback
- **Anti-aliased Rendering**: High-quality output using QPainter

### Layout Designer
- **WYSIWYG Canvas**: Visual design of UI layouts
- **Drag & Drop**: Intuitive control placement
- **Resize Handles**: Visual resizing of controls
- **Grid & Snap**: Alignment tools with snap-to-grid
- **Zoom & Pan**: Navigate large layouts easily
- **Control Types**: Knob, Slider (H/V), Button (Toggle/Momentary), Switch, Meter
- **Multi-Select**: Select and manipulate multiple controls at once
- **Copy/Paste/Duplicate**: Standard editing operations

### Import/Export
- **Import Filmstrips**: Import PNG/JPG/BMP with filmstrip slicing preview
- **Export Formats**:
  - PNG Filmstrip (Horizontal or Vertical)
  - PNG Frame Sequence
  - Atlas PNG + JSON map with frame coordinates
- **Scaling**: 1x, 2x, 3x, or 4x export scaling
- **JSON Descriptors**: Standard format for frame mapping

### Persistence & Project Management
- **Project Format**: .baslproj JSON files with versioning
- **Recent Files**: Track and quickly open recent projects
- **Autosave**: Automatic backup every 5 minutes
- **Crash Recovery**: Restore from autosave after unexpected shutdown
- **Undo/Redo**: Full undo/redo stack across all designers

### User Interface
- **Modern Qt Widgets**: Professional desktop application UI
- **Menu Bar**: File, Edit, View, Tools, Help menus - all functional
- **Toolbars**: Quick access to common operations
- **Status Bar**: Real-time feedback
- **Dockable Panels**: 
  - Property Inspector: Edit control properties
  - Asset Browser: Manage project assets
- **Document Tabs**: Knob Designer, Layout Designer, Preview

### Self-Test System
- **Automated Testing**: Built-in self-test from Tools → Run Self Test
- **Test Coverage**:
  1. Sample knob generation
  2. Layout creation
  3. Export to temp directory
  4. File verification (existence, size, format)
- **Results Dialog**: Detailed test results with pass/fail status

## Build Instructions

### Prerequisites
- **Visual Studio 2022** with C++20 support
- **Qt 6.x** (MSVC build)
  - Download from https://www.qt.io/download
  - Install Qt for MSVC 2019 64-bit (compatible with VS2022)
  - Required Qt modules: Core, GUI, Widgets
- **Qt Visual Studio Tools** extension (optional but recommended)

### Building with Visual Studio 2022

1. **Clone the repository**:
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Configure Qt Environment**:
   - Set Qt installation path in your environment or use Qt VS Tools
   - Ensure `qtenv2.bat` has been run or Qt paths are in PATH

3. **Open Solution**:
   - Open `BrokenArrowSoftware.sln` in Visual Studio 2022
   - The solution should automatically detect Qt if Qt VS Tools is installed
   - Otherwise, manually configure Qt path in project properties

4. **Build**:
   - Select configuration: Debug or Release
   - Build → Build Solution (Ctrl+Shift+B)
   - Executable will be in `bin\x64\Debug\` or `bin\x64\Release\`

5. **Run**:
   - Run the application from Visual Studio (F5)
   - Or run `bin\x64\Release\BASLDesigner.exe` directly

### Qt Integration Notes

The project uses standard Qt MSBuild integration. If you encounter build issues:

1. Install Qt VS Tools extension
2. Go to Extensions → Qt VS Tools → Qt Versions
3. Add your Qt installation path
4. Go to Project Properties → Qt Project Settings
5. Select the correct Qt version

## Usage

### Creating a New Knob

1. **File → New** to create a new project
2. Switch to the **Knob Designer** tab
3. Configure canvas size and frame count
4. Adjust angles, radius, ticks, and pointer settings
5. Manage layers using the layer panel:
   - Add layers with the **+** button
   - Edit layer colors by double-clicking
   - Reorder with ↑/↓ buttons
6. Preview updates in real-time
7. Drag on the preview to rotate and see different frames

### Designing a Layout

1. Switch to the **Layout Designer** tab
2. Set canvas dimensions
3. Add controls (currently via code/import)
4. Drag controls to position them
5. Resize using handles
6. Use Edit menu for copy/paste/duplicate operations
7. Enable/disable grid and snap-to-grid from View menu

### Exporting Your Work

1. **File → Export** to open the export dialog
2. Choose export format:
   - **Filmstrip Horizontal**: All frames in a single row
   - **Filmstrip Vertical**: All frames in a single column
   - **Frame Sequence**: Individual PNG files (frame_0000.png, etc.)
   - **Atlas + JSON**: Packed sprite sheet with JSON coordinate map
3. Set scale (1x - 4x)
4. Choose output location
5. Click OK to export

### Running Self-Tests

1. **Tools → Run Self Test**
2. Click **Run Tests** in the dialog
3. Watch progress as tests execute:
   - Knob generation
   - Layout creation
   - Export functionality
   - File verification
4. Review results - all tests should pass
5. Check temp directory for generated files

### Keyboard Shortcuts

- **Ctrl+N**: New Project
- **Ctrl+O**: Open Project
- **Ctrl+S**: Save Project
- **Ctrl+Z**: Undo
- **Ctrl+Y**: Redo
- **Ctrl+X**: Cut
- **Ctrl+C**: Copy
- **Ctrl+V**: Paste
- **Del**: Delete Selection
- **Ctrl+D**: Duplicate Selection
- **Ctrl+A**: Select All
- **Ctrl++**: Zoom In
- **Ctrl+-**: Zoom Out
- **Ctrl+0**: Reset Zoom

## Project Structure

```
Broken-Arrow-Software/
├── BrokenArrowSoftware.sln        # Visual Studio solution
├── BASLDesigner/                  # Main project
│   ├── BASLDesigner.vcxproj       # Project file
│   ├── include/                   # Header files
│   │   ├── MainWindow.h
│   │   ├── KnobDesigner.h
│   │   ├── LayoutDesigner.h
│   │   ├── PreviewWidget.h
│   │   ├── ProjectModel.h
│   │   ├── KnobRenderer.h
│   │   └── ...
│   ├── src/                       # Implementation files
│   │   ├── main.cpp
│   │   ├── MainWindow.cpp
│   │   ├── KnobDesigner.cpp
│   │   └── ...
│   └── resources/                 # Resources
│       ├── resources.qrc
│       └── icons/
├── Samples/                       # Sample projects
│   └── sample_knob.baslproj
└── README.md                      # This file
```

## Sample Project

A sample project is included in `Samples/sample_knob.baslproj`:

1. **File → Open**
2. Navigate to `Samples/sample_knob.baslproj`
3. Explore the pre-configured knob design
4. Try exporting it to see the results

## Technology Stack

- **Language**: C++20
- **GUI Framework**: Qt 6.x Widgets
- **Build System**: Visual Studio MSBuild (no CMake)
- **Compiler**: MSVC (Visual Studio 2022, v143 toolset)
- **Rendering**: QPainter, QImage, QPixmap, QPainterPath
- **Effects**: QRadialGradient, anti-aliasing
- **Persistence**: JSON (QJsonDocument, QJsonObject, QJsonArray)
- **Platform**: Windows (x64)

## Architecture

### Core Components

- **ProjectModel**: Data model for projects (knob parameters, layout, controls)
- **KnobRenderer**: Rendering engine for generating knob frames
- **KnobDesigner**: UI for configuring knob parameters
- **LayoutDesigner**: WYSIWYG canvas for UI layout
- **PreviewWidget**: Preview and animation playback
- **MainWindow**: Application shell with menu/toolbar/docks

### Design Patterns

- **MVC Pattern**: Separation of data (ProjectModel) and UI
- **Observer Pattern**: Signal/slot connections for UI updates
- **Command Pattern**: QUndoStack for undo/redo
- **Strategy Pattern**: Different export formats

## Known Limitations

- Layout designer control placement is manual (no GUI add control yet)
- Shadow/blur effects are simplified (full QGraphicsBlurEffect not implemented)
- Import functionality creates dialog but doesn't process filmstrips yet
- No preferences dialog content yet (placeholder)

## Future Enhancements

- Visual control addition in layout designer
- More sophisticated shadow/blur rendering
- Full filmstrip import with frame extraction
- Preferences dialog with settings
- Plugin system for custom control types
- Animation timeline editor
- Gradient editor UI
- Layer blend modes

## License

Copyright © 2024 Broken Arrow Software

## Support

For issues, questions, or contributions, please visit:
https://github.com/hueyf333/Broken-Arrow-Software

---

**Version**: 1.0.0  
**Last Updated**: January 2024