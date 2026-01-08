using System;
using System.Collections.Generic;

namespace BrokenArrowSkinLab.Models;

/// <summary>
/// Represents a complete skin project
/// </summary>
public class SkinProject
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public string Name { get; set; } = "Untitled Project";
    public string? FilePath { get; set; }
    public DateTime CreatedDate { get; set; } = DateTime.UtcNow;
    public DateTime ModifiedDate { get; set; } = DateTime.UtcNow;
    
    // Canvas properties
    public int CanvasWidth { get; set; } = 512;
    public int CanvasHeight { get; set; } = 512;
    
    // Animation properties
    public int FrameCount { get; set; } = 1;
    public int CurrentFrame { get; set; } = 0;
    public int FrameRate { get; set; } = 30;
    
    // Layers (per frame)
    public List<List<Layer>> Frames { get; set; } = new();
    
    public SkinProject()
    {
        // Initialize with one frame containing one default layer
        Frames.Add(new List<Layer>
        {
            new Layer
            {
                Name = "Background",
                ShapeType = ShapeType.Circle,
                FillColor = new ColorInfo { R = 100, G = 100, B = 100, A = 255 },
                Size = new System.Numerics.Vector2(400, 400),
                Position = new System.Numerics.Vector2(256, 256)
            }
        });
    }
    
    public List<Layer> GetCurrentFrameLayers()
    {
        if (CurrentFrame >= 0 && CurrentFrame < Frames.Count)
            return Frames[CurrentFrame];
        return new List<Layer>();
    }
}
