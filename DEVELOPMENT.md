# SkinningStudio Development Guide

## Project Structure

```
SkinningStudio/
├── SkinningStudio.sln           # Visual Studio solution
├── vcpkg.json                   # Dependency management
├── vcpkg-configuration.json     # vcpkg configuration
├── README.md                    # User documentation
├── DEVELOPMENT.md              # This file
├── SkinningStudio/             # Main application project
│   ├── SkinningStudio.vcxproj
│   └── src/
│       ├── main.cpp            # Entry point
│       ├── Application.h/cpp   # Main application class
│       ├── Version.h           # Version constant
│       ├── core/               # Core systems
│       │   ├── Common.h        # Shared types and structures
│       │   ├── UIElement.h/cpp # Base UI element class
│       │   ├── StyleSystem.h/cpp # Theme and style resolution
│       │   └── CommandManager.h/cpp # Undo/redo system
│       ├── widgets/            # UI widget implementations
│       │   ├── Button.h/cpp
│       │   ├── Label.h/cpp
│       │   ├── Image.h/cpp
│       │   ├── TextField.h/cpp
│       │   ├── Checkbox.h/cpp
│       │   ├── Slider.h/cpp
│       │   └── Container.h/cpp
│       ├── panels/             # Editor panels
│       │   ├── HierarchyPanel.h/cpp
│       │   ├── InspectorPanel.h/cpp
│       │   ├── AssetLibraryPanel.h/cpp
│       │   ├── CanvasPanel.h/cpp
│       │   └── RuntimePreviewPanel.h/cpp
│       ├── serialization/      # Import/export
│       │   └── ProjectSerializer.h/cpp
│       └── tools/              # Utility tools
│           ├── Validator.h/cpp
│           └── ThemeOptimizer.h/cpp
└── RuntimeSample/              # Runtime demonstration project
    ├── RuntimeSample.vcxproj
    └── src/
        ├── main.cpp
        ├── UIRuntime.h/cpp
```

## Architecture

### Core Components

#### UIElement (core/UIElement.h)
- Base class for all UI widgets
- Manages hierarchy (parent/children relationships)
- Properties: id, name, rect, visibility, enabled
- Layout: anchor, pivot, padding, margin, layoutMode
- Style: styleClass, stateOverrides
- Serialization to/from JSON

#### StyleSystem (core/StyleSystem.h)
- **Theme**: Container for style tokens and widget defaults
- **StyleToken**: Named value with category (color, size, font)
- **StyleResolver**: Resolves property values with precedence:
  1. Element state overrides (highest)
  2. Widget type defaults
  3. Theme tokens (lowest)
- Default dark theme provided

#### CommandManager (core/CommandManager.h)
- Command pattern implementation
- Maintains history stack for undo/redo
- Commands can be:
  - Design changes (move, resize, add, delete)
  - Style changes (property modifications)

### Widget System

All widgets inherit from UIElement and implement:
- `GetType()`: Returns widget type name
- `Render(StyleResolver*)`: Draws the widget using ImGui DrawList
- `Serialize()/Deserialize()`: JSON serialization

**Widget Types:**
- **Button**: Clickable button with text
- **Label**: Static text display
- **Image**: Image placeholder (path-based)
- **TextField**: Text input field
- **Checkbox**: Boolean toggle with label
- **Slider**: Numeric value slider
- **ContainerVertical**: Vertical stack layout
- **ContainerHorizontal**: Horizontal stack layout

### Editor Panels

#### HierarchyPanel
- Tree view of UI element hierarchy
- Drag-and-drop reparenting (placeholder)
- Visibility/lock toggles per element
- Selection handling

#### InspectorPanel
- Property editing for selected element
- Categories: Basic, Transform, Layout, Widget Properties
- Live updates trigger onPropertyChanged callback

#### AssetLibraryPanel
- Theme management interface
- Token editing (color, size, font)
- Widget default style editing
- Import/Export theme functionality

#### CanvasPanel
- Visual editing area with grid
- Pan (middle mouse drag) and zoom support
- Selection with bounding box and resize handles
- Grid toggle and snap-to-grid (toggleable)

#### RuntimePreviewPanel
- Live preview using same rendering code as runtime
- Shows UI as it will appear when exported

### Serialization

#### ProjectSerializer
- **SaveProject**: Exports full project (elements + theme)
- **LoadProject**: Imports project with element reconstruction
- **ExportTheme**: Exports theme.json
- **ImportTheme**: Imports theme.json
- **ExportRuntimePackage**: Generates theme.json + ui_layout.json

Format is JSON-based for human readability and easy integration.

### Tools

#### Validator
- Checks for duplicate IDs
- Validates element IDs are not empty
- Detects cyclic parent-child relationships
- Returns list of issues with severity (Error/Warning/Info)

#### ThemeOptimizer
- Analyzes theme token usage
- Reports unused tokens
- Identifies redundant overrides
- Helps reduce theme file size

## Building

### Prerequisites
- Visual Studio 2022 or later (v143 toolset)
- vcpkg integrated with Visual Studio

### First Build
1. Open SkinningStudio.sln
2. vcpkg will automatically restore dependencies:
   - SDL2
   - GLEW
   - Dear ImGui (with docking-experimental feature)
   - nlohmann-json
3. Build > Build Solution (Ctrl+Shift+B)
4. Output in bin/Debug or bin/Release

### Dependencies via vcpkg

The vcpkg.json manifest includes:
```json
{
  "dependencies": [
    "sdl2",
    "glew",
    {
      "name": "imgui",
      "features": ["docking-experimental", "sdl2-binding", "opengl3-binding"]
    },
    "nlohmann-json"
  ]
}
```

## Code Style

### C++20 Features Used
- `std::shared_ptr` and `std::unique_ptr` for memory management
- `std::function` for callbacks
- `std::optional` for optional values (declared but not used extensively yet)
- Range-based for loops
- Auto type deduction

### Naming Conventions
- Classes: PascalCase (UIElement, CommandManager)
- Methods: PascalCase (AddChild, GetType)
- Member variables: m_prefix + camelCase (m_window, m_commandManager)
- Local variables: camelCase (selectedElement, deltaTime)
- Constants: SCREAMING_SNAKE_CASE (SKINNING_STUDIO_VERSION)

### Memory Management
- UI elements use shared_ptr for safe hierarchy management
- Parent-child relationships use raw pointers for back-references
- Commands use unique_ptr (single ownership)

## Key Implementation Details

### Docking Setup
Application.cpp sets up ImGui docking:
```cpp
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
```

Main dockspace created in Render():
```cpp
ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), 
                 ImGuiDockNodeFlags_PassthruCentralNode);
```

### Rendering Pipeline
1. SDL2 event processing
2. ImGui new frame
3. Render dockspace window
4. Render menu bar
5. Render editor panels
6. Render dialogs
7. Widget rendering via StyleResolver
8. ImGui render to OpenGL
9. SDL2 buffer swap

### Frame Rate Control
Target: 60 FPS (16.67ms per frame)
```cpp
const float targetFrameTime = 1.0f / 60.0f;
// ... frame execution ...
if (frameTime < targetFrameTime) {
    SDL_Delay((Uint32)((targetFrameTime - frameTime) * 1000.0f));
}
```

### Style Resolution
Precedence order (highest to lowest):
1. Element state overrides for current state
2. Widget type defaults from theme
3. Theme tokens (with $ prefix resolution)

Example:
```cpp
// Resolve background color for a button in hover state
Color bgColor = resolver->ResolveColor(button, "backgroundColor", 
                                       WidgetState::Hover);
```

### Coordinate Systems
- **Local coordinates**: Relative to parent element
- **World coordinates**: Absolute screen position (calculated via GetWorldRect())
- Canvas uses world coordinates with pan offset applied

### Undo/Redo Pattern
```cpp
class MyCommand : public Command {
    void Execute() override { /* do action */ }
    void Undo() override { /* undo action */ }
    std::string GetDescription() const override { return "Action"; }
};

// Usage
m_commandManager.ExecuteCommand(std::make_unique<MyCommand>());
```

## Extension Points

### Adding New Widgets
1. Create Widget.h/cpp in widgets/
2. Inherit from UIElement
3. Implement GetType(), Render(), Serialize(), Deserialize()
4. Add to ProjectSerializer::CreateElementFromType()
5. Add default style to Theme::CreateDefaultDarkTheme()

### Adding New Tools
1. Create Tool.h/cpp in tools/
2. Implement analysis/modification logic
3. Add menu item in Application::RenderMenuBar()
4. Create dialog in Application::RenderDialogs()
5. Wire up keyboard shortcut if needed

### Adding Panel Features
- Panel classes are standalone
- Communicate via callbacks (onPropertyChanged, etc.)
- Can maintain internal state
- Render using ImGui immediate mode

## Performance Considerations

### Current Performance
- Target: 60 FPS
- Rendering: Immediate mode (ImGui)
- No complex scene graphs
- Minimal allocations in render loop

### Optimization Opportunities
- Implement dirty flags for re-rendering
- Cache style resolution results
- Frustum culling for large hierarchies
- Texture caching for Image widgets

## Testing Strategy

### Manual Testing Checklist
- [ ] Create new project
- [ ] Add widgets of each type
- [ ] Edit properties
- [ ] Undo/Redo operations
- [ ] Save project
- [ ] Load project (verify identical)
- [ ] Export theme
- [ ] Import theme
- [ ] Export runtime package
- [ ] Run runtime sample with exported data
- [ ] Validate project (check for errors)
- [ ] Optimize theme (check for unused tokens)

### Integration Testing
- RuntimeSample verifies export format compatibility
- Load/save round-trip should preserve all data

## Known Limitations

### Current Implementation
- File dialogs use placeholders (toast messages)
  - Should integrate native file dialogs (e.g., nativefiledialog-extended)
- Drag-and-drop reparenting not fully implemented
- Copy/paste uses placeholder
- Image widget doesn't load actual textures
- Limited undo/redo coverage (framework present, commands need implementation)

### Future Enhancements
- Multi-select with Shift+Click
- Alignment tools (align left/right/top/bottom/center)
- Distribution tools (horizontal/vertical spacing)
- Grouping/ungrouping
- z-order manipulation (bring to front/back)
- Preferences dialog implementation
- Custom widget plugins
- Asset browser for images/fonts
- Animation timeline
- Live preview on mobile device

## Troubleshooting

### Build Errors

**"Cannot find SDL2.h"**
- Ensure vcpkg is integrated: `vcpkg integrate install`
- Check vcpkg.json is in solution directory
- Clean and rebuild solution

**"Unresolved external symbol"**
- Check all .cpp files are included in .vcxproj
- Verify library linking in project properties
- Ensure proper vcpkg integration

**"ImGui docking not available"**
- Verify imgui features in vcpkg.json include "docking-experimental"
- Clean vcpkg cache and rebuild

### Runtime Errors

**"Failed to initialize SDL"**
- Update graphics drivers
- Check SDL2.dll is in output directory

**"OpenGL context creation failed"**
- Verify OpenGL 3.3 support on system
- Update graphics drivers

## Contributing

### Code Review Checklist
- [ ] Follows naming conventions
- [ ] Memory management using smart pointers
- [ ] No raw new/delete
- [ ] Error handling where appropriate
- [ ] Documentation for public APIs
- [ ] Consistent with existing code style

### Pull Request Guidelines
- Small, focused changes
- Clear description of changes
- Test plan included
- Updated documentation if needed

## Resources

### Dependencies Documentation
- [SDL2](https://wiki.libsdl.org/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json)
- [vcpkg](https://vcpkg.io/)

### Useful References
- ImGui Docking: [GitHub Issue #2109](https://github.com/ocornut/imgui/issues/2109)
- ImGui Demo: See imgui_demo.cpp for widget examples
- JSON Serialization: nlohmann/json documentation

## Version History

### v1.0.0 (Current)
- Initial release
- Complete editor with all panels
- 8 widget types
- Theme system with tokens
- Project save/load
- Export runtime package
- Validation and optimization tools
- Comprehensive documentation
