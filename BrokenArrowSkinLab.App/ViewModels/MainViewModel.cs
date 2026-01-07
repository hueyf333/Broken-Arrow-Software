using BrokenArrowSkinLab.App.Commands;
using BrokenArrowSkinLab.App.Infrastructure;
using BrokenArrowSkinLab.App.Services;
using BrokenArrowSkinLab.Core.Models;
using BrokenArrowSkinLab.Core.IO;
using BrokenArrowSkinLab.Core.Services;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Windows.Input;
using Microsoft.Win32;

namespace BrokenArrowSkinLab.App.ViewModels;

public class MainViewModel : ViewModelBase
{
    private readonly UndoRedoService _undoRedoService = new();
    private ProjectModel _currentProject = new();
    private string? _currentFilePath;
    private bool _isDirty;
    private string _statusText = "Ready";
    private object? _selectedWorkspace;

    public MainViewModel()
    {
        InitializeCommands();
        _undoRedoService.StateChanged += (s, e) => UpdateCommandStates();
        
        // Initialize with a new project
        NewProject();
    }

    #region Properties

    public ProjectModel CurrentProject
    {
        get => _currentProject;
        set => SetProperty(ref _currentProject, value);
    }

    public string? CurrentFilePath
    {
        get => _currentFilePath;
        set
        {
            if (SetProperty(ref _currentFilePath, value))
            {
                OnPropertyChanged(nameof(WindowTitle));
            }
        }
    }

    public bool IsDirty
    {
        get => _isDirty;
        set
        {
            if (SetProperty(ref _isDirty, value))
            {
                OnPropertyChanged(nameof(WindowTitle));
            }
        }
    }

    public string StatusText
    {
        get => _statusText;
        set => SetProperty(ref _statusText, value);
    }

    public object? SelectedWorkspace
    {
        get => _selectedWorkspace;
        set => SetProperty(ref _selectedWorkspace, value);
    }

    public string WindowTitle
    {
        get
        {
            var title = "BrokenArrow SkinLab";
            if (CurrentFilePath != null)
            {
                title += $" - {Path.GetFileName(CurrentFilePath)}";
            }
            if (IsDirty)
            {
                title += " *";
            }
            return title;
        }
    }

    public ObservableCollection<KnobModel> Knobs { get; } = new();
    public ObservableCollection<LayoutModel> Layouts { get; } = new();

    #endregion

    #region Commands

    public ICommand NewProjectCommand { get; private set; } = null!;
    public ICommand OpenProjectCommand { get; private set; } = null!;
    public ICommand SaveProjectCommand { get; private set; } = null!;
    public ICommand SaveProjectAsCommand { get; private set; } = null!;
    public ICommand ExitCommand { get; private set; } = null!;

    public ICommand UndoCommand { get; private set; } = null!;
    public ICommand RedoCommand { get; private set; } = null!;
    public ICommand CutCommand { get; private set; } = null!;
    public ICommand CopyCommand { get; private set; } = null!;
    public ICommand PasteCommand { get; private set; } = null!;
    public ICommand DeleteCommand { get; private set; } = null!;

    public ICommand NewKnobCommand { get; private set; } = null!;
    public ICommand NewLayoutCommand { get; private set; } = null!;
    public ICommand RunSelfTestCommand { get; private set; } = null!;
    public ICommand PreferencesCommand { get; private set; } = null!;
    public ICommand AboutCommand { get; private set; } = null!;

    private void InitializeCommands()
    {
        // File menu
        NewProjectCommand = new RelayCommand(_ => NewProject());
        OpenProjectCommand = new RelayCommand(async _ => await OpenProjectAsync());
        SaveProjectCommand = new RelayCommand(async _ => await SaveProjectAsync(), _ => IsDirty);
        SaveProjectAsCommand = new RelayCommand(async _ => await SaveProjectAsAsync());
        ExitCommand = new RelayCommand(_ => Application.Current.Shutdown());

        // Edit menu
        UndoCommand = new RelayCommand(_ => _undoRedoService.Undo(), _ => _undoRedoService.CanUndo);
        RedoCommand = new RelayCommand(_ => _undoRedoService.Redo(), _ => _undoRedoService.CanRedo);
        CutCommand = new RelayCommand(_ => StatusText = "Cut not implemented");
        CopyCommand = new RelayCommand(_ => StatusText = "Copy not implemented");
        PasteCommand = new RelayCommand(_ => StatusText = "Paste not implemented");
        DeleteCommand = new RelayCommand(_ => StatusText = "Delete not implemented");

        // Tools menu
        NewKnobCommand = new RelayCommand(_ => CreateNewKnob());
        NewLayoutCommand = new RelayCommand(_ => CreateNewLayout());
        RunSelfTestCommand = new RelayCommand(async _ => await RunSelfTestAsync());
        PreferencesCommand = new RelayCommand(_ => ShowPreferences());
        AboutCommand = new RelayCommand(_ => ShowAbout());
    }

    private void UpdateCommandStates()
    {
        CommandManager.InvalidateRequerySuggested();
    }

    #endregion

    #region File Operations

    private void NewProject()
    {
        if (!CheckSaveChanges())
            return;

        CurrentProject = new ProjectModel
        {
            Name = "New Project",
            Created = DateTime.UtcNow
        };
        CurrentFilePath = null;
        IsDirty = false;
        Knobs.Clear();
        Layouts.Clear();
        _undoRedoService.Clear();
        StatusText = "New project created";
    }

    private async Task OpenProjectAsync()
    {
        if (!CheckSaveChanges())
            return;

        var dialog = new OpenFileDialog
        {
            Filter = "BrokenArrow SkinLab Project (*.baslproj)|*.baslproj|All Files (*.*)|*.*",
            Title = "Open Project"
        };

        if (dialog.ShowDialog() == true)
        {
            try
            {
                var project = await ProjectIO.LoadProjectAsync(dialog.FileName);
                CurrentProject = project;
                CurrentFilePath = dialog.FileName;
                IsDirty = false;

                Knobs.Clear();
                foreach (var knob in project.Knobs)
                    Knobs.Add(knob);

                Layouts.Clear();
                foreach (var layout in project.Layouts)
                    Layouts.Add(layout);

                _undoRedoService.Clear();
                StatusText = $"Opened project: {Path.GetFileName(dialog.FileName)}";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error opening project: {ex.Message}", "Error", 
                    MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }

    private async Task SaveProjectAsync()
    {
        if (CurrentFilePath == null)
        {
            await SaveProjectAsAsync();
            return;
        }

        try
        {
            CurrentProject.Knobs = Knobs.ToList();
            CurrentProject.Layouts = Layouts.ToList();
            await ProjectIO.SaveProjectAsync(CurrentFilePath, CurrentProject);
            IsDirty = false;
            StatusText = $"Saved project: {Path.GetFileName(CurrentFilePath)}";
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Error saving project: {ex.Message}", "Error", 
                MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }

    private async Task SaveProjectAsAsync()
    {
        var dialog = new SaveFileDialog
        {
            Filter = "BrokenArrow SkinLab Project (*.baslproj)|*.baslproj|All Files (*.*)|*.*",
            Title = "Save Project As",
            FileName = CurrentProject.Name + ".baslproj"
        };

        if (dialog.ShowDialog() == true)
        {
            CurrentFilePath = dialog.FileName;
            await SaveProjectAsync();
        }
    }

    private bool CheckSaveChanges()
    {
        if (!IsDirty)
            return true;

        var result = MessageBox.Show(
            "Do you want to save changes to the current project?",
            "Save Changes",
            MessageBoxButton.YesNoCancel,
            MessageBoxImage.Question);

        if (result == MessageBoxResult.Cancel)
            return false;

        if (result == MessageBoxResult.Yes)
        {
            SaveProjectAsync().Wait();
        }

        return true;
    }

    #endregion

    #region Workspace Operations

    private void CreateNewKnob()
    {
        var knob = new KnobModel
        {
            Name = $"Knob {Knobs.Count + 1}",
            Layers = new List<LayerModel>
            {
                new LayerModel { Name = "Background", Type = LayerType.Background },
                new LayerModel { Name = "Pointer", Type = LayerType.Pointer }
            }
        };
        Knobs.Add(knob);
        IsDirty = true;
        StatusText = $"Created new knob: {knob.Name}";
    }

    private void CreateNewLayout()
    {
        var layout = new LayoutModel
        {
            Name = $"Layout {Layouts.Count + 1}"
        };
        Layouts.Add(layout);
        IsDirty = true;
        StatusText = $"Created new layout: {layout.Name}";
    }

    private async Task RunSelfTestAsync()
    {
        StatusText = "Running self-test...";
        
        var testService = new SelfTestService();
        var progress = new Progress<string>(msg => StatusText = msg);
        
        try
        {
            var result = await testService.RunSelfTestAsync(progress);
            
            var message = result.Success 
                ? $"Self-test completed successfully!\n\nPassed: {result.TestsPassed}/{result.TotalTests}\n\n{result.Log}"
                : $"Self-test failed!\n\nPassed: {result.TestsPassed}/{result.TotalTests}\n\n{result.Log}";

            MessageBox.Show(message, "Self-Test Results", 
                MessageBoxButton.OK, 
                result.Success ? MessageBoxImage.Information : MessageBoxImage.Warning);
            
            StatusText = result.Success ? "Self-test passed" : "Self-test failed";
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Self-test error: {ex.Message}", "Error", 
                MessageBoxButton.OK, MessageBoxImage.Error);
            StatusText = "Self-test error";
        }
    }

    private void ShowPreferences()
    {
        MessageBox.Show("Preferences dialog - to be implemented", "Preferences", 
            MessageBoxButton.OK, MessageBoxImage.Information);
    }

    private void ShowAbout()
    {
        var version = typeof(MainViewModel).Assembly.GetName().Version;
        MessageBox.Show(
            $"BrokenArrow SkinLab\nVersion {version}\n\n" +
            "A comprehensive WPF-based GUI skinning application\n" +
            "modeled after KnobMan and SkinMan.\n\n" +
            "© 2026 Broken Arrow Software",
            "About BrokenArrow SkinLab",
            MessageBoxButton.OK,
            MessageBoxImage.Information);
    }

    #endregion
}
