# Broken Arrow Hair Studio

A production-ready Windows desktop hair authoring tool built with C++20, Visual Studio 2022, WinUI 3, and DirectX 12.

## Overview

Broken Arrow Hair Studio is a comprehensive hair authoring and grooming tool designed for creating realistic hair and fur for game engines (Unreal Engine) and rendering software (Daz Studio). It provides an intuitive workflow for creating guide curves, generating hair strands, grooming with brushes, and exporting to multiple formats.

## System Requirements

### Prerequisites
- **Operating System**: Windows 10/11 (64-bit)
- **Visual Studio**: Visual Studio 2022 (v143 toolset) with:
  - Desktop development with C++ workload
  - Windows 11 SDK (10.0.22621.0 or later)
  - Windows App SDK 1.4 or later
- **Graphics**: DirectX 12 compatible GPU
- **RAM**: 8GB minimum, 16GB recommended

### External SDKs (Optional for Full Export Functionality)
- **Autodesk FBX SDK** 2020.3.4 or later (for FBX export)
  - Download from: https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-3-4
  - Install to: `C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.4`
- **Alembic SDK** 1.8.5 or later (for Alembic cache export)
  - Build from source: https://github.com/alembic/alembic
  - Or use vcpkg: `vcpkg install alembic:x64-windows`

## Building from Source

### Quick Start

1. **Clone the repository**:
   ```bash
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   ```

2. **Open in Visual Studio 2022**:
   - Open `BrokenArrowHairStudio.sln`
   - Select **x64** platform
   - Select **Debug** or **Release** configuration

3. **Install Windows App SDK** (if not already installed):
   - Visual Studio will prompt to install required NuGet packages
   - Or manually: Tools → NuGet Package Manager → Manage NuGet Packages for Solution
   - Install `Microsoft.WindowsAppSDK` (version 1.4.x or later)

4. **Build Solution**:
   - Build → Build Solution (Ctrl+Shift+B)
   - First build may take several minutes to download NuGet packages

5. **Run**:
   - Set `BrokenArrowHairStudio` as startup project
   - Debug → Start Without Debugging (Ctrl+F5)

### Project Structure

```
BrokenArrowHairStudio/
├── src/
│   ├── app/              # Main WinUI 3 application
│   ├── viewport/         # DirectX 12 rendering viewport
│   ├── hair/             # Hair simulation and authoring core
│   ├── io/               # File I/O and serialization
│   ├── math/             # Math library (vectors, matrices, splines, noise, BVH)
│   └── third_party/      # Third-party dependencies
├── assets/
│   ├── meshes/           # Sample scalp meshes
│   ├── textures/         # Sample textures and UV checkers
│   ├── presets/          # Hair description presets
│   └── projects/         # Sample .bahair project files
├── docs/                 # Additional documentation
├── README.md             # This file
└── BrokenArrowHairStudio.sln  # Visual Studio solution
```

## Features

### Core Workflow
1. **Project Management**: Create, open, and save `.bahair` projects with autosave and crash recovery
2. **Mesh Import**: Import scalp meshes from OBJ, FBX, or GLTF formats
3. **Guide Creation**: Draw guide curves directly on the scalp surface
4. **Strand Generation**: Automatically generate hair strands interpolated from guides
5. **Grooming**: Use brushes to comb, smooth, inflate, cut, and style hair
6. **Painting**: Paint density, length, and other parameter masks
7. **Baking**: Generate hair cards or strand meshes for game engines
8. **Export**: Export to Unreal Engine (FBX) or Daz Studio (OBJ) with proper formatting

### Hair System Features
- **Guide Curves**: Create and edit guide curves on surface with transform gizmos
- **Strand Interpolation**: Generate strands using distance and orientation-based weights
- **Distribution**: Uniform surface sampling, vertex weights, or texture density maps
- **Modifier Stack**: Non-destructive modifiers with enable/bypass/reorder:
  - Length adjustment
  - Width profile (root to tip)
  - Clumping
  - Noise/Frizz
  - Curl
  - Bend/Gravity
  - Parting
  - Smooth/Relax
  - Mirror X
- **Deterministic Seeds**: Reproducible results

### Grooming Tools
- **Brushes**:
  - Comb: Direct hair flow
  - Smooth: Relax and blend strands
  - Inflate/Deflate: Adjust volume
  - Cut: Trim hair length
  - Grow: Extend hair
  - Clump: Group nearby strands
- **Brush Parameters**: Radius, strength, falloff, spacing
- **X-Symmetry**: Mirror strokes across center

### Paint System
- **Mask Types**: Density, Length, Clump weight, Frizz, Curl
- **Output**: PNG/TIFF textures or per-vertex data
- **Brush Overlays**: Visual feedback in viewport

### Viewport Features
- **DirectX 12 Rendering**: High-performance real-time preview
- **Camera**: Orbit, pan, zoom navigation
- **Render Modes**:
  - Guides only
  - Preview strands (thick lines)
  - Baked mesh (cards/tubes)
- **Overlays**: Wireframe, normals, UV checker
- **Selection**: Pick and transform guides and scalp
- **Gizmos**: Move, rotate, scale tools

### Baking System
- **Hair Cards**: 
  - Cluster by clump groups
  - Generate card meshes with UVs
  - Simple atlas layout
  - Material slots: BaseColor, Opacity, Normal
- **Strand Mesh**:
  - Tubes or ribbons
  - Adjustable sides and radius
  - Taper control
  - LOD support

### Export Pipeline
- **Formats**:
  - FBX (via Autodesk SDK or OpenFBX)
  - Alembic (.abc caches)
  - OBJ
  - GLTF (via tinygltf)
- **Presets**:
  - Unreal Engine (cm, Z-up, FBX 2018+)
  - Daz Studio (cm, Y-up, FBX/OBJ)
- **Features**:
  - Scale and axis conversion
  - Automatic triangulation
  - Merge by material
  - Texture embedding/copying
- **Validation**: Scene validation before export

### Quality Features
- **Autosave**: Periodic automatic saving
- **Crash Recovery**: Restore from autosave on crash
- **Undo/Redo**: Command pattern for all operations
- **Error Reporting**: Modal dialogs and log panel
- **Performance**: BVH acceleration, multithreaded job system

## User Guide

### Getting Started

1. **Launch the Application**
   - Run `BrokenArrowHairStudio.exe`
   - The main window opens with an empty viewport

2. **Create a New Project**
   - File → New Project
   - Save location dialog appears
   - New project created with default scene

3. **Import a Scalp Mesh**
   - File → Import Mesh
   - Select an OBJ, FBX, or GLTF file
   - Mesh appears in viewport and outliner

4. **Create Hair Description**
   - Right-click scalp mesh in outliner
   - Select "Add Hair Description"
   - New hair system created and bound to mesh

5. **Draw Guide Curves**
   - Select "Draw Guides" tool from toolbar
   - Click and drag on scalp surface to draw curves
   - Guides appear as colored curves in viewport

6. **Generate Strands**
   - In Properties panel, adjust:
     - Density: Number of strands per unit area
     - Length: Base hair length
     - Segments: Curve resolution
   - Click "Generate Strands"
   - Interpolated strands appear in viewport

7. **Add Modifiers**
   - In Modifier Stack panel, click "Add Modifier"
   - Select modifier type (e.g., Clump, Noise, Curl)
   - Adjust parameters in Properties panel
   - See real-time preview in viewport

8. **Groom the Hair**
   - Select grooming brush from toolbar (e.g., Comb)
   - Adjust brush size and strength
   - Click and drag in viewport to modify hair
   - Use X-symmetry for symmetric styling

9. **Paint Masks**
   - Switch to Paint Maps panel
   - Select mask type (e.g., Density)
   - Paint in viewport to control parameter variations
   - Save texture maps for export

10. **Bake and Export**
    - Switch to Bake/Export panel
    - Select bake mode (Hair Cards or Strand Mesh)
    - Click "Bake"
    - Select export format and preset
    - Click "Export"
    - Choose output file location

### Keyboard Shortcuts

- **General**:
  - Ctrl+N: New Project
  - Ctrl+O: Open Project
  - Ctrl+S: Save Project
  - Ctrl+Z: Undo
  - Ctrl+Y: Redo
  - Ctrl+Shift+S: Save As

- **Viewport**:
  - Middle Mouse: Rotate camera (orbit)
  - Shift+Middle Mouse: Pan camera
  - Mouse Wheel: Zoom camera
  - F: Frame selection
  - Alt+X: Toggle X-ray mode

- **Tools**:
  - Q: Select tool
  - W: Move tool
  - E: Rotate tool
  - R: Scale tool
  - G: Draw guides tool

- **Display**:
  - 1: Guides only
  - 2: Preview strands
  - 3: Baked mesh
  - Alt+W: Toggle wireframe
  - Alt+N: Toggle normals
  - Alt+U: Toggle UV checker

## Import to Game Engines

### Unreal Engine 5

1. **Export from Broken Arrow**:
   - Bake → Hair Cards or Strand Mesh
   - Export → Format: FBX
   - Preset: Unreal Engine 5
   - Export with textures

2. **Import to Unreal**:
   - Content Browser → Import
   - Select exported FBX file
   - Import settings:
     - Skeletal Mesh: No
     - Import Materials: Yes
     - Import Textures: Yes
     - Transform → Convert Scene: No (already in UE coordinates)
   - Click Import

3. **Setup Groom Asset** (for strand-based hair):
   - Right-click FBX → Create Groom
   - Or use Groom plugin for Alembic import
   - Attach to character skeletal mesh

4. **Setup Material** (for hair cards):
   - Open imported material
   - Connect BaseColor, Opacity, Normal textures
   - Set Blend Mode: Masked or Translucent
   - Enable Two Sided
   - Adjust opacity clip threshold

### Daz Studio

1. **Export from Broken Arrow**:
   - Bake → Hair Cards or Strand Mesh
   - Export → Format: OBJ or FBX
   - Preset: Daz Studio
   - Export with textures

2. **Import to Daz**:
   - File → Import
   - Select exported OBJ/FBX
   - Import settings:
     - Scale: 1.0 (already in cm)
     - Axis: Y-up (already correct)
   - Click Accept

3. **Parent to Figure**:
   - Select hair mesh in Scene tab
   - Right-click → Change Parent
   - Select character figure head bone

4. **Apply Textures**:
   - Surfaces tab → Select hair mesh
   - Load textures into appropriate channels:
     - Diffuse Color → BaseColor
     - Opacity → Opacity map
     - Bump/Normal → Normal map
   - Adjust surface settings (transparency, glossiness, etc.)

## Workflow Tutorial

### Creating Realistic Hair

This tutorial walks through creating a complete hairstyle from scratch.

**Part 1: Setup**

1. Create new project
2. Import base scalp mesh (`assets/meshes/female_scalp.obj`)
3. Create hair description named "MainHair"

**Part 2: Guide Placement**

1. Enable "Draw Guides" tool
2. Draw primary guides following desired hair flow:
   - Start at hairline, draw backward
   - Create guides for partings
   - Add guides for volume areas
   - Typical count: 50-100 guides for medium detail
3. Use Move tool (W) to adjust guide positions
4. Use Rotate tool (E) to adjust guide orientations

**Part 3: Strand Generation**

1. Properties panel → Hair Description:
   - Density: 10000 (adjust for performance vs. quality)
   - Length: 25.0 cm
   - Length Variation: 0.1
   - Segments: 16
   - Seed: 12345
2. Click "Generate Strands"
3. Wait for generation (progress in status bar)

**Part 4: Modifier Stack**

Add modifiers in this order:

1. **Clump** modifier:
   - Clump Count: 200
   - Clump Strength: 0.6
   - Clump Radius: 2.0

2. **Noise** modifier:
   - Noise Scale: 5.0
   - Noise Strength: 0.3
   - Frequency: 2.0

3. **Curl** modifier:
   - Curl Amount: 0.4
   - Curl Radius: 1.5
   - Curl Frequency: 3.0

4. **Gravity** modifier:
   - Gravity Strength: 0.5
   - Direction: (0, -1, 0)

5. **Smooth** modifier:
   - Smooth Iterations: 2
   - Smooth Strength: 0.5

**Part 5: Grooming**

1. Select Comb brush
   - Size: 5.0
   - Strength: 0.3
2. Comb to refine flow and remove stray hairs
3. Select Smooth brush for final polish
4. Use Cut brush to trim any overly long strands

**Part 6: Painting Variation**

1. Paint Maps panel → Density mask
2. Paint darker values where hair should be thinner (temples, nape)
3. Paint brighter values where hair should be denser (crown)

4. Length mask:
5. Paint shorter lengths at front/sides
6. Paint longer lengths at back

**Part 7: Baking**

1. Bake/Export panel
2. Mode: Hair Cards
3. Settings:
   - Cards per Clump: 2-3
   - Card Width: 0.5 cm
   - Card Segments: 4
4. Click "Bake Hair Cards"
5. Review result in viewport (mode 3)

**Part 8: Export**

1. Export panel
2. Format: FBX
3. Preset: Unreal Engine 5
4. Options:
   - Export Textures: Yes
   - Texture Size: 2048x2048
   - Include Normals: Yes
5. Export to `MyHair_UE5.fbx`

**Result**: Production-ready hair asset with textures for Unreal Engine!

## Troubleshooting

### Build Issues

**Problem**: "Windows App SDK not found"
- **Solution**: Install via NuGet Package Manager or update Visual Studio to include Windows App SDK

**Problem**: "Cannot find DirectX 12 headers"
- **Solution**: Install Windows 11 SDK via Visual Studio Installer → Modify → Individual Components → Windows 11 SDK

**Problem**: "FBX SDK not found"
- **Solution**: Either install FBX SDK to default location, or create property sheet with custom path, or build without FBX export (OBJ/GLTF will still work)

### Runtime Issues

**Problem**: Application crashes on startup
- **Solution**: Ensure DirectX 12 compatible GPU and drivers. Check Windows Event Viewer for details.

**Problem**: Viewport is black
- **Solution**: Update graphics drivers. Check that GPU supports DirectX 12.

**Problem**: Autosave files missing after crash
- **Solution**: Check `%APPDATA%\BrokenArrowHairStudio\autosave\` directory

### Performance Issues

**Problem**: Slow strand generation with high counts
- **Solution**: Reduce density or strand count. Use fewer segments. Enable multithreading in preferences.

**Problem**: Viewport lag when grooming
- **Solution**: Switch to "Guides Only" view mode while grooming, then switch back to preview.

## Architecture

### Module Overview

**BrokenArrowMath**: Core math library
- Vectors (Vector2, Vector3, Vector4)
- Matrices (Matrix4x4)
- Quaternions
- Splines (Catmull-Rom, Bezier with arc-length)
- Noise (Perlin, Simplex)
- BVH spatial acceleration
- Random number generation

**BrokenArrowIO**: File I/O and serialization
- Project format (.bahair): JSON + binary blob
- Mesh importers: OBJ, FBX, GLTF
- Mesh exporters: OBJ, FBX, GLTF, Alembic
- Texture I/O: PNG, TIFF
- Version migration

**BrokenArrowHair**: Hair simulation core
- Hair description: Guides, strands, parameters
- Strand generation: Interpolation, distribution
- Modifier system: Stack architecture, base classes
- Modifiers: Length, Width, Clump, Noise, Curl, Bend, Parting, Smooth, Mirror
- Grooming: Brush system, stroke handling
- Painting: Mask system, texture painting

**BrokenArrowViewport**: Rendering
- DirectX 12 initialization
- Camera system
- Mesh rendering
- Guide/strand rendering
- Gizmo rendering
- Picking and selection
- Overlay rendering

**BrokenArrowHairStudio**: Main application
- WinUI 3 GUI
- Panels: Outliner, Properties, Modifier Stack, Tools, Paint Maps, Bake/Export
- Toolbar and status bar
- Menu system
- Undo/Redo command system
- Autosave and crash recovery
- Error reporting

### Key Design Patterns

- **Command Pattern**: All user actions are commands for undo/redo
- **Observer Pattern**: UI observes data model for updates
- **Strategy Pattern**: Modifier and exporter implementations
- **Factory Pattern**: Creating modifiers, importers, exporters
- **Singleton Pattern**: Application-wide managers (ResourceManager, JobSystem)

### Threading Model

- **Main Thread**: UI and user input
- **Render Thread**: DirectX 12 rendering
- **Worker Threads**: Strand generation, baking, export
- **Job System**: std::jthread pool for parallel tasks

### File Format: .bahair

JSON structure with embedded binary:

```json
{
  "version": "1.0",
  "project": {
    "name": "MyHairProject",
    "created": "2024-01-15T10:30:00Z",
    "modified": "2024-01-15T15:45:00Z"
  },
  "scene": {
    "scalp_meshes": [...],
    "hair_descriptions": [...]
  },
  "binary_offset": 4096,
  "binary_size": 524288
}
```

Binary section contains:
- Vertex data
- Guide curves
- Generated strands (cached)
- Texture data

## Contributing

This is a production tool built for the Broken Arrow Software suite. Contributions are welcome via pull requests.

### Development Guidelines

- Follow C++20 best practices
- Use existing coding style (see `.clang-format`)
- Add unit tests for new math/algorithm code
- Document public APIs with Doxygen comments
- Test on Windows 10 and Windows 11

### Reporting Issues

Please report bugs and feature requests via GitHub Issues with:
- Detailed description
- Steps to reproduce
- Expected vs. actual behavior
- System information (OS, GPU, driver version)
- Log files from `%APPDATA%\BrokenArrowHairStudio\logs\`

## License

See LICENSE file for details.

## Acknowledgments

- Windows App SDK team at Microsoft
- DirectX 12 documentation and samples
- Autodesk FBX SDK
- Alembic project
- tinygltf library
- Inspiration from XGen, Ornatrix, and Yeti

## Contact

For questions and support:
- GitHub Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues
- Website: (coming soon)

---

**Version**: 1.0.0  
**Last Updated**: January 2024  
**Author**: Broken Arrow Software Team