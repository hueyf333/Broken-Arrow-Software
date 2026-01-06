using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Media;

namespace SkinningStudio.Models
{
    public class ElementStyle : INotifyPropertyChanged
    {
        private string _fontFamily = "Segoe UI";
        private double _fontSize = 12;
        private Color _foreground = Colors.White;
        private Color _background = Colors.Transparent;
        private Color _borderColor = Colors.Gray;
        private double _borderThickness = 1;
        private double _cornerRadius;
        private double _opacity = 1.0;
        private string? _backgroundToken;
        private string? _foregroundToken;
        private string? _borderToken;

        public string FontFamily
        {
            get => _fontFamily;
            set { _fontFamily = value; OnPropertyChanged(); }
        }

        public double FontSize
        {
            get => _fontSize;
            set { _fontSize = value; OnPropertyChanged(); }
        }

        public Color Foreground
        {
            get => _foreground;
            set { _foreground = value; OnPropertyChanged(); }
        }

        public Color Background
        {
            get => _background;
            set { _background = value; OnPropertyChanged(); }
        }

        public Color BorderColor
        {
            get => _borderColor;
            set { _borderColor = value; OnPropertyChanged(); }
        }

        public double BorderThickness
        {
            get => _borderThickness;
            set { _borderThickness = value; OnPropertyChanged(); }
        }

        public double CornerRadius
        {
            get => _cornerRadius;
            set { _cornerRadius = value; OnPropertyChanged(); }
        }

        public double Opacity
        {
            get => _opacity;
            set { _opacity = value; OnPropertyChanged(); }
        }

        public string? BackgroundToken
        {
            get => _backgroundToken;
            set { _backgroundToken = value; OnPropertyChanged(); }
        }

        public string? ForegroundToken
        {
            get => _foregroundToken;
            set { _foregroundToken = value; OnPropertyChanged(); }
        }

        public string? BorderToken
        {
            get => _borderToken;
            set { _borderToken = value; OnPropertyChanged(); }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
