using BrokenArrowSkinLab.App.Commands;
using BrokenArrowSkinLab.App.Infrastructure;
using BrokenArrowSkinLab.App.Services;
using BrokenArrowSkinLab.Core.Models;
using BrokenArrowSkinLab.Core.IO;
using BrokenArrowSkinLab.Core.Services;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Input;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Windows.Storage.Pickers;
using Windows.Storage;

namespace BrokenArrowSkinLab.App.ViewModels;

public class MainViewModel : ViewModelBase
{
    private readonly UndoRedoService _undoRedoService = new();
    private readonly KnobDesignerViewModel _knobDesignerViewModel = new();
    private readonly LayoutDesignerViewModel _layoutDesignerViewModel = new();
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

    public KnobDesignerViewModel KnobDesigner => _knobDesignerViewModel;
    public LayoutDesignerViewModel LayoutDesigner => _layoutDesignerViewModel;

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
        ExitCommand = new RelayCommand(_ => Application.Current.Exit());

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
        if (UndoCommand is RelayCommand undoCmd)
            undoCmd.RaiseCanExecuteChanged();
        if (RedoCommand is RelayCommand redoCmd)
            redoCmd.RaiseCanExecuteChanged();
        if (SaveProjectCommand is RelayCommand saveCmd)
            saveCmd.RaiseCanExecuteChanged();
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

        var picker = new FileOpenPicker();
        picker.FileTypeFilter.Add(".baslproj");
        picker.FileTypeFilter.Add("*");
        
        // Get window handle for picker
        var window = (Application.Current as App)?.Window;
        if (window != null)
        {
            var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(window);
            WinRT.Interop.InitializeWithWindow.Initialize(picker, hwnd);
        }

        var file = await picker.PickSingleFileAsync();
        if (file != null)
        {
            try
            {
                var project = await ProjectIO.LoadProjectAsync(file.Path);
                CurrentProject = project;
                CurrentFilePath = file.Path;
                IsDirty = false;

                Knobs.Clear();
                foreach (var knob in project.Knobs)
                    Knobs.Add(knob);

                Layouts.Clear();
                foreach (var layout in project.Layouts)
                    Layouts.Add(layout);

                _undoRedoService.Clear();
                StatusText = $"Opened project: {file.Name}";
            }
            catch (Exception ex)
            {
                await ShowErrorAsync($"Error opening project: {ex.Message}");
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
            await ShowErrorAsync($"Error saving project: {ex.Message}");
        }
    }

    private async Task SaveProjectAsAsync()
    {
        var picker = new FileSavePicker();
        picker.FileTypeChoices.Add("BrokenArrow SkinLab Project", new List<string> { ".baslproj" });
        picker.SuggestedFileName = CurrentProject.Name + ".baslproj";
        
        // Get window handle for picker
        var window = (Application.Current as App)?.Window;
        if (window != null)
        {
            var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(window);
            WinRT.Interop.InitializeWithWindow.Initialize(picker, hwnd);
        }

        var file = await picker.PickSaveFileAsync();
        if (file != null)
        {
            CurrentFilePath = file.Path;
            await SaveProjectAsync();
        }
    }

    private bool CheckSaveChanges()
    {
        if (!IsDirty)
            return true;

        // Note: In WinUI, dialogs must be async, so this is a simplified version
        // A full implementation would need to restructure calling code
        StatusText = "Unsaved changes - please save manually";
        return true; // Simplified for now
    }

    private async Task ShowErrorAsync(string message)
    {
        var dialog = new ContentDialog
        {
            Title = "Error",
            Content = message,
            CloseButtonText = "OK",
            XamlRoot = (Application.Current as App)?.Window?.Content?.XamlRoot
        };
        await dialog.ShowAsync();
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
        _knobDesignerViewModel.CurrentKnob = knob;
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
        _layoutDesignerViewModel.CurrentLayout = layout;
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

            var dialog = new ContentDialog
            {
                Title = "Self-Test Results",
                Content = new ScrollViewer
                {
                    Content = new TextBlock { Text = message, TextWrapping = TextWrapping.Wrap }
                },
                CloseButtonText = "OK",
                XamlRoot = (Application.Current as App)?.Window?.Content?.XamlRoot
            };
            await dialog.ShowAsync();
            
            StatusText = result.Success ? "Self-test passed" : "Self-test failed";
        }
        catch (Exception ex)
        {
            await ShowErrorAsync($"Self-test error: {ex.Message}");
            StatusText = "Self-test error";
        }
    }

    private void ShowPreferences()
    {
        StatusText = "Preferences dialog - to be implemented";
    }

    private void ShowAbout()
    {
        var version = typeof(MainViewModel).Assembly.GetName().Version;
        var message = $"BrokenArrow SkinLab\nVersion {version}\n\n" +
            "A comprehensive WinUI-based GUI skinning application\n" +
            "modeled after KnobMan and SkinMan.\n\n" +
            "© 2026 Broken Arrow Software";
        
        var dialog = new ContentDialog
        {
            Title = "About BrokenArrow SkinLab",
            Content = message,
            CloseButtonText = "OK",
            XamlRoot = (Application.Current as App)?.Window?.Content?.XamlRoot
        };
        _ = dialog.ShowAsync();
    }

    #endregion
}
