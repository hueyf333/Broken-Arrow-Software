# .BASL File Format Specification

**Broken Arrow Skin Lab Project File Format**

Version: 1.0  
Extension: `.basl`  
MIME Type: `application/json`

## Overview

The `.basl` file format is a JSON-based format for storing Broken Arrow Skin Lab projects. It contains all information needed to recreate a skin project, including layers, frames, and project settings.

## File Structure

```json
{
  "id": "guid",
  "name": "string",
  "filePath": "string|null",
  "createdDate": "ISO8601 datetime",
  "modifiedDate": "ISO8601 datetime",
  "canvasWidth": "integer",
  "canvasHeight": "integer",
  "frameCount": "integer",
  "currentFrame": "integer",
  "frameRate": "integer",
  "frames": [
    [ /* Layer array for frame 0 */ ],
    [ /* Layer array for frame 1 */ ],
    ...
  ]
}
```

## Top-Level Properties

| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `id` | GUID | Yes | Unique project identifier |
| `name` | string | Yes | Project name |
| `filePath` | string | No | Last saved file path |
| `createdDate` | DateTime | Yes | Project creation timestamp (UTC) |
| `modifiedDate` | DateTime | Yes | Last modification timestamp (UTC) |
| `canvasWidth` | integer | Yes | Canvas width in pixels (default: 512) |
| `canvasHeight` | integer | Yes | Canvas height in pixels (default: 512) |
| `frameCount` | integer | Yes | Total number of animation frames |
| `currentFrame` | integer | Yes | Currently selected frame (0-based) |
| `frameRate` | integer | Yes | Animation frame rate in FPS |
| `frames` | array | Yes | Array of frame arrays, each containing layers |

## Layer Object

Each frame contains an array of layer objects:

```json
{
  "id": "guid",
  "name": "string",
  "isVisible": "boolean",
  "opacity": "number (0-1)",
  "position": { "x": "number", "y": "number" },
  "scale": { "x": "number", "y": "number" },
  "rotation": "number (degrees)",
  "type": "LayerType enum",
  "textureFilePath": "string|null",
  "shapeType": "ShapeType enum",
  "size": { "x": "number", "y": "number" },
  "fillColor": { /* ColorInfo */ },
  "strokeColor": { /* ColorInfo */ },
  "strokeWidth": "number",
  "gradient": { /* GradientInfo|null */ },
  "shadow": { /* ShadowInfo|null */ },
  "reflection": { /* ReflectionInfo|null */ },
  "blendMode": "BlendMode enum"
}
```

### Layer Properties

| Property | Type | Default | Description |
|----------|------|---------|-------------|
| `id` | GUID | Generated | Unique layer identifier |
| `name` | string | "New Layer" | Layer display name |
| `isVisible` | boolean | true | Layer visibility |
| `opacity` | number | 1.0 | Layer opacity (0.0 = transparent, 1.0 = opaque) |
| `position` | Vector2 | {0, 0} | Layer position in pixels |
| `scale` | Vector2 | {1, 1} | Layer scale multiplier |
| `rotation` | number | 0.0 | Rotation in degrees |
| `type` | string | "Shape" | Layer type: "Shape", "Texture", or "Text" |
| `textureFilePath` | string | null | Path to texture image (for Texture layers) |
| `shapeType` | string | "Circle" | Shape: "Circle", "Rectangle", "RoundedRectangle", "Ellipse" |
| `size` | Vector2 | {100, 100} | Shape dimensions in pixels |
| `fillColor` | ColorInfo | Gray | Shape fill color |
| `strokeColor` | ColorInfo | Black | Shape stroke/border color |
| `strokeWidth` | number | 1.0 | Stroke width in pixels |
| `gradient` | GradientInfo | null | Optional gradient overlay |
| `shadow` | ShadowInfo | null | Optional drop shadow |
| `reflection` | ReflectionInfo | null | Optional reflection effect |
| `blendMode` | string | "Normal" | Blend mode: "Normal", "Multiply", "Screen", "Overlay", "Additive" |

## ColorInfo Object

```json
{
  "r": "byte (0-255)",
  "g": "byte (0-255)",
  "b": "byte (0-255)",
  "a": "byte (0-255)"
}
```

| Property | Type | Range | Description |
|----------|------|-------|-------------|
| `r` | integer | 0-255 | Red component |
| `g` | integer | 0-255 | Green component |
| `b` | integer | 0-255 | Blue component |
| `a` | integer | 0-255 | Alpha (opacity) component |

## GradientInfo Object

```json
{
  "type": "GradientType enum",
  "startColor": { /* ColorInfo */ },
  "endColor": { /* ColorInfo */ },
  "angle": "number (degrees)",
  "center": { "x": "number", "y": "number" }
}
```

| Property | Type | Description |
|----------|------|-------------|
| `type` | string | "Linear" or "Radial" |
| `startColor` | ColorInfo | Gradient start color |
| `endColor` | ColorInfo | Gradient end color |
| `angle` | number | Gradient angle in degrees (Linear only) |
| `center` | Vector2 | Gradient center point (Radial only), normalized 0-1 |

## ShadowInfo Object

```json
{
  "color": { /* ColorInfo */ },
  "offset": { "x": "number", "y": "number" },
  "blurRadius": "number"
}
```

| Property | Type | Description |
|----------|------|-------------|
| `color` | ColorInfo | Shadow color |
| `offset` | Vector2 | Shadow offset in pixels |
| `blurRadius` | number | Shadow blur radius in pixels |

## ReflectionInfo Object

```json
{
  "intensity": "number (0-1)",
  "offset": "number"
}
```

| Property | Type | Description |
|----------|------|-------------|
| `intensity` | number | Reflection opacity (0.0-1.0) |
| `offset` | number | Distance from original in pixels |

## Enumerations

### LayerType
- `"Shape"` - Geometric shape layer
- `"Texture"` - Image texture layer
- `"Text"` - Text layer (future)

### ShapeType
- `"Circle"` - Perfect circle
- `"Rectangle"` - Rectangle with sharp corners
- `"RoundedRectangle"` - Rectangle with rounded corners
- `"Ellipse"` - Ellipse shape

### BlendMode
- `"Normal"` - Standard alpha blending
- `"Multiply"` - Multiply blend
- `"Screen"` - Screen blend
- `"Overlay"` - Overlay blend
- `"Additive"` - Additive blend

### GradientType
- `"Linear"` - Linear gradient
- `"Radial"` - Radial gradient

## Example File

### Minimal Project

```json
{
  "id": "a1b2c3d4-e5f6-4a5b-8c7d-9e0f1a2b3c4d",
  "name": "My First Knob",
  "filePath": null,
  "createdDate": "2026-01-08T14:00:00Z",
  "modifiedDate": "2026-01-08T14:00:00Z",
  "canvasWidth": 512,
  "canvasHeight": 512,
  "frameCount": 1,
  "currentFrame": 0,
  "frameRate": 30,
  "frames": [
    [
      {
        "id": "b2c3d4e5-f6a7-4b5c-8d7e-9f0a1b2c3d4e",
        "name": "Background",
        "isVisible": true,
        "opacity": 1.0,
        "position": { "x": 256, "y": 256 },
        "scale": { "x": 1.0, "y": 1.0 },
        "rotation": 0.0,
        "type": "Shape",
        "shapeType": "Circle",
        "size": { "x": 400, "y": 400 },
        "fillColor": { "r": 100, "g": 100, "b": 100, "a": 255 },
        "strokeColor": { "r": 0, "g": 0, "b": 0, "a": 255 },
        "strokeWidth": 2.0,
        "blendMode": "Normal"
      }
    ]
  ]
}
```

### Complete Project with Effects

See `Assets/Templates/basic-knob.basl` for a complete example with multiple layers and effects.

## Version Compatibility

- **Version 1.0**: Initial format (current)
- Future versions will maintain backward compatibility

## File Size Considerations

- Typical single-frame project: 1-5 KB
- 64-frame animation: 50-200 KB
- Add ~10-50 KB per texture reference

## Validation Rules

1. `canvasWidth` and `canvasHeight` must be > 0
2. `frameCount` must equal `frames.length`
3. `currentFrame` must be in range [0, frameCount-1]
4. `opacity` must be in range [0.0, 1.0]
5. Color components (r, g, b, a) must be in range [0, 255]
6. All GUIDs must be unique within a project
7. Enums must match valid values

## Parsing and Generation

### Reading

```csharp
var json = File.ReadAllText("project.basl");
var project = JsonSerializer.Deserialize<SkinProject>(json);
```

### Writing

```csharp
var json = JsonSerializer.Serialize(project, new JsonSerializerOptions {
    WriteIndented = true,
    PropertyNamingPolicy = JsonNamingPolicy.CamelCase
});
File.WriteAllText("project.basl", json);
```

## Migration and Conversion

Future tools may support conversion from:
- KnobMan .knb files
- SkinMan .skn files
- Generic image sequences

## Security Considerations

1. **File Path Validation**: Always validate `textureFilePath` before loading
2. **Size Limits**: Enforce reasonable canvas size limits
3. **Frame Count**: Limit maximum frames to prevent memory issues
4. **Sanitization**: Sanitize all string inputs when loading

## Best Practices

1. Use descriptive layer names
2. Keep frame count reasonable (≤128 for most use cases)
3. Normalize positions to canvas coordinates
4. Use GUIDs for layer identification, not names
5. Store relative paths for textures when possible

## Tools and Libraries

- **Parser**: System.Text.Json (C#)
- **Validator**: JSON Schema validation (optional)
- **Editor**: Broken Arrow Skin Lab (this application)

---

**Specification Version**: 1.0  
**Last Updated**: January 2026
