# Developer Documentation

## Architecture Overview

Broken Arrow Skin Lab follows the MVVM (Model-View-ViewModel) pattern with WinUI 3.

### Project Structure

```
BrokenArrowSkinLab/
├── Models/                      # Data models
│   ├── Layer.cs                 # Layer representation with transforms and effects
│   └── SkinProject.cs           # Complete project with frames and layers
├── ViewModels/                  # Business logic and presentation
│   └── MainViewModel.cs         # Main window view model
├── Services/                    # Business services
│   ├── ProjectSerializationService.cs  # Save/load .basl files
│   └── ExportService.cs         # Export to PNG/sprite sheets
├── Helpers/                     # Utility classes
│   └── Converters.cs            # XAML value converters
├── Assets/                      # Application resources
│   ├── Templates/               # Pre-built project templates
│   └── *.png                    # Application icons and assets
├── MainWindow.xaml              # Main application window
├── App.xaml                     # Application resources and startup
└── Package.appxmanifest         # Application packaging manifest
```

## Core Components

### Models

#### Layer.cs
Represents a single layer in the skin project. Key properties:
- **Transform**: Position, Scale, Rotation
- **Appearance**: Opacity, Visibility, Blend Mode
- **Shape**: Type (Circle, Rectangle, etc.), Size
- **Color**: Fill Color, Stroke Color, Stroke Width
- **Effects**: Gradient, Shadow, Reflection

#### SkinProject.cs
Represents the complete project. Key properties:
- **Canvas**: Width, Height
- **Animation**: Frame Count, Current Frame, Frame Rate
- **Frames**: List of layer lists (one per frame)

### ViewModels

#### MainViewModel.cs
Main application logic using CommunityToolkit.Mvvm. Key responsibilities:
- Layer management (add, remove, duplicate, reorder)
- Frame management (add, remove, navigate)
- Zoom and pan controls
- Save/load project files
- Export operations

Implements `INotifyPropertyChanged` via `ObservableObject` base class.

### Services

#### ProjectSerializationService.cs
Handles project persistence:
- Saves projects to JSON format (.basl extension)
- Loads projects from disk
- Uses System.Text.Json for serialization

#### ExportService.cs
Handles image export:
- Single frame export to PNG
- Sprite sheet export (all frames in grid)
- Uses Win2D (Microsoft.Graphics.Canvas) for rendering

## Data Flow

1. **User Interaction** → MainWindow.xaml
2. **Command Binding** → MainViewModel commands
3. **Model Update** → SkinProject/Layer models
4. **Property Changed** → UI updates automatically via data binding
5. **Save/Export** → Services handle persistence and export

## Key Technologies

### WinUI 3
- Modern Windows UI framework
- XAML-based declarative UI
- Native Windows look and feel
- Hardware-accelerated rendering

### Windows App SDK
- Provides Windows platform features
- Packaging and deployment
- System integration

### Win2D
- Hardware-accelerated 2D graphics API
- Built on Direct2D
- Used for rendering and export

### CommunityToolkit.Mvvm
- Source generators for MVVM boilerplate
- RelayCommand implementation
- ObservableProperty attributes

## Building and Debugging

### Requirements
- Windows 10 version 1809 or later
- Visual Studio 2022 or later
- .NET 8.0 SDK
- Windows App SDK workload

### Build Configuration

The solution supports three platforms:
- **x64**: 64-bit Intel/AMD processors
- **x86**: 32-bit Intel/AMD processors (legacy)
- **ARM64**: ARM-based processors (Surface devices)

### Debug Configuration

1. Set BrokenArrowSkinLab as startup project
2. Select target platform (usually x64)
3. Press F5 to debug

### Release Configuration

For release builds:
1. Select "Release" configuration
2. Build solution
3. Package for deployment (if needed)

## Testing

### Unit Tests

Located in `BrokenArrowSkinLab.Tests/`:
- **LayerTests.cs**: Tests layer creation, cloning, and properties
- **SkinProjectTests.cs**: Tests project initialization and frame management
- **ProjectSerializationTests.cs**: Tests save/load functionality

Run tests:
```powershell
dotnet test BrokenArrowSkinLab.Tests/BrokenArrowSkinLab.Tests.csproj
```

### Test Framework
- MSTest for test execution
- Arrange-Act-Assert pattern

## Extending the Application

### Adding New Layer Types

1. Add enum value to `LayerType` in Layer.cs
2. Update rendering logic in ExportService.cs
3. Add UI controls in MainWindow.xaml properties panel

### Adding New Effects

1. Create effect info class (similar to `ShadowInfo`)
2. Add property to `Layer` class
3. Implement rendering in `ExportService.RenderLayer()`
4. Add UI controls for effect parameters

### Adding New Export Formats

1. Add method to `ExportService`
2. Add command to `MainViewModel`
3. Add menu item in MainWindow.xaml

## Code Conventions

### Naming
- **PascalCase**: Classes, methods, properties, public fields
- **camelCase**: Private fields, parameters, local variables
- **_underscore**: Private fields in view models

### Async/Await
- Use `async`/`await` for I/O operations
- Suffix async methods with `Async`
- Always return `Task` or `Task<T>`

### XAML Binding
- Use `x:Bind` (compiled binding) instead of `Binding` when possible
- Specify `Mode=OneWay` or `Mode=TwoWay` explicitly
- Use converters for complex transformations

## Common Tasks

### Adding a New Command

1. Add `[RelayCommand]` attribute to method in ViewModel:
```csharp
[RelayCommand]
private void MyCommand()
{
    // Implementation
}
```

2. Bind in XAML:
```xml
<Button Command="{x:Bind ViewModel.MyCommandCommand}"/>
```

### Adding a New Property

1. Use `[ObservableProperty]` in ViewModel:
```csharp
[ObservableProperty]
private string _myProperty = "default";
```

2. Bind in XAML:
```xml
<TextBlock Text="{x:Bind ViewModel.MyProperty, Mode=OneWay}"/>
```

### Persisting New Data

1. Add property to model class
2. Serialization is automatic via System.Text.Json
3. Ensure property has public getter/setter

## Performance Considerations

### Rendering
- Win2D uses GPU acceleration
- Rendering happens on export, not during editing
- Large sprite sheets may require time to generate

### Memory
- Each frame contains its own layer list
- Cloning layers duplicates all properties
- Be mindful of animation frame count

### UI Responsiveness
- Long operations should be async
- Consider using `Task.Run()` for CPU-intensive work
- Update UI on completion

## Known Issues and Limitations

1. **Texture Loading**: File picker integration needed for texture layers
2. **Gradient Rendering**: Currently defined but not fully rendered
3. **Undo/Redo**: Not implemented
4. **Layer Groups**: Not supported
5. **Vector Text**: Not implemented

## Future Enhancements

### High Priority
- Undo/redo system
- Real-time canvas rendering with Win2D
- Texture layer file picker integration
- Enhanced gradient editor with visual preview

### Medium Priority
- Layer grouping and masking
- More shape types (polygon, star, etc.)
- Animation preview playback
- Color palette management

### Low Priority
- Plugin system for custom effects
- Scripting support
- Batch export
- Template library expansion

## Debugging Tips

### XAML Issues
- Use Live Visual Tree in Visual Studio
- Check Output window for binding errors
- Use x:Name to access elements in code-behind if needed

### Rendering Issues
- Enable graphics debugging in Visual Studio
- Check Win2D device initialization
- Verify canvas dimensions are valid

### Serialization Issues
- Add `[JsonPropertyName]` attributes if needed
- Check for circular references
- Verify all types are serializable

## Resources

- [WinUI 3 Documentation](https://docs.microsoft.com/en-us/windows/apps/winui/winui3/)
- [Windows App SDK](https://docs.microsoft.com/en-us/windows/apps/windows-app-sdk/)
- [Win2D](https://github.com/microsoft/Win2D)
- [CommunityToolkit.Mvvm](https://learn.microsoft.com/en-us/dotnet/communitytoolkit/mvvm/)

## Support

For issues or questions:
1. Check this documentation
2. Review the README.md
3. Open an issue on GitHub

---

Last Updated: January 2026
