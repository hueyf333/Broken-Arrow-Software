using BrokenArrowSkinLab.Core.Models;
using System.Windows.Media.Imaging;
using Newtonsoft.Json;

namespace BrokenArrowSkinLab.Core.Services;

public class ExportService
{
    public async Task ExportFilmstripAsync(
        List<BitmapSource> frames, 
        string outputPath, 
        FilmstripOrientation orientation,
        CancellationToken cancellationToken = default)
    {
        await Task.Run(() =>
        {
            if (frames.Count == 0) return;

            int width, height;
            if (orientation == FilmstripOrientation.Vertical)
            {
                width = frames[0].PixelWidth;
                height = frames[0].PixelHeight * frames.Count;
            }
            else
            {
                width = frames[0].PixelWidth * frames.Count;
                height = frames[0].PixelHeight;
            }

            var filmstrip = new RenderTargetBitmap(
                width, height, 96, 96, System.Windows.Media.PixelFormats.Pbgra32);

            var visual = new System.Windows.Media.DrawingVisual();
            using (var context = visual.RenderOpen())
            {
                for (int i = 0; i < frames.Count; i++)
                {
                    cancellationToken.ThrowIfCancellationRequested();

                    var x = orientation == FilmstripOrientation.Horizontal ? i * frames[0].PixelWidth : 0;
                    var y = orientation == FilmstripOrientation.Vertical ? i * frames[0].PixelHeight : 0;

                    context.DrawImage(frames[i], new System.Windows.Rect(x, y, frames[0].PixelWidth, frames[0].PixelHeight));
                }
            }

            filmstrip.Render(visual);
            SaveBitmap(filmstrip, outputPath);
        }, cancellationToken);
    }

    public async Task ExportSequenceAsync(
        List<BitmapSource> frames, 
        string outputFolder, 
        string baseName,
        CancellationToken cancellationToken = default)
    {
        await Task.Run(() =>
        {
            Directory.CreateDirectory(outputFolder);

            for (int i = 0; i < frames.Count; i++)
            {
                cancellationToken.ThrowIfCancellationRequested();
                
                var filename = Path.Combine(outputFolder, $"{baseName}_{i:D4}.png");
                SaveBitmap(frames[i], filename);
            }
        }, cancellationToken);
    }

    public async Task ExportAtlasAsync(
        List<BitmapSource> frames, 
        string outputPath,
        CancellationToken cancellationToken = default)
    {
        await Task.Run(() =>
        {
            if (frames.Count == 0) return;

            var frameWidth = frames[0].PixelWidth;
            var frameHeight = frames[0].PixelHeight;

            // Calculate atlas dimensions (try to make it square-ish)
            var cols = (int)Math.Ceiling(Math.Sqrt(frames.Count));
            var rows = (int)Math.Ceiling((double)frames.Count / cols);

            var atlasWidth = cols * frameWidth;
            var atlasHeight = rows * frameHeight;

            var atlas = new RenderTargetBitmap(
                atlasWidth, atlasHeight, 96, 96, System.Windows.Media.PixelFormats.Pbgra32);

            var visual = new System.Windows.Media.DrawingVisual();
            using (var context = visual.RenderOpen())
            {
                for (int i = 0; i < frames.Count; i++)
                {
                    cancellationToken.ThrowIfCancellationRequested();

                    var col = i % cols;
                    var row = i / cols;
                    var x = col * frameWidth;
                    var y = row * frameHeight;

                    context.DrawImage(frames[i], new System.Windows.Rect(x, y, frameWidth, frameHeight));
                }
            }

            atlas.Render(visual);
            SaveBitmap(atlas, outputPath);

            // Create atlas metadata JSON
            var metadata = new
            {
                frameWidth,
                frameHeight,
                frameCount = frames.Count,
                columns = cols,
                rows,
                frames = Enumerable.Range(0, frames.Count).Select(i => new
                {
                    index = i,
                    x = (i % cols) * frameWidth,
                    y = (i / cols) * frameHeight,
                    width = frameWidth,
                    height = frameHeight
                }).ToList()
            };

            var metadataPath = Path.ChangeExtension(outputPath, ".json");
            File.WriteAllText(metadataPath, JsonConvert.SerializeObject(metadata, Formatting.Indented));
        }, cancellationToken);
    }

    public async Task ExportSkinDescriptorAsync(
        LayoutModel layout, 
        string outputPath,
        double scaleFactor = 1.0)
    {
        var descriptor = new
        {
            name = layout.Name,
            width = (int)(layout.Width * scaleFactor),
            height = (int)(layout.Height * scaleFactor),
            scaleFactor,
            backgroundImage = layout.BackgroundImage,
            controls = layout.Controls.Select(c => new
            {
                id = c.Id,
                name = c.Name,
                type = c.Type.ToString(),
                x = (int)(c.Bounds.X * scaleFactor),
                y = (int)(c.Bounds.Y * scaleFactor),
                width = (int)(c.Bounds.Width * scaleFactor),
                height = (int)(c.Bounds.Height * scaleFactor),
                imageSource = c.ImageSource,
                stateImages = c.StateImages,
                hitTestShape = c.HitTestShape.ToString(),
                metadata = c.Metadata
            }).ToList()
        };

        var json = JsonConvert.SerializeObject(descriptor, Formatting.Indented);
        await File.WriteAllTextAsync(outputPath, json);
    }

    private void SaveBitmap(BitmapSource bitmap, string path)
    {
        var encoder = new PngBitmapEncoder();
        encoder.Frames.Add(BitmapFrame.Create(bitmap));

        using var stream = File.Create(path);
        encoder.Save(stream);
    }
}

public enum FilmstripOrientation
{
    Horizontal,
    Vertical
}
