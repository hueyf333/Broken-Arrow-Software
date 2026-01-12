# Architecture Overview - Broken Arrow Hair Studio

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                   Application Layer                          │
│  ┌────────────┐  ┌──────────────┐  ┌─────────────────┐     │
│  │ Main Window│  │  UI Panels   │  │  Command System │     │
│  │  (WinUI 3) │  │  (Dockable)  │  │   (Undo/Redo)   │     │
│  └────────────┘  └──────────────┘  └─────────────────┘     │
└─────────────────────────────────────────────────────────────┘
                         │
           ┌─────────────┼─────────────┐
           │             │             │
           ▼             ▼             ▼
  ┌────────────┐  ┌────────────┐  ┌────────────┐
  │  Viewport  │  │ Hair System│  │   I/O      │
  │  (DX12)    │  │            │  │            │
  └────────────┘  └────────────┘  └────────────┘
           │             │             │
           │             ▼             ▼
           │      ┌────────────┐  ┌────────────┐
           │      │   Math     │  │Third Party │
           └─────>│  Library   │  │   (FBX,    │
                  │            │  │  Alembic)  │
                  └────────────┘  └────────────┘
```

## Module Details

### 1. Math Library (`BrokenArrowMath`)

**Purpose**: Foundational mathematics for 3D graphics and hair simulation

**Key Components**:
- `Vector2`, `Vector3`, `Vector4`: Vector math
- `Matrix4x4`: Transformations, view/projection
- `Quaternion`: Rotations
- `Spline`: Catmull-Rom and Bezier curves with arc-length parameterization
- `Noise`: Perlin and Simplex noise for procedural variation
- `BVH`: Bounding Volume Hierarchy for spatial queries
- `Random`: Deterministic random number generation

**Dependencies**: None

**Used By**: All other modules

### 2. I/O Library (`BrokenArrowIO`)

**Purpose**: File input/output and serialization

**Key Components**:
- `MeshImporter`/`MeshExporter`: Base classes
- `OBJImporter`/`OBJExporter`: Wavefront OBJ support
- `ProjectFile`: .bahair project format
- `Serialization`: JSON reading/writing

**Dependencies**: Math

**Used By**: Hair, Application

**Future Extensions**:
- FBX importer/exporter (via FBX SDK)
- GLTF importer/exporter (via tinygltf)
- Alembic cache exporter
- PNG/TIFF texture I/O

### 3. Hair System (`BrokenArrowHair`)

**Purpose**: Core hair authoring and simulation

**Key Components**:

#### Data Structures
- `GuideCurve`: Spline-based guide curves
- `HairStrand`: Individual hair strand (points + width)
- `HairDescription`: Complete hair system (guides + strands + modifiers)

#### Generation
- `StrandGenerator`: Interpolate strands from guides
  - Distance-based weighting
  - Orientation-based weighting
  - Deterministic seeding

#### Modifiers
- `Modifier`: Base class (virtual Apply method)
- `LengthModifier`: Scale hair length
- `ClumpModifier`: Group strands together
- `NoiseModifier`: Add Perlin noise
- `GravityModifier`: Apply gravity/bend
- `ModifierStack`: Non-destructive stack with reorder

#### Tools
- `Brush`: Grooming tool base
  - Comb: Direct hair flow
  - Smooth: Relax strands
  - Inflate/Deflate: Volume control
  - Cut: Trim length
  - Grow: Extend length
  - Clump: Local clustering

#### Baking
- `HairBaker`: Convert strands to renderable geometry
  - Hair Cards: Quad strips with UVs
  - Strand Mesh: Tube geometry

**Dependencies**: Math, I/O

**Used By**: Viewport, Application

### 4. Viewport (`BrokenArrowViewport`)

**Purpose**: Real-time DirectX 12 rendering

**Key Components**:

#### Camera
- `Camera`: Orbit/pan/zoom navigation
  - View matrix generation
  - Projection matrix (perspective)
  - Frame-to-fit functionality

#### Rendering
- `DX12Renderer`: DirectX 12 rendering backend
  - Device and swap chain management
  - Command list recording
  - Resource management
  - Mesh rendering
  - Line rendering (guides/strands)
  
- `Viewport`: High-level viewport wrapper
  - Render mode switching
  - Scene management

**Render Modes**:
1. **Guides Only**: Show guide curves
2. **Preview Strands**: Show generated strands as lines
3. **Baked Mesh**: Show final baked geometry

**Dependencies**: Math, Hair, I/O

**Used By**: Application

### 5. Application (`BrokenArrowHairStudio`)

**Purpose**: User interface and application logic

**Key Components**:

#### Application
- `Application`: Singleton application manager
  - Project lifecycle (New/Open/Save)
  - Import/Export
  - Hair description management

#### UI
- `MainWindow`: Win32 main window
  - Menu bar
  - Toolbar
  - Status bar
  - Viewport integration

**Future WinUI 3 Panels** (structure ready):
- **Outliner**: Scene hierarchy tree
- **Properties**: Selected object properties
- **Modifier Stack**: Hair modifier list
- **Groom Tools**: Brush selector and parameters
- **Paint Maps**: Texture painting controls
- **Bake/Export**: Export settings and actions

**Dependencies**: All modules

## Data Flow

### Project Loading
```
File → ProjectFile::Load() → Parse JSON → 
Restore hair descriptions → Load meshes → 
Update viewport
```

### Strand Generation
```
User triggers generate → StrandGenerator::Generate() →
For each density sample point:
  - Find nearest guides
  - Interpolate position and orientation
  - Add variation (seed-based)
  - Create strand points
→ Apply modifier stack → Update viewport
```

### Modifier Application
```
User adds/modifies → ModifierStack::Apply() →
For each modifier (in order):
  - If enabled: modifier->Apply(strands)
→ Update viewport
```

### Baking
```
User clicks bake → HairBaker::BakeToMesh() →
If Hair Cards:
  - Cluster strands by clumps
  - Generate quad strips
  - Create UVs (0-1 along strand, 0-1 across width)
If Strand Mesh:
  - For each strand: generate tube
  - Weld vertices at roots
→ Export mesh
```

### Export Pipeline
```
User selects export → Choose format →
Apply preset (Unreal/Daz):
  - Set scale (cm)
  - Set axis (Z-up/Y-up)
  - Set options (triangulate, merge materials)
→ Exporter::Export() →
Write file (OBJ/FBX/GLTF/ABC)
```

## Performance Considerations

### Bottlenecks
1. **Strand Generation**: O(n × m) where n=density, m=guides
2. **Modifier Application**: O(n × p) where n=strands, p=points per strand
3. **Baking**: O(n × s) where n=strands, s=segments
4. **Rendering**: GPU-bound for high strand counts

### Optimizations

**Current**:
- BVH for scalp raycast (O(log n) vs O(n))
- Spatial hashing for nearest guide lookups
- Incremental updates (only regenerate changed strands)
- Level-of-detail for viewport preview

**Future** (structure ready):
- Job system for parallel strand generation
- SIMD vectorization for math operations
- GPU compute shaders for modifier application
- Frustum culling for rendering

## Threading Model

**Current**: Single-threaded with async hooks

**Designed For**:
```
Main Thread:
  - UI event handling
  - User input processing
  - Viewport updates

Render Thread:
  - DirectX 12 command list recording
  - GPU synchronization

Worker Threads (Job System):
  - Strand generation
  - Modifier application
  - Baking
  - File I/O
```

## Memory Management

### Smart Pointers
- `std::unique_ptr`: Exclusive ownership (modifiers, application state)
- `std::shared_ptr`: Shared ownership (resources)
- Raw pointers: Non-owning references only

### Resource Lifetime
- **Meshes**: Owned by scene, referenced by viewport
- **Hair Descriptions**: Owned by application, referenced by viewport
- **Modifiers**: Owned by modifier stack (unique_ptr in vector)
- **Strands**: Owned by hair description, copied for baking

## Extensibility Points

### Adding New Modifiers
```cpp
class MyModifier : public Modifier {
public:
    std::string GetName() const override { return "My Modifier"; }
    
    void Apply(std::vector<HairStrand>& strands) override {
        // Modify strands
    }
};
```

### Adding New Export Formats
```cpp
class MyExporter : public MeshExporter {
public:
    bool Export(const std::string& filename, const Mesh& mesh) override {
        // Write file
    }
};
```

### Adding New Brushes
```cpp
// Add new BrushType enum value
// Implement in Brush::ApplyToStrand switch statement
```

## Error Handling

### Strategy
- Exceptions for critical errors (file I/O, out of memory)
- Return codes for recoverable errors (import failures)
- Logging for warnings and diagnostics
- UI error dialogs for user-facing errors

### Logging
```cpp
// Future: Centralized logging
Logger::Error("Failed to import mesh");
Logger::Warning("High strand count may impact performance");
Logger::Info("Project saved successfully");
```

## Testing Strategy

### Unit Tests (Future)
- Math: Vector/matrix operations, spline evaluation
- Hair: Strand generation, modifier application
- I/O: OBJ import/export round-trip

### Integration Tests
- Full workflow: Import → Generate → Modify → Bake → Export
- Viewport rendering
- Project save/load

### Performance Tests
- Strand generation with varying density
- Modifier stack with many modifiers
- Large mesh imports

## Security Considerations

- Input validation on all file imports
- Path sanitization for file operations
- Memory bounds checking
- Safe string operations (use std::string, avoid strcpy)
- Integer overflow protection in loops

## Future Enhancements

### Planned Features
1. **Physics Simulation**: Wind, collisions
2. **Animation**: Cache animated hair
3. **Texture Baking**: Ambient occlusion, thickness maps
4. **Advanced Interpolation**: RBF, layered guides
5. **Procedural Distribution**: Texture-driven density
6. **LOD System**: Automatic level-of-detail generation
7. **Scripting**: Python or Lua API

### Architecture Changes Needed
- Plugin system for custom modifiers
- Render pipeline abstraction (support Vulkan, Metal)
- Scripting VM integration
- Network rendering for distributed baking

---

**Document Version**: 1.0  
**Last Updated**: January 2024  
**Maintainer**: Broken Arrow Software Team
