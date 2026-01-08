# BrokenArrowSkinLab

A KnobMan/SkinMan-style skin authoring tool for creating knob graphics and UI layouts for audio plugins and applications.

## Features

### Knob Designer
- **Layer Stack Editor**: Background, ring, ticks, pointer, gloss, shadow, and text layers
- **Live Preview**: Interactive knob that can be dragged to rotate and preview frames
- **Layer Management**: Add, remove, reorder, and toggle visibility of layers
- **Per-layer Properties**: Color/gradient, opacity, blur/shadow effects, blend modes
- **Customizable Parameters**:
  - Canvas size (width/height)
  - Frame count (for animation)
  - Min/max rotation angles
  - Tick count and style
  - Pointer style (Line, Dot)
  - Shadow parameters

### Layout Designer (WYSIWYG)
- **Grid System**: Customizable grid with snap-to-grid functionality
- **Zoom/Pan**: Navigate large layouts easily
- **Control Types**: Knob, Slider (H/V), Button (toggle/momentary), Switch, Meter
- **Drag & Drop**: Place and position controls visually
- **Multi-select**: Select and manipulate multiple controls
- **Align & Distribute**: Tools for precise alignment

### Asset Generation & Export
- **PNG Filmstrips**: Generate vertical and horizontal filmstrips
- **PNG Sequence**: Export individual frame files
- **Atlas Generation**: Create texture atlas with JSON mapping
- **Multiple Scales**: Export at 1x, 2x, or custom scaling

### Project Management
- **Save/Load Projects**: JSON-based project files (.baslproj)
- **Recent Files**: Quick access to recently opened projects
- **Autosave**: Never lose your work
- **Undo/Redo**: Full undo/redo support across all designers

### Additional Features
- **Dark Theme UI**: Professional dark-themed interface
- **Menu System**: Complete File/Edit/View/Tools/Help menus
- **Property Inspector**: View and edit control properties
- **Project Tree**: Organize your assets and controls
- **Self Test**: Built-in testing tool to verify functionality

## Building from Source

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+, Xcode 10+)
- JUCE framework (version 8.0.4 or compatible)
- **For CMake builds**: CMake 3.22 or higher
- **For Projucer builds**: Projucer application from JUCE

#### Linux
```bash
sudo apt-get install libasound2-dev libx11-dev libxrandr-dev libxinerama-dev \
                     libxcursor-dev libfreetype6-dev libgl1-mesa-dev libfontconfig1-dev
```

#### macOS
```bash
# Xcode and Command Line Tools required
xcode-select --install
```

#### Windows
- Visual Studio 2017 or later with C++ development tools

### Build Steps

#### Method 1: Using CMake (Recommended)

1. **Clone the repository**:
```bash
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
```

2. **Download JUCE** (if not already present):
```bash
git clone --depth 1 --branch 8.0.4 https://github.com/juce-framework/JUCE.git
```

3. **Configure with CMake**:
```bash
mkdir build
cd build
cmake ..
```

4. **Build**:
```bash
cmake --build . --config Release
```

5. **Run the application**:
```bash
# Linux/macOS
./BrokenArrowSkinLab_artefacts/Release/BrokenArrowSkinLab

# Windows
BrokenArrowSkinLab_artefacts\Release\BrokenArrowSkinLab.exe
```

#### Method 2: Using Projucer

1. **Clone the repository and download JUCE** (same as Method 1, steps 1-2)

2. **Open the project in Projucer**:
   - Launch Projucer from your JUCE installation
   - Open `BrokenArrowSkinLab.jucer` file in the repository root
   - The project will automatically configure itself

3. **Select your target platform**:
   - Click on the appropriate exporter (Xcode, Visual Studio 2022, Linux Makefile)
   - Click "Save Project and Open in IDE" button

4. **Build in your IDE**:
   - **Xcode (macOS)**: Select the BrokenArrowSkinLab target and build (⌘B)
   - **Visual Studio (Windows)**: Build the solution (F7)
   - **Linux Makefile**: Run `make` in the Builds/LinuxMakefile directory

5. **Run the application**:
   - The built executable will be in the `Builds/[Platform]/build/` directory

## Usage

### Creating a New Knob

1. Launch BrokenArrowSkinLab
2. Go to **File → New Project**
3. Switch to the **Knob Designer** tab
4. Adjust canvas size and frame count in the properties
5. Add/remove/reorder layers as needed
6. Drag the preview knob to see different frames
7. Export via **File → Export Assets**

### Loading the Sample Project

1. Go to **File → Open Project**
2. Navigate to `SampleProject/sample.baslproj`
3. Click **Open**
4. The project will load with default knob settings

### Exporting Assets

1. Complete your knob design
2. Go to **File → Export Assets**
3. Select output directory
4. Choose export formats:
   - Vertical filmstrip (default)
   - Horizontal filmstrip
   - PNG sequence
   - Atlas + JSON
5. Select export scale (1x or 2x)
6. Click **Export**

### Running Self Test

1. Go to **Tools → Run Self Test**
2. The application will:
   - Generate a sample knob preset
   - Render all frames
   - Export to temporary directory
   - Verify output dimensions
   - Display results

## Project Structure

```
Broken-Arrow-Software/
├── BrokenArrowSkinLab/
│   └── Source/
│       ├── Main.cpp              # Application entry point
│       ├── MainComponent.h       # Main UI component header
│       └── MainComponent.cpp     # Main UI implementation
├── SampleProject/
│   └── sample.baslproj          # Sample project file
├── JUCE/                        # JUCE framework (gitignored)
├── CMakeLists.txt               # CMake build configuration
└── README.md                    # This file
```

## File Formats

### .baslproj (Project File)
JSON format containing:
- Canvas dimensions
- Frame count and angle settings
- Tick configuration
- Grid and snap settings
- Layer definitions
- Layout control data

Example:
```json
{
  "canvasWidth": 128,
  "canvasHeight": 128,
  "frameCount": 64,
  "minAngle": -135,
  "maxAngle": 135,
  "tickCount": 11,
  "pointerStyle": "Line",
  "gridEnabled": true,
  "gridSize": 10,
  "snapEnabled": true,
  "zoom": 1
}
```

## Keyboard Shortcuts

- **Ctrl/Cmd + N**: New Project
- **Ctrl/Cmd + O**: Open Project
- **Ctrl/Cmd + S**: Save Project
- **Ctrl/Cmd + Z**: Undo
- **Ctrl/Cmd + Shift + Z**: Redo
- **Ctrl/Cmd + +**: Zoom In
- **Ctrl/Cmd + -**: Zoom Out
- **Ctrl/Cmd + 0**: Reset Zoom

## Menu Reference

### File Menu
- **New Project**: Create a new empty project
- **Open Project**: Load an existing .baslproj file
- **Save Project**: Save current project
- **Save Project As**: Save project with new name
- **Export Assets**: Export knob frames and layouts
- **Exit**: Close the application

### Edit Menu
- **Undo**: Undo last action
- **Redo**: Redo previously undone action
- **Cut**: Cut selected items
- **Copy**: Copy selected items
- **Paste**: Paste from clipboard
- **Delete**: Delete selected items

### View Menu
- **Zoom In**: Increase canvas zoom
- **Zoom Out**: Decrease canvas zoom
- **Reset Zoom**: Reset to 100% zoom
- **Show Grid**: Toggle grid visibility
- **Snap to Grid**: Toggle snap-to-grid

### Tools Menu
- **Add Knob**: Add new knob control
- **Add Slider (H/V)**: Add slider controls
- **Add Button**: Add button control
- **Run Self Test**: Execute built-in tests

### Help Menu
- **About**: Show application information
- **Documentation**: Open online documentation

## Troubleshooting

### Build Issues

**Error: JUCE not found**
- Ensure JUCE is cloned in the repository root
- Check that JUCE is version 8.0.4 or compatible

**Error: Missing dependencies (Linux)**
- Install all required development packages listed in Prerequisites
- Run `sudo apt-get update` before installing

**CMake configuration fails**
- Update CMake to version 3.22 or higher
- Ensure all development tools are installed

### Runtime Issues

**Application doesn't start**
- Check that all shared libraries are installed
- On Linux, verify X11 display is available

**Export fails**
- Ensure write permissions in output directory
- Check available disk space

## Contributing

Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

See LICENSE file for details.

## Credits

Built with:
- [JUCE Framework](https://juce.com/) - Cross-platform C++ framework
- CMake - Build system
- Modern C++17

## Contact

For issues, questions, or suggestions:
- GitHub Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues
- Project Repository: https://github.com/hueyf333/Broken-Arrow-Software

---

**BrokenArrowSkinLab** - Professional skin authoring for audio applications
