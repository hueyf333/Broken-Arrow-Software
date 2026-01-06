using SkinningStudio.Commands;
using SkinningStudio.Models;
using SkinningStudio.Serialization;
using SkinningStudio.Services;
using SkinningStudio.Views.Dialogs;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Input;

namespace SkinningStudio.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private readonly UndoRedoService _undoRedoService;
        private readonly SelectionService _selectionService;
        private readonly ThemeService _themeService;
        private ProjectModel _project;
        private bool _showGrid = true;
        private bool _snapToGrid = true;
        private bool _showHierarchy = true;
        private bool _showInspector = true;
        private bool _showThemeLibrary = true;
        private bool _showLivePreview = true;
        private UIElementModel? _selectedElement;

        public MainViewModel()
        {
            _undoRedoService = new UndoRedoService();
            _selectionService = new SelectionService();
            _themeService = new ThemeService();
            _project = new ProjectModel();

            _undoRedoService.StacksChanged += (s, e) =>
            {
                CommandManager.InvalidateRequerySuggested();
            };

            _selectionService.SelectionChanged += (s, e) =>
            {
                SelectedElement = _selectionService.SelectedElements.FirstOrDefault();
            };

            InitializeCommands();
            NewProject();
        }

        public ProjectModel Project
        {
            get => _project;
            set { _project = value; OnPropertyChanged(); }
        }

        public UndoRedoService UndoRedoService => _undoRedoService;
        public SelectionService SelectionService => _selectionService;
        public ThemeService ThemeService => _themeService;

        public bool ShowGrid
        {
            get => _showGrid;
            set { _showGrid = value; OnPropertyChanged(); }
        }

        public bool SnapToGrid
        {
            get => _snapToGrid;
            set { _snapToGrid = value; OnPropertyChanged(); }
        }

        public bool ShowHierarchy
        {
            get => _showHierarchy;
            set { _showHierarchy = value; OnPropertyChanged(); }
        }

        public bool ShowInspector
        {
            get => _showInspector;
            set { _showInspector = value; OnPropertyChanged(); }
        }

        public bool ShowThemeLibrary
        {
            get => _showThemeLibrary;
            set { _showThemeLibrary = value; OnPropertyChanged(); }
        }

        public bool ShowLivePreview
        {
            get => _showLivePreview;
            set { _showLivePreview = value; OnPropertyChanged(); }
        }

        public UIElementModel? SelectedElement
        {
            get => _selectedElement;
            set { _selectedElement = value; OnPropertyChanged(); }
        }

        public ICommand NewCommand { get; private set; } = null!;
        public ICommand OpenCommand { get; private set; } = null!;
        public ICommand SaveCommand { get; private set; } = null!;
        public ICommand SaveAsCommand { get; private set; } = null!;
        public ICommand ExitCommand { get; private set; } = null!;
        public ICommand UndoCommand { get; private set; } = null!;
        public ICommand RedoCommand { get; private set; } = null!;
        public ICommand CutCommand { get; private set; } = null!;
        public ICommand CopyCommand { get; private set; } = null!;
        public ICommand PasteCommand { get; private set; } = null!;
        public ICommand DuplicateCommand { get; private set; } = null!;
        public ICommand DeleteCommand { get; private set; } = null!;
        public ICommand SelectAllCommand { get; private set; } = null!;
        public ICommand AddElementCommand { get; private set; } = null!;
        public ICommand ImportThemeCommand { get; private set; } = null!;
        public ICommand ExportThemeCommand { get; private set; } = null!;
        public ICommand ExportLayoutCommand { get; private set; } = null!;
        public ICommand ToggleGridCommand { get; private set; } = null!;
        public ICommand ToggleSnapCommand { get; private set; } = null!;
        public ICommand BringToFrontCommand { get; private set; } = null!;
        public ICommand SendToBackCommand { get; private set; } = null!;
        public ICommand CreateThemeCommand { get; private set; } = null!;
        public ICommand DuplicateThemeCommand { get; private set; } = null!;
        public ICommand DeleteThemeCommand { get; private set; } = null!;
        public ICommand ShowShortcutsCommand { get; private set; } = null!;
        public ICommand ShowAboutCommand { get; private set; } = null!;

        private void InitializeCommands()
        {
            NewCommand = new RelayCommand(_ => NewProject());
            OpenCommand = new RelayCommand(async _ => await OpenProjectAsync());
            SaveCommand = new RelayCommand(async _ => await SaveProjectAsync(), _ => _project.IsDirty);
            SaveAsCommand = new RelayCommand(async _ => await SaveProjectAsAsync());
            ExitCommand = new RelayCommand(_ => Application.Current.Shutdown());
            
            UndoCommand = new RelayCommand(_ => _undoRedoService.Undo(), _ => _undoRedoService.CanUndo);
            RedoCommand = new RelayCommand(_ => _undoRedoService.Redo(), _ => _undoRedoService.CanRedo);
            
            CutCommand = new RelayCommand(_ => Cut(), _ => _selectionService.SelectedElements.Any());
            CopyCommand = new RelayCommand(_ => Copy(), _ => _selectionService.SelectedElements.Any());
            PasteCommand = new RelayCommand(_ => Paste());
            DuplicateCommand = new RelayCommand(_ => Duplicate(), _ => _selectionService.SelectedElements.Any());
            DeleteCommand = new RelayCommand(_ => Delete(), _ => _selectionService.SelectedElements.Any());
            SelectAllCommand = new RelayCommand(_ => SelectAll());
            
            AddElementCommand = new RelayCommand(param => AddElement(param));
            ImportThemeCommand = new RelayCommand(async _ => await ImportThemeAsync());
            ExportThemeCommand = new RelayCommand(async _ => await ExportThemeAsync());
            ExportLayoutCommand = new RelayCommand(async _ => await ExportLayoutAsync());
            
            ToggleGridCommand = new RelayCommand(_ => ShowGrid = !ShowGrid);
            ToggleSnapCommand = new RelayCommand(_ => SnapToGrid = !SnapToGrid);
            
            BringToFrontCommand = new RelayCommand(_ => BringToFront(), _ => _selectionService.SelectedElements.Any());
            SendToBackCommand = new RelayCommand(_ => SendToBack(), _ => _selectionService.SelectedElements.Any());
            
            CreateThemeCommand = new RelayCommand(_ => CreateTheme());
            DuplicateThemeCommand = new RelayCommand(_ => DuplicateTheme(), _ => _project.CurrentTheme != null);
            DeleteThemeCommand = new RelayCommand(_ => DeleteTheme(), _ => _project.Themes.Count > 1);
            
            ShowShortcutsCommand = new RelayCommand(_ => ShowShortcuts());
            ShowAboutCommand = new RelayCommand(_ => ShowAbout());
        }

        private void NewProject()
        {
            _project = new ProjectModel();
            _project.Themes.Add(Theme.CreateDefaultDarkTheme());
            _project.CurrentTheme = _project.Themes[0];
            _undoRedoService.Clear();
            _selectionService.ClearSelection();
            OnPropertyChanged(nameof(Project));
        }

        private async Task OpenProjectAsync()
        {
            var dialog = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Skin Studio Files (*.skinstudio)|*.skinstudio|All Files (*.*)|*.*",
                DefaultExt = ".skinstudio"
            };

            if (dialog.ShowDialog() == true)
            {
                try
                {
                    _project = await ProjectSerializer.LoadAsync(dialog.FileName);
                    _undoRedoService.Clear();
                    _selectionService.ClearSelection();
                    OnPropertyChanged(nameof(Project));
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Failed to load project: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private async Task SaveProjectAsync()
        {
            if (string.IsNullOrEmpty(_project.FilePath))
            {
                await SaveProjectAsAsync();
                return;
            }

            try
            {
                await ProjectSerializer.SaveAsync(_project, _project.FilePath);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to save project: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private async Task SaveProjectAsAsync()
        {
            var dialog = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Skin Studio Files (*.skinstudio)|*.skinstudio|All Files (*.*)|*.*",
                DefaultExt = ".skinstudio"
            };

            if (dialog.ShowDialog() == true)
            {
                try
                {
                    await ProjectSerializer.SaveAsync(_project, dialog.FileName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Failed to save project: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void AddElement(object? parameter)
        {
            if (parameter is string typeStr && Enum.TryParse<UIElementType>(typeStr, out var type))
            {
                var element = new UIElementModel
                {
                    Name = $"{type}_{_project.RootElements.Count + 1}",
                    ElementType = type,
                    X = 50,
                    Y = 50,
                    Width = type == UIElementType.Button ? 100 : type == UIElementType.TextBox ? 150 : type == UIElementType.Label ? 80 : 100,
                    Height = type == UIElementType.Slider ? 20 : 30,
                    Text = type == UIElementType.Button ? "Button" : type == UIElementType.Label ? "Label" : ""
                };

                _project.RootElements.Add(element);
                _project.IsDirty = true;
                _selectionService.Select(element);
            }
        }

        private void Cut()
        {
            Copy();
            Delete();
        }

        private void Copy()
        {
        }

        private void Paste()
        {
        }

        private void Duplicate()
        {
            var selected = _selectionService.SelectedElements.FirstOrDefault();
            if (selected == null) return;

            var duplicate = new UIElementModel
            {
                Name = selected.Name + "_copy",
                ElementType = selected.ElementType,
                X = selected.X + 10,
                Y = selected.Y + 10,
                Width = selected.Width,
                Height = selected.Height,
                Text = selected.Text
            };

            _project.RootElements.Add(duplicate);
            _project.IsDirty = true;
            _selectionService.Select(duplicate);
        }

        private void Delete()
        {
            var toDelete = _selectionService.SelectedElements.ToList();
            foreach (var element in toDelete)
            {
                _project.RootElements.Remove(element);
            }
            _project.IsDirty = true;
            _selectionService.ClearSelection();
        }

        private void SelectAll()
        {
            _selectionService.SelectAll(_project.RootElements);
        }

        private async Task ImportThemeAsync()
        {
            var dialog = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Theme Files (*.json)|*.json|All Files (*.*)|*.*",
                DefaultExt = ".json"
            };

            if (dialog.ShowDialog() == true)
            {
                try
                {
                    var theme = await ProjectSerializer.ImportThemeAsync(dialog.FileName);
                    _project.Themes.Add(theme);
                    _project.CurrentTheme = theme;
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Failed to import theme: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private async Task ExportThemeAsync()
        {
            var dialog = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Theme Files (*.json)|*.json|All Files (*.*)|*.*",
                DefaultExt = ".json",
                FileName = "theme.json"
            };

            if (dialog.ShowDialog() == true)
            {
                try
                {
                    await ProjectSerializer.ExportThemeAsync(_project.CurrentTheme, dialog.FileName);
                    MessageBox.Show("Theme exported successfully!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Failed to export theme: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private async Task ExportLayoutAsync()
        {
            var dialog = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Layout Files (*.json)|*.json|All Files (*.*)|*.*",
                DefaultExt = ".json",
                FileName = "layout.json"
            };

            if (dialog.ShowDialog() == true)
            {
                try
                {
                    await ProjectSerializer.ExportLayoutAsync(_project.RootElements, dialog.FileName);
                    MessageBox.Show("Layout exported successfully!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Failed to export layout: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void BringToFront()
        {
            var selected = _selectionService.SelectedElements.FirstOrDefault();
            if (selected == null) return;

            var maxZIndex = _project.RootElements.Max(e => e.ZIndex);
            selected.ZIndex = maxZIndex + 1;
            _project.IsDirty = true;
        }

        private void SendToBack()
        {
            var selected = _selectionService.SelectedElements.FirstOrDefault();
            if (selected == null) return;

            var minZIndex = _project.RootElements.Min(e => e.ZIndex);
            selected.ZIndex = minZIndex - 1;
            _project.IsDirty = true;
        }

        private void CreateTheme()
        {
            var theme = _themeService.CreateTheme("New Theme");
            _project.Themes.Add(theme);
            _project.CurrentTheme = theme;
            _project.IsDirty = true;
        }

        private void DuplicateTheme()
        {
            var duplicate = _themeService.DuplicateTheme(_project.CurrentTheme);
            _project.Themes.Add(duplicate);
            _project.CurrentTheme = duplicate;
            _project.IsDirty = true;
        }

        private void DeleteTheme()
        {
            if (_project.Themes.Count > 1)
            {
                var themeToDelete = _project.CurrentTheme;
                var newCurrent = _project.Themes.FirstOrDefault(t => t != themeToDelete);
                if (newCurrent != null)
                {
                    _project.CurrentTheme = newCurrent;
                    _project.Themes.Remove(themeToDelete);
                    _project.IsDirty = true;
                }
            }
        }

        private void ShowShortcuts()
        {
            var dialog = new ShortcutsDialog { Owner = Application.Current.MainWindow };
            dialog.ShowDialog();
        }

        private void ShowAbout()
        {
            var dialog = new AboutDialog { Owner = Application.Current.MainWindow };
            dialog.ShowDialog();
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
