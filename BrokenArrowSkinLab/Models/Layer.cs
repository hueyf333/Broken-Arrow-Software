using System;
using System.Numerics;

namespace BrokenArrowSkinLab.Models;

/// <summary>
/// Represents a single layer in a skin project
/// </summary>
public class Layer
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public string Name { get; set; } = "New Layer";
    public bool IsVisible { get; set; } = true;
    public double Opacity { get; set; } = 1.0;
    
    // Transform properties
    public Vector2 Position { get; set; } = Vector2.Zero;
    public Vector2 Scale { get; set; } = Vector2.One;
    public float Rotation { get; set; } = 0f;
    
    // Layer type and content
    public LayerType Type { get; set; } = LayerType.Shape;
    public string? TextureFilePath { get; set; }
    
    // Shape properties
    public ShapeType ShapeType { get; set; } = ShapeType.Circle;
    public Vector2 Size { get; set; } = new Vector2(100, 100);
    
    // Color and gradient
    public ColorInfo FillColor { get; set; } = new ColorInfo { R = 128, G = 128, B = 128, A = 255 };
    public ColorInfo StrokeColor { get; set; } = new ColorInfo { R = 0, G = 0, B = 0, A = 255 };
    public float StrokeWidth { get; set; } = 1f;
    public GradientInfo? Gradient { get; set; }
    
    // Effects
    public ShadowInfo? Shadow { get; set; }
    public ReflectionInfo? Reflection { get; set; }
    
    // Blend mode
    public BlendMode BlendMode { get; set; } = BlendMode.Normal;
    
    public Layer Clone()
    {
        return new Layer
        {
            Id = Guid.NewGuid(),
            Name = $"{Name} (Copy)",
            IsVisible = IsVisible,
            Opacity = Opacity,
            Position = Position,
            Scale = Scale,
            Rotation = Rotation,
            Type = Type,
            TextureFilePath = TextureFilePath,
            ShapeType = ShapeType,
            Size = Size,
            FillColor = FillColor,
            StrokeColor = StrokeColor,
            StrokeWidth = StrokeWidth,
            Gradient = Gradient?.Clone(),
            Shadow = Shadow?.Clone(),
            Reflection = Reflection?.Clone(),
            BlendMode = BlendMode
        };
    }
}

public enum LayerType
{
    Shape,
    Texture,
    Text
}

public enum ShapeType
{
    Circle,
    Rectangle,
    RoundedRectangle,
    Ellipse
}

public enum BlendMode
{
    Normal,
    Multiply,
    Screen,
    Overlay,
    Additive
}

public class ColorInfo
{
    public byte R { get; set; }
    public byte G { get; set; }
    public byte B { get; set; }
    public byte A { get; set; } = 255;
}

public class GradientInfo
{
    public GradientType Type { get; set; } = GradientType.Linear;
    public ColorInfo StartColor { get; set; } = new ColorInfo { R = 255, G = 255, B = 255, A = 255 };
    public ColorInfo EndColor { get; set; } = new ColorInfo { R = 0, G = 0, B = 0, A = 255 };
    public float Angle { get; set; } = 0f;
    public Vector2 Center { get; set; } = new Vector2(0.5f, 0.5f);
    
    public GradientInfo Clone()
    {
        return new GradientInfo
        {
            Type = Type,
            StartColor = StartColor,
            EndColor = EndColor,
            Angle = Angle,
            Center = Center
        };
    }
}

public enum GradientType
{
    Linear,
    Radial
}

public class ShadowInfo
{
    public ColorInfo Color { get; set; } = new ColorInfo { R = 0, G = 0, B = 0, A = 128 };
    public Vector2 Offset { get; set; } = new Vector2(2, 2);
    public float BlurRadius { get; set; } = 4f;
    
    public ShadowInfo Clone()
    {
        return new ShadowInfo
        {
            Color = Color,
            Offset = Offset,
            BlurRadius = BlurRadius
        };
    }
}

public class ReflectionInfo
{
    public float Intensity { get; set; } = 0.5f;
    public float Offset { get; set; } = 2f;
    
    public ReflectionInfo Clone()
    {
        return new ReflectionInfo
        {
            Intensity = Intensity,
            Offset = Offset
        };
    }
}
