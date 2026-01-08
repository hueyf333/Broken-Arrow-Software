# Quick Start Guide

Get started with Broken Arrow Skin Lab in 5 minutes!

## Installation

### Option 1: Build from Source (Windows Only)

1. **Install Prerequisites**
   - Windows 10 (version 1809+) or Windows 11
   - Visual Studio 2022 with .NET Desktop Development workload
   - .NET 8.0 SDK

2. **Clone and Build**
   ```powershell
   git clone https://github.com/hueyf333/Broken-Arrow-Software.git
   cd Broken-Arrow-Software
   dotnet restore
   dotnet build -c Release
   ```

3. **Run the Application**
   ```powershell
   dotnet run --project BrokenArrowSkinLab/BrokenArrowSkinLab.csproj
   ```

   Or open `BrokenArrowSkinLab.sln` in Visual Studio and press F5.

## First Steps

### Create Your First Knob

1. **Launch the application**
   - The app opens with a default project containing one layer

2. **Add a new layer**
   - Click the **+** button in the Layers panel (left side)
   - Or press `Ctrl+L`

3. **Adjust layer properties** (right panel)
   - **Position**: Move the layer on the canvas
   - **Size**: Change width and height
   - **Shape Type**: Choose Circle, Rectangle, etc.
   - **Opacity**: Adjust transparency

4. **Preview your work**
   - The center canvas shows real-time preview
   - Use zoom controls at the bottom to zoom in/out

5. **Save your project**
   - File → Save Project (`Ctrl+S`)
   - Choose a location and filename
   - Projects are saved as `.basl` files

6. **Export as image**
   - File → Export Single Frame
   - Choose PNG destination
   - Your knob is now ready to use!

### Create an Animated Knob

1. **Add frames**
   - Click **Add Frame** in the frame scrubber (bottom)
   - Repeat to create 32 or 64 frames

2. **Edit each frame**
   - Use the slider to navigate between frames
   - Adjust the pointer layer's rotation for each frame
   - Create smooth rotation animation

3. **Export sprite sheet**
   - File → Export Sprite Sheet
   - All frames are exported in a grid
   - Use in your DAW or game engine

## Essential Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl+N` | New Project |
| `Ctrl+O` | Open Project |
| `Ctrl+S` | Save Project |
| `Ctrl+L` | Add Layer |
| `Ctrl+D` | Duplicate Layer |
| `Delete` | Remove Layer |
| `Ctrl++` | Zoom In |
| `Ctrl+-` | Zoom Out |
| `Ctrl+0` | Reset Zoom |

## Using Templates

### Load a Template

1. File → Open Project
2. Navigate to `BrokenArrowSkinLab/Assets/Templates/`
3. Choose a template:
   - **basic-knob.basl**: Simple 3-layer knob
   - **animated-knob.basl**: 64-frame animation template

### Customize the Template

1. Select layers from the Layers panel
2. Modify properties (colors, sizes, positions)
3. Add or remove layers as needed
4. Save as your own project

## Common Tasks

### Change Layer Color

1. Select the layer
2. In Properties panel (right), locate color sliders
3. Adjust fill color values
4. Layer updates automatically

### Reorder Layers

1. Select a layer
2. Click **Up** or **Down** buttons
3. Higher layers appear on top

### Hide/Show Layers

1. Click the checkbox next to layer name
2. Hidden layers don't appear in preview or export

### Adjust Canvas Size

Currently, canvas size is set when creating a project. Default is 512x512 pixels.

## Tips and Tricks

### Creating Metallic Look
1. Add multiple circular layers
2. Use gradient overlays (configure in layer properties)
3. Add highlight layer with low opacity at top
4. Add shadow layer for depth

### Smooth Animation
- Use 64 frames for smooth rotation
- Export as sprite sheet
- Most DAWs support sprite sheets for knob controls

### Layer Organization
- Name your layers descriptively
- Use "Background", "Body", "Highlight", "Pointer" naming
- Toggle visibility to work on specific parts

### Performance
- Keep frame count reasonable (32-64 for animations)
- Complex shapes render slower
- Test export on a single frame first

## Troubleshooting

### App Won't Start
- Verify Windows version: `winver` (must be 1809+)
- Reinstall .NET 8.0 SDK
- Check Visual Studio has Windows App SDK installed

### Can't Save Project
- Ensure you have write permissions to target folder
- Check available disk space
- Try saving to Documents folder

### Export Creates Blank Image
- Ensure at least one visible layer exists
- Check layer opacity is > 0
- Verify canvas dimensions are valid

### Layers Not Showing
- Check layer visibility (checkbox)
- Verify opacity is not 0
- Check position is within canvas bounds

## Next Steps

Now that you're familiar with the basics:

1. **Explore the Documentation**
   - [README.md](README.md) - Full feature overview
   - [DEVELOPER.md](DEVELOPER.md) - Architecture and extending
   - [BUILD.md](BUILD.md) - Detailed build instructions

2. **Try Advanced Features**
   - Create multi-frame animations
   - Experiment with blend modes
   - Add effects (shadows, reflections)

3. **Share Your Work**
   - Export your creations
   - Use in audio plugins or games
   - Share templates with the community

## Getting Help

- **Documentation**: Check README.md and DEVELOPER.md
- **Issues**: Open a GitHub issue for bugs
- **Templates**: See Assets/Templates/ for examples

## Video Tutorial (Coming Soon)

A video walkthrough will be available showing:
- Creating a basic knob from scratch
- Exporting for use in a DAW
- Creating a 64-frame animation
- Advanced effects and techniques

---

**Have fun creating!**

For complete documentation, see [README.md](README.md)
