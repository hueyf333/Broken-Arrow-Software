using SkinningStudio.Models;
using System.Collections.ObjectModel;

namespace SkinningStudio.Services
{
    public class ThemeService
    {
        private readonly ObservableCollection<Theme> _themes = new();
        private Theme? _currentTheme;

        public ObservableCollection<Theme> Themes => _themes;

        public Theme? CurrentTheme
        {
            get => _currentTheme;
            set
            {
                _currentTheme = value;
                ThemeChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        public event EventHandler? ThemeChanged;

        public ThemeService()
        {
            var defaultTheme = Theme.CreateDefaultDarkTheme();
            _themes.Add(defaultTheme);
            _currentTheme = defaultTheme;
        }

        public Theme CreateTheme(string name)
        {
            var theme = new Theme { Name = name };
            _themes.Add(theme);
            return theme;
        }

        public Theme DuplicateTheme(Theme source)
        {
            var duplicate = new Theme { Name = source.Name + " Copy" };
            
            foreach (var token in source.ColorTokens)
            {
                duplicate.ColorTokens.Add(new ColorToken { Name = token.Name, Color = token.Color });
            }

            foreach (var kvp in source.TypographyTokens)
            {
                duplicate.TypographyTokens[kvp.Key] = kvp.Value;
            }

            _themes.Add(duplicate);
            return duplicate;
        }

        public void DeleteTheme(Theme theme)
        {
            if (_themes.Count > 1 && _themes.Contains(theme))
            {
                if (_currentTheme == theme)
                {
                    _currentTheme = _themes.FirstOrDefault(t => t != theme);
                }
                _themes.Remove(theme);
            }
        }

        public System.Windows.Media.Color? GetColorFromToken(string? tokenName)
        {
            if (string.IsNullOrEmpty(tokenName) || _currentTheme == null)
                return null;

            var token = _currentTheme.ColorTokens.FirstOrDefault(t => t.Name == tokenName);
            return token?.Color;
        }
    }
}
