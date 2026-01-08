using BrokenArrowSkinLab.Core.Models;
using BrokenArrowSkinLab.Core.IO;
using BrokenArrowSkinLab.Core.Rendering;
using BrokenArrowSkinLab.Core.Services;

namespace BrokenArrowSkinLab.Tests;

public class ProjectIOTests
{
    [Fact]
    public async Task ProjectSaveAndLoad_RoundTrip_Success()
    {
        // Arrange
        var tempFile = Path.GetTempFileName();
        var project = new ProjectModel
        {
            Name = "Test Project",
            Version = "1.0.0",
            Knobs = new List<KnobModel>
            {
                new KnobModel { Name = "Test Knob", FrameCount = 32 }
            },
            Layouts = new List<LayoutModel>
            {
                new LayoutModel { Name = "Test Layout", Width = 800, Height = 600 }
            }
        };

        try
        {
            // Act
            await ProjectIO.SaveProjectAsync(tempFile, project);
            var loaded = await ProjectIO.LoadProjectAsync(tempFile);

            // Assert
            Assert.Equal(project.Name, loaded.Name);
            Assert.Equal(project.Version, loaded.Version);
            Assert.Single(loaded.Knobs);
            Assert.Equal("Test Knob", loaded.Knobs[0].Name);
            Assert.Equal(32, loaded.Knobs[0].FrameCount);
            Assert.Single(loaded.Layouts);
            Assert.Equal("Test Layout", loaded.Layouts[0].Name);
        }
        finally
        {
            if (File.Exists(tempFile))
                File.Delete(tempFile);
        }
    }

    [Fact]
    public async Task KnobPresetSaveAndLoad_RoundTrip_Success()
    {
        // Arrange
        var tempFile = Path.GetTempFileName();
        var knob = new KnobModel
        {
            Name = "Test Knob",
            Width = 64,
            Height = 64,
            FrameCount = 64,
            StartAngle = -135,
            EndAngle = 135,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Name = "Background",
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 128, G = 128, B = 128, A = 255 }
                }
            }
        };

        try
        {
            // Act
            await ProjectIO.SaveKnobPresetAsync(tempFile, knob);
            var loaded = await ProjectIO.LoadKnobPresetAsync(tempFile);

            // Assert
            Assert.Equal(knob.Name, loaded.Name);
            Assert.Equal(knob.Width, loaded.Width);
            Assert.Equal(knob.Height, loaded.Height);
            Assert.Equal(knob.FrameCount, loaded.FrameCount);
            Assert.Equal(knob.StartAngle, loaded.StartAngle);
            Assert.Equal(knob.EndAngle, loaded.EndAngle);
            Assert.Single(loaded.Layers);
            Assert.Equal("Background", loaded.Layers[0].Name);
        }
        finally
        {
            if (File.Exists(tempFile))
                File.Delete(tempFile);
        }
    }

    [Fact]
    public async Task LayoutTemplateSaveAndLoad_RoundTrip_Success()
    {
        // Arrange
        var tempFile = Path.GetTempFileName();
        var layout = new LayoutModel
        {
            Name = "Test Layout",
            Width = 1024,
            Height = 768,
            Controls = new List<ControlModel>
            {
                new ControlModel
                {
                    Name = "Knob1",
                    Type = ControlType.Knob,
                    Bounds = new RectModel { X = 100, Y = 100, Width = 64, Height = 64 }
                },
                new ControlModel
                {
                    Name = "Slider1",
                    Type = ControlType.SliderHorizontal,
                    Bounds = new RectModel { X = 200, Y = 100, Width = 128, Height = 32 }
                }
            }
        };

        try
        {
            // Act
            await ProjectIO.SaveLayoutTemplateAsync(tempFile, layout);
            var loaded = await ProjectIO.LoadLayoutTemplateAsync(tempFile);

            // Assert
            Assert.Equal(layout.Name, loaded.Name);
            Assert.Equal(layout.Width, loaded.Width);
            Assert.Equal(layout.Height, loaded.Height);
            Assert.Equal(2, loaded.Controls.Count);
            Assert.Equal("Knob1", loaded.Controls[0].Name);
            Assert.Equal(ControlType.Knob, loaded.Controls[0].Type);
        }
        finally
        {
            if (File.Exists(tempFile))
                File.Delete(tempFile);
        }
    }
}

public class RenderingTests
{
    [Fact]
    public async Task KnobRenderer_GeneratesCorrectNumberOfFrames()
    {
        // Arrange
        var knob = new KnobModel
        {
            Name = "Test Knob",
            Width = 32,
            Height = 32,
            FrameCount = 16,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Name = "Background",
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 128, G = 128, B = 128, A = 255 }
                }
            }
        };
        var renderer = new KnobRenderer();

        // Act
        var frames = await renderer.RenderKnobFramesAsync(knob);

        // Assert
        Assert.Equal(16, frames.Count);
        Assert.All(frames, frame =>
        {
            Assert.Equal(32, frame.PixelWidth);
            Assert.Equal(32, frame.PixelHeight);
        });
    }

    [Fact]
    public void KnobRenderer_SingleFrame_ProducesValidBitmap()
    {
        // Arrange
        var knob = new KnobModel
        {
            Width = 64,
            Height = 64,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Name = "Background",
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 100, G = 100, B = 100, A = 255 }
                }
            }
        };
        var renderer = new KnobRenderer();

        // Act
        var frame = renderer.RenderFrame(knob, 0);

        // Assert
        Assert.NotNull(frame);
        Assert.Equal(64, frame.PixelWidth);
        Assert.Equal(64, frame.PixelHeight);
    }

    [Fact]
    public async Task KnobRenderer_Determinism_SameInputProducesSameOutput()
    {
        // Arrange
        var knob = new KnobModel
        {
            Width = 32,
            Height = 32,
            FrameCount = 4,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Name = "Background",
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 128, G = 128, B = 128, A = 255 }
                }
            }
        };
        var renderer = new KnobRenderer();

        // Act
        var frames1 = await renderer.RenderKnobFramesAsync(knob);
        var frames2 = await renderer.RenderKnobFramesAsync(knob);

        // Assert
        Assert.Equal(frames1.Count, frames2.Count);
        for (int i = 0; i < frames1.Count; i++)
        {
            Assert.Equal(frames1[i].PixelWidth, frames2[i].PixelWidth);
            Assert.Equal(frames1[i].PixelHeight, frames2[i].PixelHeight);
        }
    }
}

public class ExportServiceTests
{
    [Fact]
    public async Task ExportFilmstrip_Vertical_CreatesValidFile()
    {
        // Arrange
        var knob = new KnobModel
        {
            Width = 32,
            Height = 32,
            FrameCount = 4,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 128, G = 128, B = 128, A = 255 }
                }
            }
        };
        var renderer = new KnobRenderer();
        var frames = await renderer.RenderKnobFramesAsync(knob);
        var exportService = new ExportService();
        var tempFile = Path.GetTempFileName() + ".png";

        try
        {
            // Act
            await exportService.ExportFilmstripAsync(frames, tempFile, FilmstripOrientation.Vertical);

            // Assert
            Assert.True(File.Exists(tempFile));
            var fileInfo = new FileInfo(tempFile);
            Assert.True(fileInfo.Length > 0);
        }
        finally
        {
            if (File.Exists(tempFile))
                File.Delete(tempFile);
        }
    }

    [Fact]
    public async Task ExportSequence_CreatesCorrectNumberOfFiles()
    {
        // Arrange
        var knob = new KnobModel
        {
            Width = 32,
            Height = 32,
            FrameCount = 4,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 128, G = 128, B = 128, A = 255 }
                }
            }
        };
        var renderer = new KnobRenderer();
        var frames = await renderer.RenderKnobFramesAsync(knob);
        var exportService = new ExportService();
        var tempDir = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());

        try
        {
            // Act
            await exportService.ExportSequenceAsync(frames, tempDir, "test");

            // Assert
            Assert.True(Directory.Exists(tempDir));
            var files = Directory.GetFiles(tempDir, "*.png");
            Assert.Equal(4, files.Length);
        }
        finally
        {
            if (Directory.Exists(tempDir))
                Directory.Delete(tempDir, true);
        }
    }

    [Fact]
    public async Task ExportAtlas_CreatesImageAndMetadata()
    {
        // Arrange
        var knob = new KnobModel
        {
            Width = 32,
            Height = 32,
            FrameCount = 4,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 128, G = 128, B = 128, A = 255 }
                }
            }
        };
        var renderer = new KnobRenderer();
        var frames = await renderer.RenderKnobFramesAsync(knob);
        var exportService = new ExportService();
        var tempFile = Path.GetTempFileName() + ".png";

        try
        {
            // Act
            await exportService.ExportAtlasAsync(frames, tempFile);

            // Assert
            Assert.True(File.Exists(tempFile));
            Assert.True(File.Exists(Path.ChangeExtension(tempFile, ".json")));
        }
        finally
        {
            if (File.Exists(tempFile))
                File.Delete(tempFile);
            var jsonFile = Path.ChangeExtension(tempFile, ".json");
            if (File.Exists(jsonFile))
                File.Delete(jsonFile);
        }
    }
}
