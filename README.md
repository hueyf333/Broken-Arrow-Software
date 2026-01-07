# KnobSmith - Professional Knob and Control Designer

KnobSmith is a powerful desktop application for designing custom GUI controls, knobs, sliders, and buttons for audio plugins and software interfaces. Built with JUCE framework, it provides a complete workflow from creation to filmstrip export.

## Features

### Project Management
- **JSON-based project files** (.knob) with complete state preservation
- New, Open, Save, and Save As operations
- Recent files tracking
- Auto-save support
- Robust error handling and validation

### Image Import & Conversion
- **Supported formats**: PNG, JPG, JPEG, BMP, GIF (TIFF and WEBP with codec availability)
- Automatic conversion to internal RGBA format
- Drag & drop import capability
- Asset browser with thumbnail view
- Error messages for unsupported codecs

### Professional Editor Workspace
- **Central canvas** with zoom (10%-1000%), pan, and fit-to-screen
- **Grid overlay** with customizable size and snap-to-grid
- **Five-layer system**:
  - Background - Base background elements
  - Base Art - Main control artwork
  - Indicator/Cap - Rotating indicator or knob cap
  - Glow/Shadow - Lighting effects and shadows
  - Marks/Ticks - Scale markings and tick marks
- Layer visibility toggling and reordering
- Visual layer panel with quick access

### Painting & Editing Tools
- **Selection Tool** (V) - Select, move, scale, and rotate
- **Brush Tool** (B) - Paint with adjustable size, hardness, and opacity
- **Eraser Tool** (E) - Erase with variable size
- **Fill Tool** (F) - Flood fill with selected color
- **Shape Tools** - Draw lines, rectangles, and ellipses
- **Color Picker** - Eyedropper to sample colors from canvas
- Comprehensive undo/redo system for all operations

### Filmstrip Generation
- **Configurable filmstrip settings**:
  - Orientation: Vertical or Horizontal
  - Frame count (1-999)
  - Frame dimensions (width x height)
  - Padding between frames
  - Start/End frame range
  - Rotation angle range (e.g., -135° to +135°)
  - Rotation center point (normalized 0-1)
- **Automatic generation pipeline** - Renders filmstrip by rotating indicator layer across N frames
- Frame preview scrubber
- Frame guides overlay for precise positioning

### Export Capabilities
- **PNG Export** (Single composited image)
- **Filmstrip Export** (Multi-frame strip)
- **Export options**:
  - Scale: 1x, 2x, 4x for retina displays
  - Transparent background toggle
  - Premultiply alpha toggle
- **Layer Export** - Export individual layers as PNG
- All exports use JUCE PNGImageFormat for consistent quality

### User Interface
- **Dark professional theme** for comfortable long editing sessions
- **Menu system** fully integrated with keyboard shortcuts
- **Panel layout**:
  - Left: Tool palette and layer panel
  - Center: Canvas workspace
  - Right: Properties inspector and asset browser
  - Bottom: Status bar with cursor position, zoom level, and tool info
- **Status bar** shows real-time cursor coordinates, zoom percentage, and contextual hints

## System Requirements

- Windows 10/11 with Visual Studio 2022
- JUCE Framework (modules: juce_core, juce_data_structures, juce_events, juce_graphics, juce_gui_basics, juce_gui_extra)
- Minimum 4GB RAM
- OpenGL-capable graphics card recommended

## Building from Source

### Prerequisites
1. Install Visual Studio 2022 with C++ Desktop Development workload
2. Download and install JUCE from https://juce.com/get-juce
3. Set up JUCE global paths in Projucer

### Build Steps
1. **Open the project**:
   ```
   Open KnobSmith.jucer in Projucer
   ```

2. **Configure JUCE paths**:
   - Go to File → Global Paths
   - Set JUCE Modules path to your JUCE installation
   - Save settings

3. **Export Visual Studio project**:
   - In Projucer, click "Save Project and Open in IDE" or
   - Select File → Export → Visual Studio 2022
   - This creates the Builds/VisualStudio2022 folder

4. **Build in Visual Studio**:
   ```
   Open Builds/VisualStudio2022/KnobSmith.sln
   Select Debug or Release configuration
   Build → Build Solution (Ctrl+Shift+B)
   ```

5. **Run the application**:
   - Executable located in: `Builds/VisualStudio2022/x64/Debug/KnobSmith.exe`
   - Or: `Builds/VisualStudio2022/x64/Release/KnobSmith.exe`

### Troubleshooting
- **Missing JUCE modules**: Ensure global paths are set correctly in Projucer
- **Build errors**: Verify C++20 language standard is enabled (set in .jucer)
- **Linking errors**: Rebuild entire solution (Clean → Build)

## Usage Guide

### Creating a New Control

1. **Start a new project**:
   - File → New (Ctrl+N) or click New button
   - Set canvas size (default 512x512)
   - Choose control type (Knob, Slider, Button, Custom)

2. **Import base artwork**:
   - File → Import Image (Ctrl+I)
   - Select layer in Layer Panel
   - Choose image file
   - Image automatically converts to RGBA

3. **Paint and edit**:
   - Select tool from Tool Panel or use keyboard shortcuts
   - Adjust brush size, opacity, and color in Properties Panel
   - Paint directly on canvas
   - Use Ctrl+Z/Ctrl+Shift+Z for undo/redo

4. **Configure filmstrip**:
   - Filmstrip → Settings
   - Set frame count (e.g., 64 for smooth rotation)
   - Configure rotation angles (-135° to +135° for 270° sweep)
   - Set rotation center point (usually 0.5, 0.5 for center)
   - Choose orientation (vertical for most DAWs)

5. **Generate filmstrip**:
   - Filmstrip → Generate Filmstrip
   - Preview with frame scrubber
   - Adjust as needed

6. **Export**:
   - File → Export PNG (Ctrl+E) for single frame
   - File → Export As for filmstrip with options
   - Choose scale factor (1x, 2x, 4x)
   - Select transparent background if needed

7. **Save project**:
   - File → Save (Ctrl+S) or Save As (Ctrl+Shift+S)
   - Projects saved as .knob files (JSON format)

### Keyboard Shortcuts

#### File Operations
- `Ctrl+N` - New project
- `Ctrl+O` - Open project
- `Ctrl+S` - Save project
- `Ctrl+Shift+S` - Save As
- `Ctrl+I` - Import image
- `Ctrl+E` - Export PNG
- `Ctrl+Q` - Quit (Windows) / `Cmd+Q` (Mac)

#### Edit Operations
- `Ctrl+Z` - Undo
- `Ctrl+Shift+Z` / `Ctrl+Y` - Redo
- `Ctrl+X` - Cut
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste
- `Delete` - Delete selection

#### View Controls
- `Ctrl++` / `Ctrl+=` - Zoom in
- `Ctrl+-` - Zoom out
- `Ctrl+0` - Fit to screen
- `Ctrl+G` - Toggle grid
- `Middle Mouse` or `Ctrl+Drag` - Pan canvas
- `Ctrl+Mouse Wheel` - Zoom at cursor

#### Tools (Single Key)
- `V` - Selection tool
- `B` - Brush tool
- `E` - Eraser tool
- `F` - Fill tool
- `S` - Shape tool
- `I` - Color picker (eyedropper)

### Supported Import Formats

| Format | Extension | Status | Notes |
|--------|-----------|--------|-------|
| PNG | .png | ✅ Guaranteed | Lossless, supports transparency |
| JPEG | .jpg, .jpeg | ✅ Guaranteed | Lossy compression, no transparency |
| BMP | .bmp | ✅ Guaranteed | Bitmap, large files |
| GIF | .gif | ✅ Guaranteed | Limited colors, supports transparency |
| TIFF | .tiff, .tif | ⚠️ Conditional | Requires codec, may not be available |
| WebP | .webp | ⚠️ Conditional | Requires codec, may not be available |

**Note**: If a codec is unavailable for TIFF or WebP, KnobSmith will display a message explaining the limitation. Use PNG for best compatibility.

### Export Formats

All exports use **PNG format** for maximum quality and compatibility:
- **Single PNG**: Composited image of all visible layers
- **Filmstrip PNG**: Vertical or horizontal strip of all frames
- **Layer PNG**: Individual layer export

### Tips & Best Practices

1. **Layer Organization**:
   - Use Background layer for solid fills and base elements
   - Place main artwork on Base Art layer
   - Put rotating elements (needles, indicators) on Indicator/Cap layer
   - Add lighting effects to Glow/Shadow layer
   - Use Marks/Ticks for scale markings

2. **Filmstrip Generation**:
   - 64 frames provides smooth animation for most knobs
   - Use 128 frames for ultra-smooth professional results
   - Set rotation center precisely for best results
   - Preview generated frames before exporting

3. **Performance**:
   - Larger canvas sizes (>1024x1024) may impact performance
   - Use zoom to work on details at 100% canvas size
   - Save frequently to prevent data loss

4. **Workflow**:
   - Start with Base Art layer for main design
   - Add Indicator on separate layer for easy rotation
   - Test filmstrip generation early in the design process
   - Export at multiple scales (1x, 2x) for different display densities

## Project File Format

KnobSmith projects are saved as JSON (.knob) files containing:
- Canvas dimensions and background color
- Layer data (names, visibility, opacity, images as base64 PNG)
- Layer transforms (rotation, scale, position)
- Filmstrip configuration (orientation, frame count, angles, etc.)
- Export settings (scale, transparency, premultiply alpha)
- Control type metadata

Example structure:
```json
{
  "version": 1,
  "canvasWidth": 512,
  "canvasHeight": 512,
  "backgroundColour": "ff808080",
  "controlType": 0,
  "layers": [...],
  "filmstripConfig": {...},
  "exportSettings": {...}
}
```

## Architecture Overview

KnobSmith follows a modular architecture:

- **App/** - Application entry point, main window, menu system
- **UI/** - User interface components (panels, canvas view, status bar)
- **Canvas/** - Canvas rendering, layer composition, filmstrip generation
- **Tools/** - Tool implementations (brush, eraser, fill, shapes, etc.)
- **Model/** - Data models (Project, Layer, FilmstripConfig, ExportSettings)
- **IO/** - File I/O (project serialization, image import/export)
- **Commands/** - Command system for undo/redo and menu integration
- **Utils/** - Utility functions (image manipulation, geometry, recent files)

## License

See LICENSE file for details.

## Contributing

This project uses modern C++20 idioms and JUCE best practices. All contributions should:
- Follow RAII principles
- Use JUCE coding conventions
- Include proper error handling
- Maintain compatibility with Visual Studio 2022

## Support

For issues, questions, or feature requests, please open an issue on the project repository.

---

**Built with JUCE** - Cross-platform C++ framework for audio applications