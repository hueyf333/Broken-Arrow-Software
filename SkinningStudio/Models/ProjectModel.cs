using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SkinningStudio.Models
{
    public class ProjectModel : INotifyPropertyChanged
    {
        private string _filePath = string.Empty;
        private bool _isDirty;
        private Theme _currentTheme;

        public ProjectModel()
        {
            _currentTheme = Theme.CreateDefaultDarkTheme();
        }

        public string FilePath
        {
            get => _filePath;
            set { _filePath = value; OnPropertyChanged(); }
        }

        public bool IsDirty
        {
            get => _isDirty;
            set { _isDirty = value; OnPropertyChanged(); }
        }

        public ObservableCollection<UIElementModel> RootElements { get; set; } = new();
        public ObservableCollection<Theme> Themes { get; set; } = new();

        public Theme CurrentTheme
        {
            get => _currentTheme;
            set { _currentTheme = value; OnPropertyChanged(); }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
