using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.Effects;
using Windows.Graphics.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;
using BrokenArrowSkinLab.Models;

namespace BrokenArrowSkinLab.Services;

/// <summary>
/// Service for exporting skin projects to image formats
/// </summary>
public class ExportService
{
    public async Task<bool> ExportSingleFrameAsync(SkinProject project, string filePath, int frameIndex = 0)
    {
        try
        {
            using var device = CanvasDevice.GetSharedDevice();
            using var renderTarget = new CanvasRenderTarget(device, project.CanvasWidth, project.CanvasHeight, 96);
            
            using (var session = renderTarget.CreateDrawingSession())
            {
                session.Clear(Windows.UI.Color.FromArgb(0, 0, 0, 0));
                
                if (frameIndex >= 0 && frameIndex < project.Frames.Count)
                {
                    RenderFrame(session, project.Frames[frameIndex], project.CanvasWidth, project.CanvasHeight);
                }
            }

            var file = await StorageFile.GetFileFromPathAsync(filePath);
            using var stream = await file.OpenAsync(FileAccessMode.ReadWrite);
            await renderTarget.SaveAsync(stream, CanvasBitmapFileFormat.Png);
            
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"Error exporting frame: {ex.Message}");
            return false;
        }
    }

    public async Task<bool> ExportSpriteSheetAsync(SkinProject project, string filePath, int columns = 0)
    {
        try
        {
            if (project.Frames.Count == 0)
                return false;

            // Auto-calculate columns if not specified
            if (columns <= 0)
            {
                columns = (int)Math.Ceiling(Math.Sqrt(project.Frames.Count));
            }

            int rows = (int)Math.Ceiling((double)project.Frames.Count / columns);
            int spriteWidth = project.CanvasWidth * columns;
            int spriteHeight = project.CanvasHeight * rows;

            using var device = CanvasDevice.GetSharedDevice();
            using var renderTarget = new CanvasRenderTarget(device, spriteWidth, spriteHeight, 96);
            
            using (var session = renderTarget.CreateDrawingSession())
            {
                session.Clear(Windows.UI.Color.FromArgb(0, 0, 0, 0));
                
                for (int i = 0; i < project.Frames.Count; i++)
                {
                    int col = i % columns;
                    int row = i / columns;
                    
                    session.Transform = System.Numerics.Matrix3x2.CreateTranslation(
                        col * project.CanvasWidth,
                        row * project.CanvasHeight
                    );
                    
                    RenderFrame(session, project.Frames[i], project.CanvasWidth, project.CanvasHeight);
                }
            }

            var file = await StorageFile.GetFileFromPathAsync(filePath);
            using var stream = await file.OpenAsync(FileAccessMode.ReadWrite);
            await renderTarget.SaveAsync(stream, CanvasBitmapFileFormat.Png);
            
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"Error exporting sprite sheet: {ex.Message}");
            return false;
        }
    }

    private void RenderFrame(CanvasDrawingSession session, List<Layer> layers, int width, int height)
    {
        foreach (var layer in layers.Where(l => l.IsVisible))
        {
            RenderLayer(session, layer, width, height);
        }
    }

    private void RenderLayer(CanvasDrawingSession session, Layer layer, int canvasWidth, int canvasHeight)
    {
        var transform = session.Transform;
        
        // Apply layer transform
        var layerTransform = System.Numerics.Matrix3x2.CreateRotation(layer.Rotation, layer.Position) *
                           System.Numerics.Matrix3x2.CreateScale(layer.Scale, layer.Position) *
                           System.Numerics.Matrix3x2.CreateTranslation(layer.Position);
        
        session.Transform = layerTransform * transform;
        
        var color = Windows.UI.Color.FromArgb(
            (byte)(layer.FillColor.A * layer.Opacity),
            layer.FillColor.R,
            layer.FillColor.G,
            layer.FillColor.B
        );

        // Render based on layer type
        switch (layer.Type)
        {
            case LayerType.Shape:
                RenderShape(session, layer, color);
                break;
            case LayerType.Texture:
                // Texture rendering would go here
                break;
        }
        
        session.Transform = transform;
    }

    private void RenderShape(CanvasDrawingSession session, Layer layer, Windows.UI.Color color)
    {
        var rect = new Windows.Foundation.Rect(
            -layer.Size.X / 2,
            -layer.Size.Y / 2,
            layer.Size.X,
            layer.Size.Y
        );

        switch (layer.ShapeType)
        {
            case ShapeType.Circle:
            case ShapeType.Ellipse:
                session.FillEllipse(
                    0, 0,
                    layer.Size.X / 2,
                    layer.Size.Y / 2,
                    color
                );
                if (layer.StrokeWidth > 0)
                {
                    var strokeColor = Windows.UI.Color.FromArgb(
                        layer.StrokeColor.A,
                        layer.StrokeColor.R,
                        layer.StrokeColor.G,
                        layer.StrokeColor.B
                    );
                    session.DrawEllipse(
                        0, 0,
                        layer.Size.X / 2,
                        layer.Size.Y / 2,
                        strokeColor,
                        layer.StrokeWidth
                    );
                }
                break;

            case ShapeType.Rectangle:
                session.FillRectangle(rect, color);
                if (layer.StrokeWidth > 0)
                {
                    var strokeColor = Windows.UI.Color.FromArgb(
                        layer.StrokeColor.A,
                        layer.StrokeColor.R,
                        layer.StrokeColor.G,
                        layer.StrokeColor.B
                    );
                    session.DrawRectangle(rect, strokeColor, layer.StrokeWidth);
                }
                break;

            case ShapeType.RoundedRectangle:
                float radius = Math.Min(layer.Size.X, layer.Size.Y) * 0.1f;
                session.FillRoundedRectangle(rect, radius, radius, color);
                if (layer.StrokeWidth > 0)
                {
                    var strokeColor = Windows.UI.Color.FromArgb(
                        layer.StrokeColor.A,
                        layer.StrokeColor.R,
                        layer.StrokeColor.G,
                        layer.StrokeColor.B
                    );
                    session.DrawRoundedRectangle(rect, radius, radius, strokeColor, layer.StrokeWidth);
                }
                break;
        }
    }
}
