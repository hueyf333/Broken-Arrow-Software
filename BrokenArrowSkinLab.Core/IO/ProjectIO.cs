using BrokenArrowSkinLab.Core.Models;
using Newtonsoft.Json;

namespace BrokenArrowSkinLab.Core.IO;

public class ProjectIO
{
    private static readonly JsonSerializerSettings SerializerSettings = new()
    {
        Formatting = Formatting.Indented,
        NullValueHandling = NullValueHandling.Ignore
    };

    public static async Task<ProjectModel> LoadProjectAsync(string filePath)
    {
        var json = await File.ReadAllTextAsync(filePath);
        var project = JsonConvert.DeserializeObject<ProjectModel>(json, SerializerSettings);
        return project ?? throw new InvalidOperationException("Failed to deserialize project");
    }

    public static async Task SaveProjectAsync(string filePath, ProjectModel project)
    {
        project.Modified = DateTime.UtcNow;
        var json = JsonConvert.SerializeObject(project, SerializerSettings);
        await File.WriteAllTextAsync(filePath, json);
    }

    public static async Task<KnobModel> LoadKnobPresetAsync(string filePath)
    {
        var json = await File.ReadAllTextAsync(filePath);
        var knob = JsonConvert.DeserializeObject<KnobModel>(json, SerializerSettings);
        return knob ?? throw new InvalidOperationException("Failed to deserialize knob preset");
    }

    public static async Task SaveKnobPresetAsync(string filePath, KnobModel knob)
    {
        var json = JsonConvert.SerializeObject(knob, SerializerSettings);
        await File.WriteAllTextAsync(filePath, json);
    }

    public static async Task<LayoutModel> LoadLayoutTemplateAsync(string filePath)
    {
        var json = await File.ReadAllTextAsync(filePath);
        var layout = JsonConvert.DeserializeObject<LayoutModel>(json, SerializerSettings);
        return layout ?? throw new InvalidOperationException("Failed to deserialize layout template");
    }

    public static async Task SaveLayoutTemplateAsync(string filePath, LayoutModel layout)
    {
        var json = JsonConvert.SerializeObject(layout, SerializerSettings);
        await File.WriteAllTextAsync(filePath, json);
    }
}
