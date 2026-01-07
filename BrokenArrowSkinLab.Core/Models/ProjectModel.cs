using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace BrokenArrowSkinLab.Core.Models;

public class ProjectModel
{
    public string Name { get; set; } = "New Project";
    public string Version { get; set; } = "1.0.0";
    public DateTime Created { get; set; } = DateTime.UtcNow;
    public DateTime Modified { get; set; } = DateTime.UtcNow;
    public List<KnobModel> Knobs { get; set; } = new();
    public List<LayoutModel> Layouts { get; set; } = new();
    public ProjectSettings Settings { get; set; } = new();
}

public class ProjectSettings
{
    public int GridSize { get; set; } = 10;
    public int AutosaveInterval { get; set; } = 300;
    public string DefaultExportFolder { get; set; } = "Exports";
    public string Theme { get; set; } = "Light";
}

public class KnobModel
{
    public string Id { get; set; } = Guid.NewGuid().ToString();
    public string Name { get; set; } = "New Knob";
    public int FrameCount { get; set; } = 64;
    public double StartAngle { get; set; } = -135;
    public double EndAngle { get; set; } = 135;
    public int Width { get; set; } = 64;
    public int Height { get; set; } = 64;
    public List<LayerModel> Layers { get; set; } = new();
}

public class LayerModel
{
    public string Id { get; set; } = Guid.NewGuid().ToString();
    public string Name { get; set; } = "Layer";
    
    [JsonConverter(typeof(StringEnumConverter))]
    public LayerType Type { get; set; } = LayerType.Background;
    
    public bool Visible { get; set; } = true;
    public double Opacity { get; set; } = 1.0;
    
    [JsonConverter(typeof(StringEnumConverter))]
    public BlendMode BlendMode { get; set; } = BlendMode.Normal;
    
    public ColorModel Color { get; set; } = new() { R = 128, G = 128, B = 128, A = 255 };
    public GradientModel? Gradient { get; set; }
    public double BlurRadius { get; set; } = 0;
    public ShadowModel? Shadow { get; set; }
    public TextModel? Text { get; set; }
    
    // Layer-specific properties
    public Dictionary<string, object> Properties { get; set; } = new();
}

public enum LayerType
{
    Background,
    Ring,
    Ticks,
    Pointer,
    Gloss,
    Shadow,
    Text
}

public enum BlendMode
{
    Normal,
    Multiply,
    Screen
}

public class ColorModel
{
    public byte R { get; set; }
    public byte G { get; set; }
    public byte B { get; set; }
    public byte A { get; set; } = 255;
}

public class GradientModel
{
    public List<GradientStop> Stops { get; set; } = new();
    
    [JsonConverter(typeof(StringEnumConverter))]
    public GradientType Type { get; set; } = GradientType.Linear;
    
    public double Angle { get; set; } = 0;
}

public enum GradientType
{
    Linear,
    Radial
}

public class GradientStop
{
    public double Offset { get; set; }
    public ColorModel Color { get; set; } = new();
}

public class ShadowModel
{
    public double OffsetX { get; set; } = 2;
    public double OffsetY { get; set; } = 2;
    public double BlurRadius { get; set; } = 4;
    public ColorModel Color { get; set; } = new() { R = 0, G = 0, B = 0, A = 128 };
}

public class TextModel
{
    public string Content { get; set; } = "Text";
    public string FontFamily { get; set; } = "Arial";
    public double FontSize { get; set; } = 12;
    public bool Bold { get; set; } = false;
    public bool Italic { get; set; } = false;
}

public class LayoutModel
{
    public string Id { get; set; } = Guid.NewGuid().ToString();
    public string Name { get; set; } = "New Layout";
    public int Width { get; set; } = 800;
    public int Height { get; set; } = 600;
    public string? BackgroundImage { get; set; }
    public List<ControlModel> Controls { get; set; } = new();
}

public class ControlModel
{
    public string Id { get; set; } = Guid.NewGuid().ToString();
    public string Name { get; set; } = "Control";
    
    [JsonConverter(typeof(StringEnumConverter))]
    public ControlType Type { get; set; } = ControlType.Knob;
    
    public RectModel Bounds { get; set; } = new();
    public string? ImageSource { get; set; }
    public List<string> StateImages { get; set; } = new();
    public Dictionary<string, object> Metadata { get; set; } = new();
    
    [JsonConverter(typeof(StringEnumConverter))]
    public HitTestShape HitTestShape { get; set; } = HitTestShape.Rectangle;
}

public enum ControlType
{
    Knob,
    SliderHorizontal,
    SliderVertical,
    ButtonToggle,
    ButtonMomentary,
    SwitchMultiState,
    LEDMeter
}

public enum HitTestShape
{
    Rectangle,
    Ellipse,
    Custom
}

public class RectModel
{
    public double X { get; set; }
    public double Y { get; set; }
    public double Width { get; set; } = 64;
    public double Height { get; set; } = 64;
}
