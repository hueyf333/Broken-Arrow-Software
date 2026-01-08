using System;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using BrokenArrowSkinLab.Models;

namespace BrokenArrowSkinLab.Services;

/// <summary>
/// Service for saving and loading project files
/// </summary>
public class ProjectSerializationService
{
    private static readonly JsonSerializerOptions JsonOptions = new()
    {
        WriteIndented = true,
        PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
        Converters = { new JsonStringEnumConverter() }
    };

    public async Task<bool> SaveProjectAsync(SkinProject project, string filePath)
    {
        try
        {
            project.FilePath = filePath;
            project.ModifiedDate = DateTime.UtcNow;
            
            var json = JsonSerializer.Serialize(project, JsonOptions);
            await File.WriteAllTextAsync(filePath, json);
            
            return true;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"Error saving project: {ex.Message}");
            return false;
        }
    }

    public async Task<SkinProject?> LoadProjectAsync(string filePath)
    {
        try
        {
            if (!File.Exists(filePath))
                return null;

            var json = await File.ReadAllTextAsync(filePath);
            var project = JsonSerializer.Deserialize<SkinProject>(json, JsonOptions);
            
            if (project != null)
            {
                project.FilePath = filePath;
            }
            
            return project;
        }
        catch (Exception ex)
        {
            System.Diagnostics.Debug.WriteLine($"Error loading project: {ex.Message}");
            return null;
        }
    }

    public string GetFileFilter()
    {
        return "Broken Arrow Skin Lab Project (*.basl)|*.basl";
    }
}
