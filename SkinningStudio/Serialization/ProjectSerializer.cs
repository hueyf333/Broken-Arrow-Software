using SkinningStudio.Models;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Windows;
using System.Windows.Media;

namespace SkinningStudio.Serialization
{
    public class ProjectSerializer
    {
        private static readonly JsonSerializerOptions _options = new()
        {
            WriteIndented = true,
            Converters = 
            {
                new ColorJsonConverter(),
                new ThicknessJsonConverter()
            }
        };

        public static async Task<ProjectModel> LoadAsync(string filePath)
        {
            using var stream = File.OpenRead(filePath);
            var data = await JsonSerializer.DeserializeAsync<ProjectData>(stream, _options);
            
            var project = new ProjectModel
            {
                FilePath = filePath,
                IsDirty = false
            };

            if (data != null)
            {
                foreach (var themeData in data.Themes ?? new List<ThemeData>())
                {
                    project.Themes.Add(DeserializeTheme(themeData));
                }

                if (data.Elements != null)
                {
                    foreach (var elementData in data.Elements)
                    {
                        project.RootElements.Add(DeserializeElement(elementData));
                    }
                }

                if (!string.IsNullOrEmpty(data.CurrentThemeName))
                {
                    var theme = project.Themes.FirstOrDefault(t => t.Name == data.CurrentThemeName);
                    if (theme != null)
                        project.CurrentTheme = theme;
                }
            }

            return project;
        }

        public static async Task SaveAsync(ProjectModel project, string filePath)
        {
            var data = new ProjectData
            {
                Themes = project.Themes.Select(SerializeTheme).ToList(),
                Elements = project.RootElements.Select(SerializeElement).ToList(),
                CurrentThemeName = project.CurrentTheme.Name
            };

            using var stream = File.Create(filePath);
            await JsonSerializer.SerializeAsync(stream, data, _options);
            
            project.FilePath = filePath;
            project.IsDirty = false;
        }

        public static async Task ExportThemeAsync(Theme theme, string filePath)
        {
            var themeData = SerializeTheme(theme);
            using var stream = File.Create(filePath);
            await JsonSerializer.SerializeAsync(stream, themeData, _options);
        }

        public static async Task<Theme> ImportThemeAsync(string filePath)
        {
            using var stream = File.OpenRead(filePath);
            var themeData = await JsonSerializer.DeserializeAsync<ThemeData>(stream, _options);
            return themeData != null ? DeserializeTheme(themeData) : Theme.CreateDefaultDarkTheme();
        }

        public static async Task ExportLayoutAsync(IEnumerable<UIElementModel> elements, string filePath)
        {
            var layoutData = elements.Select(SerializeElement).ToList();
            using var stream = File.Create(filePath);
            await JsonSerializer.SerializeAsync(stream, layoutData, _options);
        }

        private static ThemeData SerializeTheme(Theme theme)
        {
            return new ThemeData
            {
                Name = theme.Name,
                ColorTokens = theme.ColorTokens.Select(t => new ColorTokenData 
                { 
                    Name = t.Name, 
                    R = t.Color.R, 
                    G = t.Color.G, 
                    B = t.Color.B, 
                    A = t.Color.A 
                }).ToList(),
                TypographyTokens = new Dictionary<string, double>(theme.TypographyTokens)
            };
        }

        private static Theme DeserializeTheme(ThemeData data)
        {
            var theme = new Theme { Name = data.Name ?? "Untitled" };
            
            foreach (var tokenData in data.ColorTokens ?? new List<ColorTokenData>())
            {
                theme.ColorTokens.Add(new ColorToken
                {
                    Name = tokenData.Name ?? "",
                    Color = Color.FromArgb(tokenData.A, tokenData.R, tokenData.G, tokenData.B)
                });
            }

            foreach (var kvp in data.TypographyTokens ?? new Dictionary<string, double>())
            {
                theme.TypographyTokens[kvp.Key] = kvp.Value;
            }

            return theme;
        }

        private static ElementData SerializeElement(UIElementModel element)
        {
            return new ElementData
            {
                Id = element.Id,
                Name = element.Name,
                ElementType = element.ElementType.ToString(),
                X = element.X,
                Y = element.Y,
                Width = element.Width,
                Height = element.Height,
                Text = element.Text,
                IsVisible = element.IsVisible,
                ZIndex = element.ZIndex,
                Children = element.Children.Select(SerializeElement).ToList()
            };
        }

        private static UIElementModel DeserializeElement(ElementData data)
        {
            var element = new UIElementModel
            {
                Id = data.Id ?? Guid.NewGuid().ToString(),
                Name = data.Name ?? "",
                ElementType = Enum.TryParse<UIElementType>(data.ElementType, out var type) ? type : UIElementType.Panel,
                X = data.X,
                Y = data.Y,
                Width = data.Width,
                Height = data.Height,
                Text = data.Text ?? "",
                IsVisible = data.IsVisible,
                ZIndex = data.ZIndex
            };

            foreach (var childData in data.Children ?? new List<ElementData>())
            {
                var child = DeserializeElement(childData);
                child.Parent = element;
                element.Children.Add(child);
            }

            return element;
        }
    }

    public class ProjectData
    {
        public List<ThemeData>? Themes { get; set; }
        public List<ElementData>? Elements { get; set; }
        public string? CurrentThemeName { get; set; }
    }

    public class ThemeData
    {
        public string? Name { get; set; }
        public List<ColorTokenData>? ColorTokens { get; set; }
        public Dictionary<string, double>? TypographyTokens { get; set; }
    }

    public class ColorTokenData
    {
        public string? Name { get; set; }
        public byte R { get; set; }
        public byte G { get; set; }
        public byte B { get; set; }
        public byte A { get; set; } = 255;
    }

    public class ElementData
    {
        public string? Id { get; set; }
        public string? Name { get; set; }
        public string? ElementType { get; set; }
        public double X { get; set; }
        public double Y { get; set; }
        public double Width { get; set; }
        public double Height { get; set; }
        public string? Text { get; set; }
        public bool IsVisible { get; set; } = true;
        public int ZIndex { get; set; }
        public List<ElementData>? Children { get; set; }
    }

    public class ColorJsonConverter : JsonConverter<Color>
    {
        public override Color Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            var str = reader.GetString();
            return (Color)ColorConverter.ConvertFromString(str ?? "#000000");
        }

        public override void Write(Utf8JsonWriter writer, Color value, JsonSerializerOptions options)
        {
            writer.WriteStringValue($"#{value.A:X2}{value.R:X2}{value.G:X2}{value.B:X2}");
        }
    }

    public class ThicknessJsonConverter : JsonConverter<Thickness>
    {
        public override Thickness Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            var str = reader.GetString();
            var parts = str?.Split(',') ?? new[] { "0" };
            if (parts.Length == 1 && double.TryParse(parts[0], out var uniform))
                return new Thickness(uniform);
            if (parts.Length == 4)
            {
                return new Thickness(
                    double.Parse(parts[0]),
                    double.Parse(parts[1]),
                    double.Parse(parts[2]),
                    double.Parse(parts[3])
                );
            }
            return new Thickness();
        }

        public override void Write(Utf8JsonWriter writer, Thickness value, JsonSerializerOptions options)
        {
            writer.WriteStringValue($"{value.Left},{value.Top},{value.Right},{value.Bottom}");
        }
    }
}
