# Quick Start Tutorial - Broken Arrow Hair Studio

## Tutorial 1: Your First Hairstyle (15 minutes)

This tutorial walks you through creating a simple hairstyle from a scalp mesh to final export.

### Prerequisites
- Broken Arrow Hair Studio installed and running
- Sample scalp mesh (included: `assets/meshes/sample_scalp.obj`)

### Step 1: Create New Project (2 min)

1. Launch Broken Arrow Hair Studio
2. **File → New Project**
3. Choose save location: `Documents\BrokenArrow\MyFirstHair.bahair`
4. Project created successfully!

### Step 2: Import Scalp Mesh (3 min)

1. **File → Import Mesh**
2. Navigate to: `[InstallDir]\assets\meshes\sample_scalp.obj`
3. Click **Open**
4. Scalp mesh appears in viewport
5. Use mouse to navigate:
   - **Middle Mouse Button**: Rotate (orbit camera)
   - **Shift + Middle Mouse**: Pan camera
   - **Mouse Wheel**: Zoom in/out
6. Press **F** to frame the mesh in view

### Step 3: Create Hair Description (2 min)

1. In **Outliner** panel, right-click on `sample_scalp`
2. Select **Add Hair Description**
3. Name it: `MainHair`
4. Hair description appears under scalp mesh

### Step 4: Draw Guide Curves (4 min)

1. Select **Draw Guides** tool from toolbar (or press **G**)
2. Click and drag on scalp surface to create guide curves
3. Create 8-10 guides flowing from front to back:
   - Start at front of head
   - Draw toward back
   - Space guides evenly around scalp
4. **Tips:**
   - Longer strokes = longer guides
   - Multiple clicks = more control points
   - Press **Esc** to finish current guide

### Step 5: Generate Hair Strands (2 min)

1. Select `MainHair` in Outliner
2. In **Properties** panel:
   - Density: `5000` (for quick preview)
   - Length: `15.0` cm
   - Length Variation: `0.1`
   - Segments: `12`
   - Seed: `12345`
3. Click **Generate Strands** button
4. Wait 2-5 seconds
5. Hair strands appear in viewport!

### Step 6: Add Modifiers (2 min)

Let's make the hair more realistic:

1. In **Modifier Stack** panel, click **Add Modifier**
2. Select **Clump**
   - Clump Strength: `0.4`
   - Apply
3. Click **Add Modifier** again
4. Select **Noise**
   - Noise Strength: `0.2`
   - Noise Scale: `3.0`
   - Apply
5. See hair update in real-time!

### Step 7: Save and Export (2 min)

1. **File → Save Project** (Ctrl+S)
2. Project saved!

Now export:
1. Switch to **Bake/Export** panel
2. Bake Mode: **Hair Cards**
3. Settings:
   - Cards Per Clump: `2`
   - Card Width: `0.5`
   - Card Segments: `4`
4. Click **Bake Hair Cards**
5. Wait for baking (5-10 seconds)
6. Click **Export**:
   - Format: **OBJ**
   - Location: `Documents\BrokenArrow\MyFirstHair.obj`
   - Click **Export**

**Congratulations!** You've created your first hair asset!

---

## Tutorial 2: Advanced Grooming (20 minutes)

### Setup (5 min)

1. Open the project from Tutorial 1
2. Or create new project and import scalp mesh

### Step 1: Create Detailed Guides (5 min)

For better control:
1. Draw 20-30 guides instead of 10
2. Create guides in layers:
   - First layer: Hairline guides (front)
   - Second layer: Crown guides (top)
   - Third layer: Back guides (nape)
3. Vary guide lengths for natural look

### Step 2: Advanced Modifiers (5 min)

Stack modifiers for complex styles:

1. **Length Modifier**
   - Length Multiplier: `1.2` (20% longer)
   
2. **Clump Modifier**
   - Clump Strength: `0.6`
   
3. **Noise Modifier**
   - Noise Strength: `0.3`
   - Noise Scale: `5.0`
   
4. **Gravity Modifier**
   - Gravity Strength: `0.4`
   - Direction: `(0, -1, 0)` (downward)

Order matters! Reorder by dragging modifiers in stack.

### Step 3: Grooming Brushes (5 min)

Fine-tune hair with brushes:

1. Select **Comb Brush** (toolbar)
   - Radius: `3.0`
   - Strength: `0.4`
2. Brush over hair to direct flow
3. Switch to **Smooth Brush**
   - Radius: `4.0`
   - Strength: `0.3`
4. Smooth out rough areas

**Tips:**
- Enable **X-Symmetry** for symmetric styles
- Smaller radius = more precise control
- Lower strength = subtle changes

### Step 4: Export for Unreal Engine (5 min)

1. **Bake/Export** panel
2. Bake Mode: **Strand Mesh**
3. Settings:
   - Tube Sides: `8`
   - Tube Radius: `0.01`
4. Click **Bake Strand Mesh**
5. Export:
   - Format: **FBX**
   - Preset: **Unreal Engine 5**
   - Options:
     - Export Textures: Yes
     - Include Normals: Yes
   - Export to: `MyHair_UE5.fbx`

### Import to Unreal Engine

1. In Unreal: Content Browser → Import
2. Select `MyHair_UE5.fbx`
3. Import Settings:
   - Skeletal Mesh: No
   - Import Materials: Yes
4. Click Import
5. Hair mesh imported!

---

## Tutorial 3: Painting Variation Maps (15 minutes)

Coming soon...

---

## Common Workflows

### Quick Hair Prototype
1. Import scalp (1 min)
2. Draw 5-8 guides (2 min)
3. Generate with low density (1 min)
4. Export as OBJ (1 min)
**Total: ~5 minutes**

### Production Quality Hair
1. Import scalp (1 min)
2. Draw 30-50 guides (10 min)
3. Generate with high density (2 min)
4. Add 3-5 modifiers (5 min)
5. Groom with brushes (15 min)
6. Paint variation maps (10 min)
7. Bake and export (3 min)
**Total: ~45 minutes**

### Character Hair Set
For a complete character:
1. Create hairline hair description
2. Create main hair description
3. Create bangs/fringe description (optional)
4. Create eyebrows description
5. Create eyelashes description
6. Export each separately
7. Combine in target application
**Total: ~2-3 hours**

---

## Tips & Tricks

### Performance
- Use lower density during editing
- Increase density only for final export
- Switch to "Guides Only" view mode while working

### Visual Quality
- More guides = better interpolation
- Higher segment count = smoother curves
- Add subtle noise for realism
- Use multiple hair descriptions for layers

### Export Best Practices
- Always save project before exporting
- Test export with low settings first
- Keep file sizes reasonable (< 50MB for hair cards)
- Use appropriate formats for target engine

### Keyboard Shortcuts
- **Ctrl+N**: New Project
- **Ctrl+O**: Open Project
- **Ctrl+S**: Save Project
- **F**: Frame Selection
- **G**: Draw Guides Tool
- **1**: Guides Only View
- **2**: Preview Strands View
- **3**: Baked Mesh View
- **Esc**: Cancel Current Operation

---

## Next Steps

- Experiment with different modifier combinations
- Try creating different hairstyles (ponytail, bob, afro)
- Learn about painting density maps
- Export to your favorite game engine

## Support

Questions? Check:
- Main README.md
- BUILD.md for technical issues
- GitHub Issues for community support

---

**Happy Hairstyling!**

*Created with Broken Arrow Hair Studio*
