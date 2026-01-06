using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SkinningStudio.Models
{
    public class Theme : INotifyPropertyChanged
    {
        private string _name = "Default";

        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(); }
        }

        public ObservableCollection<ColorToken> ColorTokens { get; set; } = new();
        public Dictionary<string, double> TypographyTokens { get; set; } = new();

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public static Theme CreateDefaultDarkTheme()
        {
            var theme = new Theme { Name = "Dark" };
            theme.ColorTokens.Add(new ColorToken { Name = "Primary", Color = System.Windows.Media.Color.FromRgb(0, 120, 215) });
            theme.ColorTokens.Add(new ColorToken { Name = "Accent", Color = System.Windows.Media.Color.FromRgb(16, 110, 190) });
            theme.ColorTokens.Add(new ColorToken { Name = "Text", Color = System.Windows.Media.Color.FromRgb(255, 255, 255) });
            theme.ColorTokens.Add(new ColorToken { Name = "Background", Color = System.Windows.Media.Color.FromRgb(32, 32, 32) });
            theme.ColorTokens.Add(new ColorToken { Name = "Surface", Color = System.Windows.Media.Color.FromRgb(45, 45, 45) });
            theme.ColorTokens.Add(new ColorToken { Name = "Border", Color = System.Windows.Media.Color.FromRgb(60, 60, 60) });
            theme.ColorTokens.Add(new ColorToken { Name = "TextSecondary", Color = System.Windows.Media.Color.FromRgb(200, 200, 200) });
            theme.ColorTokens.Add(new ColorToken { Name = "Hover", Color = System.Windows.Media.Color.FromRgb(60, 60, 60) });
            
            theme.TypographyTokens["FontSizeSmall"] = 10;
            theme.TypographyTokens["FontSizeNormal"] = 12;
            theme.TypographyTokens["FontSizeLarge"] = 14;
            theme.TypographyTokens["FontSizeHeading"] = 16;

            return theme;
        }
    }
}
