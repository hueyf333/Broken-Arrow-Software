# Broken Arrow Skinning Studio

A fully functional WPF (.NET 8) application that provides a Unity IMGUI–inspired, WYSIWYG skinning editor.

## Features

### Editor Layout & Panels
- **Design Canvas**: WYSIWYG editor with grid background
- **Hierarchy Panel**: Tree view of UI structure with visibility toggles
- **Inspector Panel**: Property editor for selected elements
- **Theme Library Panel**: Manage themes and color tokens
- **Live Preview Panel**: Real-time preview with interactive WPF controls

### Supported UI Elements
- Panel
- Button
- Label
- TextBox
- CheckBox
- Slider
- Image

### Canvas Interactions
- **Selection**: Click to select, Ctrl/Shift for multi-select
- **Manipulation**: Drag to move elements
- **Grid & Snap**: Toggle grid display and snap-to-grid (20px)
- **Z-Order**: Bring to front / Send to back

### Theme Management
- Create, duplicate, and delete themes
- Color tokens: Primary, Accent, Text, Background, Surface, Border, etc.
- Typography tokens for font sizes
- Import/Export themes as JSON

### File Operations
- **New Project**: Create new skinning project with sample elements
- **Open Project**: Load .skinstudio files
- **Save/Save As**: Persist projects to disk
- **Import Theme**: Load theme.json
- **Export Theme**: Save current theme as JSON
- **Export Layout**: Save UI structure as layout.json

### Keyboard Shortcuts
- `Ctrl+N`: New Project
- `Ctrl+O`: Open Project
- `Ctrl+S`: Save Project
- `Ctrl+Z`: Undo
- `Ctrl+Y`: Redo
- `Ctrl+A`: Select All
- `Ctrl+D`: Duplicate
- `Delete`: Delete selected element
- `Ctrl+Click`: Multi-select
- `Shift+Click`: Toggle selection

### Undo/Redo
Command-based undo/redo system for all editing operations.

## Building the Project

### Prerequisites
- .NET 8.0 SDK or later
- Windows (for WPF support)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/hueyf333/Broken-Arrow-Software.git
cd Broken-Arrow-Software

# Restore dependencies
dotnet restore SkinningStudio.sln

# Build the solution
dotnet build SkinningStudio.sln

# Run the application
dotnet run --project SkinningStudio/SkinningStudio.csproj
```

## Usage

1. **Launch** the application
2. **Add Elements**: Use toolbar buttons or File menu to add UI elements
3. **Edit Properties**: Select an element and use the Inspector panel to modify properties
4. **Arrange Layout**: Drag elements on the canvas, use Z-order controls
5. **Apply Themes**: Switch themes or create new ones in the Theme Library panel
6. **Preview**: View live rendering in the Live Preview panel
7. **Save Work**: Use Ctrl+S to save your project as .skinstudio file
8. **Export**: Export layout.json and theme.json for use in your applications

## Project Structure

```
SkinningStudio/
├── Models/              # Data models (UIElement, Theme, etc.)
├── Views/               # XAML views and user controls
│   ├── Controls/        # Custom controls
│   └── Dialogs/         # Dialog windows
├── ViewModels/          # MVVM view models
├── Commands/            # Command pattern implementation
├── Services/            # Business logic services
├── Serialization/       # JSON serialization
├── Converters/          # XAML value converters
└── Resources/           # Resource dictionaries and themes
```

## Architecture

The application follows the MVVM (Model-View-ViewModel) pattern:

- **Models**: Represent UI elements, themes, and project state
- **Views**: XAML-based user interface panels
- **ViewModels**: Handle presentation logic and data binding
- **Services**: Provide core functionality (selection, undo/redo, themes)
- **Serialization**: Async JSON-based file I/O using System.Text.Json

## File Formats

### .skinstudio Project File
Contains complete project state including:
- All UI elements with properties
- Themes and color tokens
- Current theme selection

### theme.json
Theme export containing:
- Color tokens (name, RGBA values)
- Typography tokens (font sizes)

### layout.json
UI layout export containing:
- Element hierarchy
- Positions, sizes, and properties

## Dark Theme

The application uses a dark theme by default with the following color scheme:
- Background: `#202020`
- Surface: `#2D2D2D`
- Border: `#3C3C3C`
- Text: `#FFFFFF`
- Primary: `#0078D7`
- Accent: `#106EBE`

## License

See LICENSE file for details.

## Copyright

© 2026 Broken Arrow Software. All rights reserved.
