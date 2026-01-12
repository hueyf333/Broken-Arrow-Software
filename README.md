# Broken Arrow Hair Curves Studio

A complete desktop application for creating and customizing hairstyles using a "Hair Curves" system modeled after Blender's hair curves workflow. Features curves + guides + procedural modifiers + non-destructive stack for complete hair grooming and export to game engines and DCC tools.

## Features

- **Full 3D Viewport**: Real-time OpenGL rendering with orbit/pan/zoom camera controls
- **Hair Curves System**: Guide curves with strand interpolation
- **Non-Destructive Modifiers**: Length, Width, Clump, Noise, Curl, Bend, Parting, Smooth, Mirror
- **Groom Tools**: Brush-based editing (Comb, Smooth, Cut/Grow, Paint)
- **Mask Painting**: Density, clump, and modifier masks
- **Baking**: Export to game-ready hair cards or strand tubes
- **Multiple Export Formats**: OBJ, GLTF/GLB (with Alembic/FBX noted as future additions)
- **Project System**: Save/load with autosave and crash recovery
- **Undo/Redo**: Full command history for all operations
- **Dockable UI**: Modern dark-themed interface with customizable panels

## Installation

### Prerequisites

- Python 3.11 or higher
- pip package manager

### Install from Source

1. Clone the repository:
```bash
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software
```

2. Create and activate a virtual environment:
```bash
python3 -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

3. Install dependencies:
```bash
pip install -r requirements.txt
```

### Development Install

For development with editable install:
```bash
pip install -e .
```

## Running the Application

### From Source

```bash
python -m hair_curves_studio.app.main
```

Or if installed:
```bash
hair-curves-studio
```

### Quick Start

1. Launch the application
2. **File → Import Scalp...** to load a scalp mesh (OBJ or GLTF)
3. Use the **Outliner** panel to create a Hair Description
4. In the **Groom Tools** panel, click **Generate Guides**
5. Add modifiers in the **Modifier Stack** panel (Clump, Curl, Noise, etc.)
6. Use the **Bake/Export** panel to bake hair to cards or tubes
7. Export to your target format (OBJ, GLTF)

## Usage Walkthrough

### 1. Import Scalp Mesh

The scalp mesh is the base surface where hair will grow from.

- **File → Import Scalp...** and select an OBJ or GLTF file
- Sample mesh included: `hair_curves_studio/assets/sample_scalp.obj`
- The viewport will automatically frame the imported mesh

### 2. Create Hair Description

A Hair Description defines the properties of a hair system:

- In the **Outliner** panel, right-click and select "Create Hair Description"
- Adjust properties in the **Properties** panel:
  - **Density**: Number of hair strands (100-100,000)
  - **Length**: Base hair length
  - **Width Root/Tip**: Hair thickness taper

### 3. Generate Guides

Guide curves control strand generation:

- **Uniform Distribution**: Scatters guides evenly across scalp
- **Density Map**: Uses a texture to control guide placement
- In **Groom Tools** → **Generate Guides**

### 4. Apply Modifiers

Build a non-destructive modifier stack:

- **Modifier Stack** panel → **Add Modifier**
- Available modifiers:
  - **Length**: Scale or offset hair length
  - **Width**: Control thickness profile
  - **Clump**: Group strands together
  - **Noise/Frizz**: Add variation and randomness
  - **Curl**: Create spiral curls
  - **Bend/Gravity**: Simulate gravity and directional forces
  - **Parting**: Create hair parting lines
  - **Smooth**: Relax high-frequency variation
  - **Mirror**: Symmetry across X axis

- Modifiers can be:
  - Reordered via drag-and-drop
  - Enabled/disabled
  - Bypassed temporarily

### 5. Groom with Brushes

Interactive brush tools for fine-tuning:

- **Comb**: Directional brush to guide hair flow
- **Smooth**: Relax and smooth strands
- **Cut/Grow**: Adjust length interactively
- **Clump Paint**: Paint clumping weight
- **Density Paint**: Paint density mask
- **Curl Paint**: Paint curl modifier masks

### 6. Bake Hair

Convert curves to exportable meshes:

#### Hair Cards (Game-Ready)
- Clusters strands into texture-ready strips
- Adjustable strands per card
- Generates UVs for texture mapping
- Best for real-time rendering

#### Strand Tubes
- Converts each strand to a tube mesh
- Adjustable tube sides (3-16)
- Higher fidelity, more polygons
- Best for cinematics

In **Bake/Export** panel:
- Select bake type (Cards or Tubes)
- Adjust settings
- Click **Bake Hair**

### 7. Export

Export baked meshes for your target application:

#### For Unreal Engine:
- **Format**: GLTF or OBJ
- **Target**: Select "Unreal Engine" preset
- Applies correct scale (centimeters) and axis conversion
- Import into Unreal as static mesh
- Create hair material with opacity and normal maps

#### For Daz Studio:
- **Format**: OBJ recommended
- **Target**: Select "Daz Studio" preset
- Applies typical import scale
- Import as geometry
- Apply hair shader in Daz Studio

### Export Formats:
- **OBJ**: Universal, well-supported
- **GLTF/GLB**: Modern, compact, good for Unreal
- **Alembic**: Noted as future addition (use OBJ for now)
- **FBX**: Noted as future addition due to SDK restrictions (use OBJ/GLTF)

## Packaging to Standalone Executable

### Using PyInstaller

1. Install PyInstaller:
```bash
pip install pyinstaller
```

2. Create executable:
```bash
pyinstaller --name "HairCurvesStudio" \
            --windowed \
            --onefile \
            --add-data "hair_curves_studio/app/viewport/shaders:hair_curves_studio/app/viewport/shaders" \
            --add-data "hair_curves_studio/assets:hair_curves_studio/assets" \
            hair_curves_studio/app/main.py
```

3. Find executable in `dist/` directory

### Platform-Specific Notes

**Windows**:
```bash
pyinstaller --name "HairCurvesStudio" --windowed --onefile --icon=icon.ico hair_curves_studio/app/main.py
```

**macOS**:
```bash
pyinstaller --name "HairCurvesStudio" --windowed --onefile --icon=icon.icns hair_curves_studio/app/main.py
```

**Linux**:
```bash
pyinstaller --name "HairCurvesStudio" --windowed --onefile hair_curves_studio/app/main.py
```

## Import into Target Applications

### Unreal Engine Import

1. **Export from Hair Curves Studio**:
   - Bake to hair cards or tubes
   - Export as GLTF (.gltf or .glb)
   - Target preset: "Unreal Engine"

2. **Import to Unreal**:
   - In Unreal Editor: **File → Import**
   - Select exported GLTF file
   - Import as Static Mesh
   - Set import scale to 1.0 (already converted to centimeters)

3. **Create Hair Material**:
   - Create material with Masked blend mode
   - Add texture parameters:
     - Base Color (hair color/texture)
     - Opacity (alpha for hair cards)
     - Normal (for lighting)
   - Connect Opacity to Opacity Mask
   - Enable Two-Sided rendering

4. **Apply to Mesh**:
   - Assign material to imported hair mesh
   - Adjust texture coordinates if needed

### Daz Studio Import

1. **Export from Hair Curves Studio**:
   - Bake to hair cards or tubes
   - Export as OBJ (.obj)
   - Target preset: "Daz Studio"

2. **Import to Daz Studio**:
   - **File → Import** and select OBJ
   - Use default scale settings (or adjust if needed)
   - Import as geometry

3. **Apply Hair Shader**:
   - Select imported hair mesh
   - In Surfaces pane, apply hair shader
   - Adjust shader parameters:
     - Diffuse color (hair color)
     - Specular (shine/highlights)
     - Opacity (if using hair cards)
   - Load texture maps if created

4. **Position and Parent**:
   - Position hair on character head
   - Parent to head bone for animation

### Material and Texture Notes

- **Hair Cards**: Require opacity/alpha maps for transparency
- **Strand Tubes**: Can use solid materials or transparency
- Texture atlas can be created externally (Substance, Photoshop, etc.)
- Normal maps enhance lighting detail

## Troubleshooting

### OpenGL Initialization Failed

**Issue**: Viewport shows black or error on startup

**Solutions**:
- Update graphics drivers
- Ensure OpenGL 3.3+ support
- On Linux: Install mesa-utils and check with `glxinfo | grep "OpenGL version"`
- Try setting environment variable: `export LIBGL_ALWAYS_SOFTWARE=1` (software rendering fallback)

### Dependencies Not Found

**Issue**: Import errors on launch

**Solutions**:
- Ensure virtual environment is activated
- Reinstall dependencies: `pip install -r requirements.txt --force-reinstall`
- Check Python version: `python --version` (needs 3.11+)

### Performance Issues

**Issue**: Slow viewport or lag when editing

**Solutions**:
- Reduce strand count in Properties panel
- Use LOD settings (fewer preview strands)
- Disable high-sample antialiasing
- Close unused panels

### Export File Not Readable

**Issue**: Exported OBJ/GLTF cannot be opened

**Solutions**:
- Check file permissions
- Try different export format
- Verify output path has write access
- Check application logs in `~/.hair_curves_studio/logs/`

### Crash on Import

**Issue**: Application crashes when importing mesh

**Solutions**:
- Verify mesh file is valid (open in Blender/MeshLab first)
- Try converting mesh format
- Check mesh doesn't have excessive polygon count
- Review crash logs

## Development

### Running Tests

```bash
pytest hair_curves_studio/tests/
```

### Code Structure

```
hair_curves_studio/
  app/
    core/           # Core utilities (logger, commands, jobs)
    hair/           # Hair system (curves, guides, strands, modifiers, baking)
    io/             # Import/export functionality
    ui/             # User interface panels
    viewport/       # OpenGL viewport and rendering
  assets/           # Sample meshes, textures, presets
  tests/            # Unit tests
```

### Adding New Modifiers

1. Create modifier class in `app/hair/modifiers.py` inheriting from `Modifier`
2. Implement `apply()`, `get_parameters()`, `set_parameter()` methods
3. Add to modifier registry in `ModifierStack.from_dict()`
4. Add UI in `panels_modifiers.py`

## Known Limitations

- **Alembic Export**: Requires external library with complex dependencies. Use OBJ/GLTF and convert externally.
- **FBX Export**: SDK licensing restrictions. Use OBJ/GLTF (both supported by Unreal and Daz).
- **Animation**: Currently static only. Timeline/animation support planned for future.
- **Advanced Textures**: Texture atlas generation is basic. Use external tools for final textures.

## License

GPL-3.0 License. See LICENSE file for details.

## Credits

Developed by Broken Arrow Software

Uses the following open-source libraries:
- PySide6 (Qt for Python)
- ModernGL (OpenGL rendering)
- NumPy (numerical computing)
- Trimesh (mesh processing)
- pygltflib (GLTF export)

## Contributing

This project is open for contributions under the GPL-3.0 license.

## Support

For issues, questions, or feature requests:
- GitHub Issues: https://github.com/hueyf333/Broken-Arrow-Software/issues
- Check troubleshooting section above
- Review application logs in `~/.hair_curves_studio/logs/`