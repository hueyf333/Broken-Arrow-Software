using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Media;

namespace SkinningStudio.Models
{
    public class ColorToken : INotifyPropertyChanged
    {
        private string _name = string.Empty;
        private Color _color;

        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(); }
        }

        public Color Color
        {
            get => _color;
            set { _color = value; OnPropertyChanged(); }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
