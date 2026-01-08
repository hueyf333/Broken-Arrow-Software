using BrokenArrowSkinLab.Core.Models;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Effects;

namespace BrokenArrowSkinLab.Core.Rendering;

public class KnobRenderer
{
    public async Task<List<BitmapSource>> RenderKnobFramesAsync(
        KnobModel knob, 
        IProgress<int>? progress = null,
        CancellationToken cancellationToken = default)
    {
        return await Task.Run(() =>
        {
            var frames = new List<BitmapSource>();
            for (int i = 0; i < knob.FrameCount; i++)
            {
                cancellationToken.ThrowIfCancellationRequested();
                
                var frame = RenderFrame(knob, i);
                frames.Add(frame);
                
                progress?.Report((i + 1) * 100 / knob.FrameCount);
            }
            return frames;
        }, cancellationToken);
    }

    public BitmapSource RenderFrame(KnobModel knob, int frameIndex)
    {
        var renderTarget = new RenderTargetBitmap(
            knob.Width, knob.Height, 96, 96, PixelFormats.Pbgra32);

        var drawingVisual = new DrawingVisual();
        using (var context = drawingVisual.RenderOpen())
        {
            var angle = CalculateAngle(knob, frameIndex);
            
            foreach (var layer in knob.Layers.Where(l => l.Visible))
            {
                RenderLayer(context, layer, knob, angle);
            }
        }

        renderTarget.Render(drawingVisual);
        return renderTarget;
    }

    private double CalculateAngle(KnobModel knob, int frameIndex)
    {
        if (knob.FrameCount <= 1)
            return knob.StartAngle;

        var range = knob.EndAngle - knob.StartAngle;
        return knob.StartAngle + (range * frameIndex / (knob.FrameCount - 1));
    }

    private void RenderLayer(DrawingContext context, LayerModel layer, KnobModel knob, double angle)
    {
        context.PushOpacity(layer.Opacity);

        switch (layer.Type)
        {
            case LayerType.Background:
                RenderBackground(context, layer, knob);
                break;
            case LayerType.Ring:
                RenderRing(context, layer, knob);
                break;
            case LayerType.Ticks:
                RenderTicks(context, layer, knob);
                break;
            case LayerType.Pointer:
                RenderPointer(context, layer, knob, angle);
                break;
            case LayerType.Gloss:
                RenderGloss(context, layer, knob);
                break;
            case LayerType.Shadow:
                RenderShadow(context, layer, knob);
                break;
            case LayerType.Text:
                RenderText(context, layer, knob);
                break;
        }

        context.Pop();
    }

    private void RenderBackground(DrawingContext context, LayerModel layer, KnobModel knob)
    {
        var brush = CreateBrush(layer);
        var center = new Point(knob.Width / 2.0, knob.Height / 2.0);
        var radius = Math.Min(knob.Width, knob.Height) / 2.0 - 2;

        if (layer.Shadow != null)
        {
            var effect = new DropShadowEffect
            {
                BlurRadius = layer.Shadow.BlurRadius,
                ShadowDepth = Math.Sqrt(layer.Shadow.OffsetX * layer.Shadow.OffsetX + 
                                       layer.Shadow.OffsetY * layer.Shadow.OffsetY),
                Direction = Math.Atan2(layer.Shadow.OffsetY, layer.Shadow.OffsetX) * 180 / Math.PI,
                Color = ToMediaColor(layer.Shadow.Color),
                Opacity = layer.Shadow.Color.A / 255.0
            };
        }

        context.DrawEllipse(brush, null, center, radius, radius);
    }

    private void RenderRing(DrawingContext context, LayerModel layer, KnobModel knob)
    {
        var brush = CreateBrush(layer);
        var pen = new Pen(brush, 2);
        var center = new Point(knob.Width / 2.0, knob.Height / 2.0);
        var radius = Math.Min(knob.Width, knob.Height) / 2.0 - 4;

        context.DrawEllipse(null, pen, center, radius, radius);
    }

    private void RenderTicks(DrawingContext context, LayerModel layer, KnobModel knob)
    {
        var brush = CreateBrush(layer);
        var pen = new Pen(brush, 1);
        var center = new Point(knob.Width / 2.0, knob.Height / 2.0);
        var radius = Math.Min(knob.Width, knob.Height) / 2.0 - 6;

        int tickCount = 11;
        for (int i = 0; i < tickCount; i++)
        {
            var angle = (knob.StartAngle + (knob.EndAngle - knob.StartAngle) * i / (tickCount - 1)) * Math.PI / 180.0;
            var x1 = center.X + Math.Cos(angle) * (radius - 4);
            var y1 = center.Y + Math.Sin(angle) * (radius - 4);
            var x2 = center.X + Math.Cos(angle) * radius;
            var y2 = center.Y + Math.Sin(angle) * radius;

            context.DrawLine(pen, new Point(x1, y1), new Point(x2, y2));
        }
    }

    private void RenderPointer(DrawingContext context, LayerModel layer, KnobModel knob, double angle)
    {
        var brush = CreateBrush(layer);
        var center = new Point(knob.Width / 2.0, knob.Height / 2.0);
        var radius = Math.Min(knob.Width, knob.Height) / 2.0 - 8;

        var angleRad = angle * Math.PI / 180.0;
        var x = center.X + Math.Cos(angleRad) * radius;
        var y = center.Y + Math.Sin(angleRad) * radius;

        var pen = new Pen(brush, 2);
        context.DrawLine(pen, center, new Point(x, y));
    }

    private void RenderGloss(DrawingContext context, LayerModel layer, KnobModel knob)
    {
        var center = new Point(knob.Width / 2.0, knob.Height / 2.0);
        var radius = Math.Min(knob.Width, knob.Height) / 2.0 - 2;

        var glossBrush = new RadialGradientBrush();
        glossBrush.GradientStops.Add(new System.Windows.Media.GradientStop(Color.FromArgb(80, 255, 255, 255), 0.0));
        glossBrush.GradientStops.Add(new System.Windows.Media.GradientStop(Color.FromArgb(0, 255, 255, 255), 1.0));
        glossBrush.Center = new Point(0.5, 0.3);

        context.DrawEllipse(glossBrush, null, center, radius, radius);
    }

    private void RenderShadow(DrawingContext context, LayerModel layer, KnobModel knob)
    {
        if (layer.Shadow == null) return;

        var brush = new SolidColorBrush(ToMediaColor(layer.Shadow.Color));
        var center = new Point(knob.Width / 2.0 + layer.Shadow.OffsetX, 
                             knob.Height / 2.0 + layer.Shadow.OffsetY);
        var radius = Math.Min(knob.Width, knob.Height) / 2.0 - 2;

        var effect = new BlurEffect { Radius = layer.Shadow.BlurRadius };
        
        context.DrawEllipse(brush, null, center, radius, radius);
    }

    private void RenderText(DrawingContext context, LayerModel layer, KnobModel knob)
    {
        if (layer.Text == null) return;

        var typeface = new Typeface(
            new FontFamily(layer.Text.FontFamily),
            layer.Text.Italic ? FontStyles.Italic : FontStyles.Normal,
            layer.Text.Bold ? FontWeights.Bold : FontWeights.Normal,
            FontStretches.Normal);

        var formattedText = new FormattedText(
            layer.Text.Content,
            System.Globalization.CultureInfo.CurrentCulture,
            FlowDirection.LeftToRight,
            typeface,
            layer.Text.FontSize,
            CreateBrush(layer),
            1.0);

        var x = (knob.Width - formattedText.Width) / 2;
        var y = (knob.Height - formattedText.Height) / 2;

        context.DrawText(formattedText, new Point(x, y));
    }

    private Brush CreateBrush(LayerModel layer)
    {
        if (layer.Gradient != null && layer.Gradient.Stops.Count > 0)
        {
            if (layer.Gradient.Type == GradientType.Linear)
            {
                var angleRad = layer.Gradient.Angle * Math.PI / 180.0;
                var startPoint = new Point(0.5 - Math.Cos(angleRad) * 0.5, 0.5 - Math.Sin(angleRad) * 0.5);
                var endPoint = new Point(0.5 + Math.Cos(angleRad) * 0.5, 0.5 + Math.Sin(angleRad) * 0.5);

                var brush = new LinearGradientBrush
                {
                    StartPoint = startPoint,
                    EndPoint = endPoint
                };

                foreach (var stop in layer.Gradient.Stops)
                {
                    brush.GradientStops.Add(new System.Windows.Media.GradientStop(ToMediaColor(stop.Color), stop.Offset));
                }

                return brush;
            }
            else
            {
                var brush = new RadialGradientBrush();
                foreach (var stop in layer.Gradient.Stops)
                {
                    brush.GradientStops.Add(new System.Windows.Media.GradientStop(ToMediaColor(stop.Color), stop.Offset));
                }
                return brush;
            }
        }

        return new SolidColorBrush(ToMediaColor(layer.Color));
    }

    private Color ToMediaColor(ColorModel color)
    {
        return Color.FromArgb(color.A, color.R, color.G, color.B);
    }
}
