using BrokenArrowSkinLab.Core.Models;
using BrokenArrowSkinLab.Core.Rendering;
using BrokenArrowSkinLab.Core.IO;
using System.Text;

namespace BrokenArrowSkinLab.Core.Services;

public class SelfTestService
{
    private readonly KnobRenderer _renderer = new();
    private readonly ExportService _exportService = new();

    public async Task<SelfTestResult> RunSelfTestAsync(IProgress<string>? progress = null)
    {
        var result = new SelfTestResult();
        var tempDir = Path.Combine(Path.GetTempPath(), "BrokenArrowSkinLab_SelfTest_" + Guid.NewGuid().ToString("N"));

        try
        {
            Directory.CreateDirectory(tempDir);
            result.AddLog("Created temporary directory: " + tempDir);
            progress?.Report("Created temporary directory");

            // Test 1: Create sample knob
            progress?.Report("Creating sample knob...");
            var knob = CreateSampleKnob();
            result.AddLog($"Created sample knob: {knob.Name}");
            result.TestsPassed++;

            // Test 2: Render frames
            progress?.Report("Rendering frames...");
            var frames = await _renderer.RenderKnobFramesAsync(knob);
            result.AddLog($"Rendered {frames.Count} frames");
            if (frames.Count == knob.FrameCount)
            {
                result.TestsPassed++;
            }
            else
            {
                result.AddError($"Frame count mismatch: expected {knob.FrameCount}, got {frames.Count}");
            }

            // Test 3: Export filmstrip
            progress?.Report("Exporting filmstrip...");
            var filmstripPath = Path.Combine(tempDir, "knob_filmstrip.png");
            await _exportService.ExportFilmstripAsync(frames, filmstripPath, FilmstripOrientation.Vertical);
            if (File.Exists(filmstripPath))
            {
                result.AddLog($"Exported filmstrip: {filmstripPath}");
                result.TestsPassed++;
            }
            else
            {
                result.AddError("Failed to export filmstrip");
            }

            // Test 4: Export sequence
            progress?.Report("Exporting frame sequence...");
            var sequenceDir = Path.Combine(tempDir, "sequence");
            await _exportService.ExportSequenceAsync(frames, sequenceDir, "knob");
            var sequenceFiles = Directory.GetFiles(sequenceDir, "*.png");
            if (sequenceFiles.Length == knob.FrameCount)
            {
                result.AddLog($"Exported {sequenceFiles.Length} sequence files");
                result.TestsPassed++;
            }
            else
            {
                result.AddError($"Sequence file count mismatch: expected {knob.FrameCount}, got {sequenceFiles.Length}");
            }

            // Test 5: Export atlas
            progress?.Report("Exporting atlas...");
            var atlasPath = Path.Combine(tempDir, "knob_atlas.png");
            await _exportService.ExportAtlasAsync(frames, atlasPath);
            if (File.Exists(atlasPath) && File.Exists(Path.ChangeExtension(atlasPath, ".json")))
            {
                result.AddLog($"Exported atlas with metadata: {atlasPath}");
                result.TestsPassed++;
            }
            else
            {
                result.AddError("Failed to export atlas or metadata");
            }

            // Test 6: Save and load knob preset
            progress?.Report("Testing knob preset I/O...");
            var presetPath = Path.Combine(tempDir, "knob_preset.baslknob");
            await ProjectIO.SaveKnobPresetAsync(presetPath, knob);
            var loadedKnob = await ProjectIO.LoadKnobPresetAsync(presetPath);
            if (loadedKnob.Name == knob.Name && loadedKnob.FrameCount == knob.FrameCount)
            {
                result.AddLog("Knob preset I/O test passed");
                result.TestsPassed++;
            }
            else
            {
                result.AddError("Knob preset I/O test failed");
            }

            // Test 7: Create and save sample layout
            progress?.Report("Creating sample layout...");
            var layout = CreateSampleLayout();
            var layoutPath = Path.Combine(tempDir, "layout_template.basllayout");
            await ProjectIO.SaveLayoutTemplateAsync(layoutPath, layout);
            var loadedLayout = await ProjectIO.LoadLayoutTemplateAsync(layoutPath);
            if (loadedLayout.Name == layout.Name && loadedLayout.Controls.Count == layout.Controls.Count)
            {
                result.AddLog("Layout template I/O test passed");
                result.TestsPassed++;
            }
            else
            {
                result.AddError("Layout template I/O test failed");
            }

            // Test 8: Save and load project
            progress?.Report("Testing project I/O...");
            var project = new ProjectModel
            {
                Name = "Self Test Project",
                Knobs = new List<KnobModel> { knob },
                Layouts = new List<LayoutModel> { layout }
            };
            var projectPath = Path.Combine(tempDir, "project.baslproj");
            await ProjectIO.SaveProjectAsync(projectPath, project);
            var loadedProject = await ProjectIO.LoadProjectAsync(projectPath);
            if (loadedProject.Name == project.Name && 
                loadedProject.Knobs.Count == 1 && 
                loadedProject.Layouts.Count == 1)
            {
                result.AddLog("Project I/O test passed");
                result.TestsPassed++;
            }
            else
            {
                result.AddError("Project I/O test failed");
            }

            result.AddLog($"\nAll tests completed. Passed: {result.TestsPassed}/{result.TotalTests}");
            result.Success = result.TestsPassed == result.TotalTests;
        }
        catch (Exception ex)
        {
            result.AddError($"Self-test exception: {ex.Message}");
            result.Success = false;
        }
        finally
        {
            // Cleanup
            try
            {
                if (Directory.Exists(tempDir))
                {
                    Directory.Delete(tempDir, true);
                    result.AddLog("Cleaned up temporary directory");
                }
            }
            catch (Exception ex)
            {
                result.AddLog($"Warning: Failed to cleanup temp directory: {ex.Message}");
            }
        }

        return result;
    }

    private KnobModel CreateSampleKnob()
    {
        return new KnobModel
        {
            Name = "Sample Knob",
            Width = 64,
            Height = 64,
            FrameCount = 32,
            StartAngle = -135,
            EndAngle = 135,
            Layers = new List<LayerModel>
            {
                new LayerModel
                {
                    Name = "Background",
                    Type = LayerType.Background,
                    Color = new ColorModel { R = 60, G = 60, B = 60, A = 255 },
                    Gradient = new GradientModel
                    {
                        Type = GradientType.Radial,
                        Stops = new List<GradientStop>
                        {
                            new GradientStop { Offset = 0, Color = new ColorModel { R = 80, G = 80, B = 80, A = 255 } },
                            new GradientStop { Offset = 1, Color = new ColorModel { R = 40, G = 40, B = 40, A = 255 } }
                        }
                    }
                },
                new LayerModel
                {
                    Name = "Ring",
                    Type = LayerType.Ring,
                    Color = new ColorModel { R = 100, G = 100, B = 100, A = 255 }
                },
                new LayerModel
                {
                    Name = "Ticks",
                    Type = LayerType.Ticks,
                    Color = new ColorModel { R = 200, G = 200, B = 200, A = 255 }
                },
                new LayerModel
                {
                    Name = "Pointer",
                    Type = LayerType.Pointer,
                    Color = new ColorModel { R = 255, G = 100, B = 50, A = 255 }
                },
                new LayerModel
                {
                    Name = "Gloss",
                    Type = LayerType.Gloss,
                    Opacity = 0.3
                }
            }
        };
    }

    private LayoutModel CreateSampleLayout()
    {
        return new LayoutModel
        {
            Name = "Sample Layout",
            Width = 800,
            Height = 600,
            Controls = new List<ControlModel>
            {
                new ControlModel
                {
                    Name = "Volume Knob",
                    Type = ControlType.Knob,
                    Bounds = new RectModel { X = 50, Y = 50, Width = 64, Height = 64 }
                },
                new ControlModel
                {
                    Name = "Pan Knob",
                    Type = ControlType.Knob,
                    Bounds = new RectModel { X = 150, Y = 50, Width = 64, Height = 64 }
                },
                new ControlModel
                {
                    Name = "Bypass Button",
                    Type = ControlType.ButtonToggle,
                    Bounds = new RectModel { X = 250, Y = 50, Width = 40, Height = 40 }
                }
            }
        };
    }
}

public class SelfTestResult
{
    public bool Success { get; set; }
    public int TestsPassed { get; set; }
    public int TotalTests { get; set; } = 8;
    private readonly StringBuilder _log = new();
    private readonly List<string> _errors = new();

    public string Log => _log.ToString();
    public IReadOnlyList<string> Errors => _errors;

    public void AddLog(string message)
    {
        _log.AppendLine(message);
    }

    public void AddError(string error)
    {
        _errors.Add(error);
        AddLog($"ERROR: {error}");
    }
}
