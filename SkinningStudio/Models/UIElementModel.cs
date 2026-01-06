using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Media;

namespace SkinningStudio.Models
{
    public class UIElementModel : INotifyPropertyChanged
    {
        private string _name = string.Empty;
        private UIElementType _elementType;
        private double _x;
        private double _y;
        private double _width = 100;
        private double _height = 30;
        private Thickness _margin;
        private Thickness _padding;
        private HorizontalAlignment _horizontalAlignment = HorizontalAlignment.Left;
        private VerticalAlignment _verticalAlignment = VerticalAlignment.Top;
        private string _text = string.Empty;
        private bool _isVisible = true;
        private bool _isLocked;
        private bool _isSelected;
        private int _zIndex;
        private UIElementModel? _parent;
        private Dictionary<StyleState, ElementStyle> _styles = new();

        public string Id { get; set; } = Guid.NewGuid().ToString();

        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(); }
        }

        public UIElementType ElementType
        {
            get => _elementType;
            set { _elementType = value; OnPropertyChanged(); }
        }

        public double X
        {
            get => _x;
            set { _x = value; OnPropertyChanged(); }
        }

        public double Y
        {
            get => _y;
            set { _y = value; OnPropertyChanged(); }
        }

        public double Width
        {
            get => _width;
            set { _width = value; OnPropertyChanged(); }
        }

        public double Height
        {
            get => _height;
            set { _height = value; OnPropertyChanged(); }
        }

        public Thickness Margin
        {
            get => _margin;
            set { _margin = value; OnPropertyChanged(); }
        }

        public Thickness Padding
        {
            get => _padding;
            set { _padding = value; OnPropertyChanged(); }
        }

        public HorizontalAlignment HorizontalAlignment
        {
            get => _horizontalAlignment;
            set { _horizontalAlignment = value; OnPropertyChanged(); }
        }

        public VerticalAlignment VerticalAlignment
        {
            get => _verticalAlignment;
            set { _verticalAlignment = value; OnPropertyChanged(); }
        }

        public string Text
        {
            get => _text;
            set { _text = value; OnPropertyChanged(); }
        }

        public bool IsVisible
        {
            get => _isVisible;
            set { _isVisible = value; OnPropertyChanged(); }
        }

        public bool IsLocked
        {
            get => _isLocked;
            set { _isLocked = value; OnPropertyChanged(); }
        }

        public bool IsSelected
        {
            get => _isSelected;
            set { _isSelected = value; OnPropertyChanged(); }
        }

        public int ZIndex
        {
            get => _zIndex;
            set { _zIndex = value; OnPropertyChanged(); }
        }

        public UIElementModel? Parent
        {
            get => _parent;
            set { _parent = value; OnPropertyChanged(); }
        }

        public ObservableCollection<UIElementModel> Children { get; set; } = new();

        public Dictionary<StyleState, ElementStyle> Styles
        {
            get => _styles;
            set { _styles = value; OnPropertyChanged(); }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
