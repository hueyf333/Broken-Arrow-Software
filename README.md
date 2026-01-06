# SkinningStudio

A WYSIWYG GUI skinning application built with C++20, SDL2, OpenGL, and Dear ImGui.

## Overview

SkinningStudio is a professional tool for designing and styling user interfaces. It provides a visual editor with real-time preview, comprehensive styling system, and export capabilities for runtime integration.

## Features

- **Visual Editor**: Drag-and-drop widget placement with visual editing
- **Comprehensive Styling**: Theme system with tokens, widget defaults, and state-specific overrides
- **Multiple Widgets**: Button, Label, Image, TextField, Checkbox, Slider, and Container layouts
- **Layout Modes**: Absolute positioning, vertical stacks, and horizontal stacks
- **Undo/Redo**: Full command history for all design changes
- **Export/Import**: Save projects, export themes, and generate runtime packages
- **Validation & Optimization**: Built-in tools for project validation and theme optimization
- **Runtime Preview**: See your UI exactly as it will appear at runtime

## Building on Windows (Visual Studio 2022/2026)

### Prerequisites

1. **Visual Studio 2022 or later** with C++ desktop development workload
2. **vcpkg** (for dependency management)

### Setup vcpkg

```bash
# Clone vcpkg (if not already installed)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio
.\vcpkg integrate install
```

### Build Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Open the solution**
   ```
   Open SkinningStudio.sln in Visual Studio
   ```

3. **Configure vcpkg integration**
   - The project uses vcpkg **manifest mode** (vcpkg.json)
   - Ensure vcpkg is integrated: run `vcpkg integrate install` from your vcpkg directory
   - Dependencies will be installed automatically on first build
   - If dependencies don't install automatically, close and reopen Visual Studio

4. **Build the solution**
   - Select Debug or Release configuration
   - Build > Build Solution (Ctrl+Shift+B)
   - First build may take several minutes while vcpkg installs dependencies
   - Output will be in `bin/Debug/` or `bin/Release/`

5. **Run SkinningStudio**
   - Set SkinningStudio as startup project
   - Press F5 to run with debugging or Ctrl+F5 to run without debugging

### Dependencies

The following dependencies are managed via vcpkg.json:
- **SDL2**: Window management and input handling
- **GLEW**: OpenGL extension loading
- **Dear ImGui**: Immediate mode GUI with docking support
- **nlohmann/json**: JSON serialization

## Controls & Keyboard Shortcuts

### File Operations
- `Ctrl+N` - New Project
- `Ctrl+O` - Open Project
- `Ctrl+S` - Save Project
- `Ctrl+Shift+S` - Save As

### Edit Operations
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+X` - Cut
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste
- `Ctrl+D` - Duplicate
- `Delete` - Delete selected element
- `Ctrl+A` - Select All

### View
- `F1` - Toggle Keyboard Shortcuts overlay
- `Middle Mouse Button + Drag` - Pan canvas
- `Mouse Wheel` - Zoom canvas (when implemented)

### Canvas Interactions
- `Left Click` - Select element
- `Drag` - Move selected element
- `Resize Handles` - Resize selected element
- `Shift+Drag` - Multi-select (when implemented)

## User Interface

### Panels

#### Hierarchy Panel
- Tree view of all UI elements
- Drag-and-drop to reparent elements
- Visibility and lock toggles for each element
- Right-click context menu for operations

#### Inspector/Properties Panel
- Edit selected element properties:
  - Basic: Name, ID, Visibility, Enabled
  - Transform: Position (X, Y), Size (Width, Height)
  - Layout: Anchor, Pivot, Padding, Margin
  - Layout Mode: Absolute, Vertical Stack, Horizontal Stack
  - Widget-specific properties

#### Asset/Style Library Panel
- **Theme Tokens**: View and edit color, size, and font tokens
- **Widget Defaults**: Configure default styles for each widget type
- Import/Export theme JSON files

#### Canvas Panel
- Visual editing area
- Grid display (toggle with View > Show Grid)
- Snap to grid (toggle with View > Snap to Grid)
- Selection visualization with bounding box
- Resize handles for precise sizing

#### Runtime Preview Panel
- Real-time preview of UI as it will appear at runtime
- Uses the same rendering code as the runtime sample

### Menu Bar

#### File Menu
- New, Open, Save, Save As
- Import Theme, Export Theme
- Export Runtime Package
- Recent Files
- Exit

#### Edit Menu
- Undo, Redo
- Cut, Copy, Paste, Duplicate, Delete
- Select All

#### View Menu
- Toggle panel visibility
- Show Grid, Snap to Grid
- Reset Layout

#### Tools Menu
- Validate Project - Check for errors and warnings
- Theme Optimizer - Find unused tokens and optimize theme
- Preferences

#### Help Menu
- Keyboard Shortcuts (F1)
- About - Display version information

## File Formats

### Project File (.json)

Project files contain the complete UI layout and theme:

```json
{
  "version": "1.0",
  "elements": [
    {
      "id": "element_1",
      "name": "My Button",
      "type": "Button",
      "rect": [100, 100, 120, 40],
      "visible": true,
      "enabled": true,
      "text": "Click Me",
      "children": []
    }
  ],
  "theme": {
    "name": "Dark",
    "tokens": {
      "color.primary": {
        "name": "color.primary",
        "value": "#3B82F6",
        "category": "color"
      }
    },
    "widgetDefaults": {
      "Button": {
        "backgroundColor": "$color.surface",
        "textColor": "$color.text"
      }
    }
  }
}
```

### Theme File (theme.json)

Exported theme files contain only styling information:

```json
{
  "name": "Dark",
  "tokens": {
    "color.primary": {
      "name": "color.primary",
      "value": "#3B82F6",
      "category": "color"
    },
    "color.background": {
      "name": "color.background",
      "value": "#1E1E1E",
      "category": "color"
    }
  },
  "widgetDefaults": {
    "Button": {
      "backgroundColor": "$color.surface",
      "textColor": "$color.text",
      "borderColor": "$color.border",
      "padding": "$size.padding"
    }
  }
}
```

### UI Layout File (ui_layout.json)

Exported layout files contain the UI structure without theme:

```json
{
  "version": "1.0",
  "elements": [
    {
      "id": "root",
      "name": "Main Container",
      "type": "ContainerVertical",
      "rect": [0, 0, 800, 600],
      "layoutMode": 1,
      "spacing": 10,
      "children": [...]
    }
  ]
}
```

## Runtime Integration

### Exporting a Runtime Package

1. Design your UI in SkinningStudio
2. File > Export Runtime Package
3. Select output directory
4. Generated files:
   - `theme.json` - Theme styling
   - `ui_layout.json` - UI structure

### Using the Runtime Sample

```bash
# Build RuntimeSample project
# Run with exported files
RuntimeSample.exe theme.json ui_layout.json
```

The runtime sample demonstrates loading and rendering exported UI. You can integrate the same approach into your own applications.

## Widget Types

### Button
- Displays clickable button with text
- Properties: text, onClick handler

### Label
- Displays static text
- Properties: text, font size, alignment

### Image
- Displays an image
- Properties: imagePath, scale mode

### TextField
- Text input field
- Properties: placeholder, value, maxLength

### Checkbox
- Boolean toggle with label
- Properties: label, checked

### Slider
- Numeric value slider
- Properties: minValue, maxValue, value

### ContainerVertical
- Vertical layout container
- Automatically arranges children vertically
- Properties: spacing, padding

### ContainerHorizontal
- Horizontal layout container
- Automatically arranges children horizontally
- Properties: spacing, padding

## Styling System

### Theme Tokens

Tokens are reusable style values:
- **Color tokens**: `color.primary`, `color.background`, etc.
- **Size tokens**: `size.padding`, `size.borderRadius`, etc.
- **Font tokens**: `font.default`, `font.heading`, etc.

### Widget Defaults

Each widget type has default styles that apply to all instances:
- backgroundColor
- textColor
- borderColor
- padding
- borderRadius
- fontSize

### State Overrides

Elements can have state-specific style overrides:
- **Normal**: Default state
- **Hover**: Mouse over element
- **Active**: Element is being clicked/interacted with
- **Disabled**: Element is disabled
- **Focused**: Element has input focus

### Style Resolution Order

1. Element state overrides (highest priority)
2. Widget type defaults
3. Theme tokens (lowest priority)

## Tools

### Validation Tool

Checks for common issues:
- Duplicate element IDs
- Empty or invalid IDs
- Cyclic parent-child relationships
- Missing assets or fonts
- Invalid property values

### Theme Optimizer

Analyzes theme usage and reports:
- Unused tokens that can be removed
- Redundant style overrides
- Opportunities for consolidation

## Version

Current version: **1.0.0**

## Architecture

### Core Components

- **UIElement**: Base class for all UI elements
- **StyleSystem**: Theme management and style resolution
- **CommandManager**: Undo/redo functionality
- **Widget Classes**: Specific implementations (Button, Label, etc.)
- **Panels**: Editor UI panels (Hierarchy, Inspector, Canvas, etc.)
- **Serialization**: JSON import/export
- **Tools**: Validation and optimization utilities

### Data Flow

1. User creates/modifies UI elements in editor
2. Changes are tracked via CommandManager
3. Elements are rendered using StyleResolver
4. Project can be saved/loaded via ProjectSerializer
5. Runtime package exports for production use

## Troubleshooting

### Build Issues

**Problem**: Cannot open include file: 'nlohmann/json.hpp': No such file or directory
- **Solution 1**: Close and reopen Visual Studio to trigger vcpkg dependency restoration
- **Solution 2**: Ensure vcpkg is integrated with Visual Studio:
  ```bash
  vcpkg integrate install
  ```
- **Solution 3**: Clean the solution and rebuild:
  - Build > Clean Solution
  - Build > Rebuild Solution
- **Solution 4**: Manually restore vcpkg dependencies:
  ```bash
  vcpkg install nlohmann-json:x64-windows
  ```
- **Solution 5**: Verify vcpkg.json is in the solution root directory (same folder as .sln file)

**Problem**: vcpkg dependencies not found
- **Solution**: Ensure vcpkg is integrated with Visual Studio (`vcpkg integrate install`)
- **Solution**: Check that vcpkg.json is in the solution directory
- **Solution**: Verify Visual Studio has the "vcpkg integration" feature installed

**Problem**: OpenGL errors
- **Solution**: Update graphics drivers
- **Solution**: Verify OpenGL 3.3 support on your system

### Runtime Issues

**Problem**: Window doesn't appear
- **Solution**: Check SDL2.dll is in the output directory (should be copied automatically)

**Problem**: ImGui docking not working
- **Solution**: Verify ImGui was built with docking enabled (handled by vcpkg features)

## Contributing

This project is part of Broken Arrow Software. For issues or contributions, please use the GitHub repository.

## License

See LICENSE file for details.