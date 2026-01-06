# SkinningStudio

A cross-platform WYSIWYG GUI Skinning application built with C++20, Dear ImGui, SDL2, and OpenGL.

## Features

### Core Functionality
- **WYSIWYG Canvas**: Visual editor with zoom, pan, grid, and snap-to-grid
- **Widget Library**: Button, Label, Image, TextField, Panel, Checkbox, Slider, and Container widgets
- **Theming System**: Token-based styling with color palettes, typography, and spacing tokens
- **Hierarchy Management**: Tree-based UI structure with drag-and-drop support
- **Property Inspector**: Edit widget properties, transforms, and style overrides
- **Undo/Redo**: Full command pattern implementation for all edit operations
- **Project Serialization**: Save and load projects in JSON format
- **Theme Import/Export**: Standalone theme files for easy sharing
- **Runtime Export**: Generate theme.json, ui_layout.json, and C++ runtime sample

### Editor Features
- Fully docked interface with customizable panels
- Multiple selection and bulk editing
- Alignment and distribution tools
- Z-order manipulation (bring forward/back)
- Grid and snap toggle
- Visual gizmos for selection and manipulation
- Keyboard shortcuts for common operations
- Recent files tracking
- Project validation tools
- Theme optimizer

## Building

### Windows (Visual Studio 2022+)

#### Prerequisites
- Visual Studio 2022 or later with C++ desktop development workload
- vcpkg package manager
- Git

#### Steps
```bash
# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Install vcpkg if not already installed
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
cd ..

# Configure CMake with vcpkg
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build --config Release

# Run
./build/bin/Release/SkinningStudio.exe
```

### macOS

#### Prerequisites
- Xcode Command Line Tools
- CMake 3.20+
- vcpkg package manager

#### Steps
```bash
# Install dependencies using Homebrew (alternative to vcpkg)
brew install cmake sdl2 nlohmann-json

# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Configure and build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)

# Run
./bin/SkinningStudio
```

### Linux

#### Prerequisites
- GCC 10+ or Clang 12+
- CMake 3.20+
- SDL2 development libraries
- OpenGL development libraries

#### Steps
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential cmake libsdl2-dev libgl1-mesa-dev

# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Configure and build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run
./bin/SkinningStudio
```

## Controls & Shortcuts

### File Operations
- `Ctrl+N`: New Project
- `Ctrl+O`: Open Project
- `Ctrl+S`: Save Project
- `Ctrl+Shift+S`: Save Project As

### Edit Operations
- `Ctrl+Z`: Undo
- `Ctrl+Y`: Redo
- `Ctrl+X`: Cut
- `Ctrl+C`: Copy
- `Ctrl+V`: Paste
- `Ctrl+D`: Duplicate
- `Delete`: Delete Selection
- `Ctrl+A`: Select All

### Canvas Navigation
- Mouse Wheel: Zoom in/out
- Middle Mouse + Drag: Pan canvas
- Left Click: Select widget
- Shift + Left Click: Add to selection
- Drag: Marquee selection

### Widget Manipulation
- Click + Drag: Move selected widgets
- Drag corner handles: Resize widgets
- `Ctrl` while dragging: Disable snapping

## File Formats

### Project File (.ssproj)
JSON format containing:
```json
{
  "version": "1.0.0",
  "root": {
    "id": "root",
    "type": "Panel",
    "name": "Root",
    "rect": [0, 0, 800, 600],
    "visible": true,
    "enabled": true,
    "children": [...]
  },
  "theme": {...}
}
```

### Theme File (.sstheme)
JSON format containing:
```json
{
  "colors": {
    "primary": "#1EA4EBFF",
    "background": "#21222CFF"
  },
  "spacing": {
    "sm": 8.0,
    "md": 16.0
  },
  "fonts": {
    "default": {
      "path": "fonts/roboto.ttf",
      "size": 14.0
    }
  },
  "widgetDefaults": {
    "Button": {
      "backgroundColor": "surface",
      "textColor": "text"
    }
  }
}
```

### Layout Export (ui_layout.json)
Contains the hierarchical structure of UI elements for runtime loading.

### Runtime Sample
Generated C++ code demonstrating how to load and render the exported UI using the same widget system.

## Architecture

### Modules
- **App**: Main application loop, SDL/OpenGL setup, ImGui docking
- **Editor**: Menu system, panels, keyboard shortcuts, file management
- **Canvas**: WYSIWYG rendering, selection, gizmos, grid, zoom/pan
- **UIElement**: Widget base class and derived types
- **Theme**: Token-based styling system with state support
- **CommandManager**: Undo/redo command pattern implementation
- **Project**: Serialization, validation, import/export
- **Panels**: Hierarchy, Inspector, Asset, Canvas, Preview panels

### Data Flow
1. User interacts with Canvas or Inspector
2. Changes wrapped in Command objects
3. Commands executed and pushed to undo stack
4. Project marked as modified
5. Changes reflected in all panels
6. Serialization on save

## Development

### Adding New Widgets
1. Add enum value to `WidgetType` in `UIElement.h`
2. Create derived class implementing `Render()` and serialization
3. Update `UIElement::Create()` factory method
4. Add to widget palette in HierarchyPanel

### Extending the Theme System
1. Add new token types in `Theme.h`
2. Implement token getters/setters
3. Update serialization methods
4. Add UI for token editing in AssetPanel

## Version

Current version: 1.0.0

## License

See LICENSE file for details.

## About

SkinningStudio is a professional-grade GUI design tool for creating themeable user interfaces. Built with modern C++20 and industry-standard libraries, it provides a powerful yet intuitive environment for UI/UX designers and developers.
