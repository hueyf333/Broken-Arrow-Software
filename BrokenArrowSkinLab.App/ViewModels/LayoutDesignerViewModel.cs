using BrokenArrowSkinLab.App.Commands;
using BrokenArrowSkinLab.App.Infrastructure;
using BrokenArrowSkinLab.Core.Models;
using System.Collections.ObjectModel;
using Microsoft.UI.Xaml;
using System.Windows.Input;

namespace BrokenArrowSkinLab.App.ViewModels;

public class LayoutDesignerViewModel : ViewModelBase
{
    private LayoutModel? _currentLayout;
    private ControlModel? _selectedControl;
    private bool _showGrid = true;
    private double _zoomLevel = 1.0;
    private string _statusText = "Ready";

    public LayoutDesignerViewModel()
    {
        InitializeCommands();
        Controls = new ObservableCollection<ControlModel>();
    }

    #region Properties

    public LayoutModel? CurrentLayout
    {
        get => _currentLayout;
        set
        {
            if (SetProperty(ref _currentLayout, value))
            {
                Controls.Clear();
                if (value != null)
                {
                    foreach (var control in value.Controls)
                        Controls.Add(control);
                }
            }
        }
    }

    public ObservableCollection<ControlModel> Controls { get; }

    public ControlModel? SelectedControl
    {
        get => _selectedControl;
        set
        {
            if (SetProperty(ref _selectedControl, value))
            {
                CommandManager.InvalidateRequerySuggested();
            }
        }
    }

    public bool ShowGrid
    {
        get => _showGrid;
        set => SetProperty(ref _showGrid, value);
    }

    public double ZoomLevel
    {
        get => _zoomLevel;
        set => SetProperty(ref _zoomLevel, Math.Clamp(value, 0.25, 4.0));
    }

    public string StatusText
    {
        get => _statusText;
        set => SetProperty(ref _statusText, value);
    }

    #endregion

    #region Commands

    public ICommand AddKnobCommand { get; private set; } = null!;
    public ICommand AddSliderHCommand { get; private set; } = null!;
    public ICommand AddSliderVCommand { get; private set; } = null!;
    public ICommand AddButtonCommand { get; private set; } = null!;
    public ICommand AddSwitchCommand { get; private set; } = null!;
    public ICommand AddMeterCommand { get; private set; } = null!;
    public ICommand RemoveControlCommand { get; private set; } = null!;
    public ICommand DuplicateControlCommand { get; private set; } = null!;
    public ICommand ZoomInCommand { get; private set; } = null!;
    public ICommand ZoomOutCommand { get; private set; } = null!;
    public ICommand ZoomFitCommand { get; private set; } = null!;
    public ICommand ToggleGridCommand { get; private set; } = null!;

    private void InitializeCommands()
    {
        AddKnobCommand = new RelayCommand(_ => AddControl(ControlType.Knob));
        AddSliderHCommand = new RelayCommand(_ => AddControl(ControlType.SliderHorizontal));
        AddSliderVCommand = new RelayCommand(_ => AddControl(ControlType.SliderVertical));
        AddButtonCommand = new RelayCommand(_ => AddControl(ControlType.ButtonToggle));
        AddSwitchCommand = new RelayCommand(_ => AddControl(ControlType.SwitchMultiState));
        AddMeterCommand = new RelayCommand(_ => AddControl(ControlType.LEDMeter));
        RemoveControlCommand = new RelayCommand(_ => RemoveControl(), _ => SelectedControl != null);
        DuplicateControlCommand = new RelayCommand(_ => DuplicateControl(), _ => SelectedControl != null);
        ZoomInCommand = new RelayCommand(_ => ZoomLevel *= 1.25);
        ZoomOutCommand = new RelayCommand(_ => ZoomLevel /= 1.25);
        ZoomFitCommand = new RelayCommand(_ => ZoomLevel = 1.0);
        ToggleGridCommand = new RelayCommand(_ => ShowGrid = !ShowGrid);
    }

    #endregion

    #region Control Operations

    private void AddControl(ControlType type)
    {
        if (CurrentLayout == null) return;

        var control = new ControlModel
        {
            Name = $"{type} {Controls.Count + 1}",
            Type = type,
            Bounds = new RectModel
            {
                X = 50,
                Y = 50,
                Width = GetDefaultWidth(type),
                Height = GetDefaultHeight(type)
            }
        };

        Controls.Add(control);
        CurrentLayout.Controls.Add(control);
        SelectedControl = control;
        StatusText = $"Added {type}: {control.Name}";
    }

    private double GetDefaultWidth(ControlType type)
    {
        return type switch
        {
            ControlType.Knob => 64,
            ControlType.SliderHorizontal => 128,
            ControlType.SliderVertical => 32,
            ControlType.ButtonToggle => 64,
            ControlType.ButtonMomentary => 64,
            ControlType.SwitchMultiState => 48,
            ControlType.LEDMeter => 80,
            _ => 64
        };
    }

    private double GetDefaultHeight(ControlType type)
    {
        return type switch
        {
            ControlType.Knob => 64,
            ControlType.SliderHorizontal => 32,
            ControlType.SliderVertical => 128,
            ControlType.ButtonToggle => 32,
            ControlType.ButtonMomentary => 32,
            ControlType.SwitchMultiState => 48,
            ControlType.LEDMeter => 200,
            _ => 64
        };
    }

    private void RemoveControl()
    {
        if (SelectedControl == null || CurrentLayout == null) return;

        Controls.Remove(SelectedControl);
        CurrentLayout.Controls.Remove(SelectedControl);
        SelectedControl = null;
        StatusText = "Control removed";
    }

    private void DuplicateControl()
    {
        if (SelectedControl == null || CurrentLayout == null) return;

        var duplicate = new ControlModel
        {
            Name = SelectedControl.Name + " (Copy)",
            Type = SelectedControl.Type,
            Bounds = new RectModel
            {
                X = SelectedControl.Bounds.X + 20,
                Y = SelectedControl.Bounds.Y + 20,
                Width = SelectedControl.Bounds.Width,
                Height = SelectedControl.Bounds.Height
            },
            ImageSource = SelectedControl.ImageSource,
            HitTestShape = SelectedControl.HitTestShape
        };

        Controls.Add(duplicate);
        CurrentLayout.Controls.Add(duplicate);
        SelectedControl = duplicate;
        StatusText = "Control duplicated";
    }

    #endregion
}
