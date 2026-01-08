using BrokenArrowSkinLab.App.Commands;
using BrokenArrowSkinLab.App.Infrastructure;
using BrokenArrowSkinLab.Core.Models;
using BrokenArrowSkinLab.Core.Rendering;
using BrokenArrowSkinLab.Core.Services;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Input;
using Microsoft.UI.Xaml.Media.Imaging;
using Windows.Storage.Pickers;

namespace BrokenArrowSkinLab.App.ViewModels;

public class KnobDesignerViewModel : ViewModelBase
{
    private readonly KnobRenderer _renderer = new();
    private readonly ExportService _exportService = new();
    private KnobModel? _currentKnob;
    private LayerModel? _selectedLayer;
    private int _previewFrameIndex;
    private WriteableBitmap? _previewImage;
    private bool _isRendering;
    private string _statusText = "Ready";

    public KnobDesignerViewModel()
    {
        InitializeCommands();
        Layers = new ObservableCollection<LayerModel>();
    }

    #region Properties

    public KnobModel? CurrentKnob
    {
        get => _currentKnob;
        set
        {
            if (SetProperty(ref _currentKnob, value))
            {
                Layers.Clear();
                if (value != null)
                {
                    foreach (var layer in value.Layers)
                        Layers.Add(layer);
                }
                UpdatePreview();
            }
        }
    }

    public ObservableCollection<LayerModel> Layers { get; }

    public LayerModel? SelectedLayer
    {
        get => _selectedLayer;
        set
        {
            if (SetProperty(ref _selectedLayer, value))
            {
                CommandManager.InvalidateRequerySuggested();
            }
        }
    }

    public int PreviewFrameIndex
    {
        get => _previewFrameIndex;
        set
        {
            if (SetProperty(ref _previewFrameIndex, value))
            {
                UpdatePreview();
            }
        }
    }

    public WriteableBitmap? PreviewImage
    {
        get => _previewImage;
        set => SetProperty(ref _previewImage, value);
    }

    public bool IsRendering
    {
        get => _isRendering;
        set => SetProperty(ref _isRendering, value);
    }

    public string StatusText
    {
        get => _statusText;
        set => SetProperty(ref _statusText, value);
    }

    #endregion

    #region Commands

    public ICommand AddLayerCommand { get; private set; } = null!;
    public ICommand RemoveLayerCommand { get; private set; } = null!;
    public ICommand MoveLayerUpCommand { get; private set; } = null!;
    public ICommand MoveLayerDownCommand { get; private set; } = null!;
    public ICommand ExportFilmstripCommand { get; private set; } = null!;
    public ICommand ExportSequenceCommand { get; private set; } = null!;
    public ICommand ExportAtlasCommand { get; private set; } = null!;

    private void InitializeCommands()
    {
        AddLayerCommand = new RelayCommand(_ => AddLayer());
        RemoveLayerCommand = new RelayCommand(_ => RemoveLayer(), _ => SelectedLayer != null);
        MoveLayerUpCommand = new RelayCommand(_ => MoveLayerUp(), _ => CanMoveLayerUp());
        MoveLayerDownCommand = new RelayCommand(_ => MoveLayerDown(), _ => CanMoveLayerDown());
        ExportFilmstripCommand = new RelayCommand(async _ => await ExportFilmstripAsync(), _ => CurrentKnob != null);
        ExportSequenceCommand = new RelayCommand(async _ => await ExportSequenceAsync(), _ => CurrentKnob != null);
        ExportAtlasCommand = new RelayCommand(async _ => await ExportAtlasAsync(), _ => CurrentKnob != null);
    }

    #endregion

    #region Layer Operations

    private void AddLayer()
    {
        if (CurrentKnob == null) return;

        var layer = new LayerModel
        {
            Name = $"Layer {Layers.Count + 1}",
            Type = LayerType.Background
        };
        
        Layers.Add(layer);
        CurrentKnob.Layers.Add(layer);
        SelectedLayer = layer;
        UpdatePreview();
        StatusText = $"Added layer: {layer.Name}";
    }

    private void RemoveLayer()
    {
        if (SelectedLayer == null || CurrentKnob == null) return;

        var index = Layers.IndexOf(SelectedLayer);
        Layers.Remove(SelectedLayer);
        CurrentKnob.Layers.Remove(SelectedLayer);
        
        if (Layers.Count > 0)
        {
            SelectedLayer = Layers[Math.Min(index, Layers.Count - 1)];
        }
        
        UpdatePreview();
        StatusText = "Layer removed";
    }

    private bool CanMoveLayerUp()
    {
        return SelectedLayer != null && Layers.IndexOf(SelectedLayer) > 0;
    }

    private void MoveLayerUp()
    {
        if (!CanMoveLayerUp()) return;

        var index = Layers.IndexOf(SelectedLayer!);
        Layers.Move(index, index - 1);
        
        if (CurrentKnob != null)
        {
            CurrentKnob.Layers = Layers.ToList();
        }
        
        UpdatePreview();
    }

    private bool CanMoveLayerDown()
    {
        return SelectedLayer != null && Layers.IndexOf(SelectedLayer) < Layers.Count - 1;
    }

    private void MoveLayerDown()
    {
        if (!CanMoveLayerDown()) return;

        var index = Layers.IndexOf(SelectedLayer!);
        Layers.Move(index, index + 1);
        
        if (CurrentKnob != null)
        {
            CurrentKnob.Layers = Layers.ToList();
        }
        
        UpdatePreview();
    }

    #endregion

    #region Preview

    private void UpdatePreview()
    {
        if (CurrentKnob == null) return;

        try
        {
            var frameIndex = Math.Min(PreviewFrameIndex, CurrentKnob.FrameCount - 1);
            PreviewImage = _renderer.RenderFrame(CurrentKnob, frameIndex);
        }
        catch (Exception ex)
        {
            StatusText = $"Preview error: {ex.Message}";
        }
    }

    #endregion

    #region Export

    private async Task ExportFilmstripAsync()
    {
        if (CurrentKnob == null) return;

        var dialog = new Microsoft.Win32.SaveFileDialog
        {
            Filter = "PNG Image (*.png)|*.png",
            FileName = $"{CurrentKnob.Name}_filmstrip.png"
        };

        if (dialog.ShowDialog() != true) return;

        try
        {
            IsRendering = true;
            StatusText = "Rendering frames...";

            var progress = new Progress<int>(percent => StatusText = $"Rendering: {percent}%");
            var frames = await _renderer.RenderKnobFramesAsync(CurrentKnob, progress);

            StatusText = "Exporting filmstrip...";
            await _exportService.ExportFilmstripAsync(frames, dialog.FileName, FilmstripOrientation.Vertical);

            StatusText = $"Exported filmstrip: {Path.GetFileName(dialog.FileName)}";
        }
        catch (Exception ex)
        {
            StatusText = $"Export error: {ex.Message}";
        }
        finally
        {
            IsRendering = false;
        }
    }

    private async Task ExportSequenceAsync()
    {
        if (CurrentKnob == null) return;

        var dialog = new Microsoft.Win32.SaveFileDialog
        {
            Filter = "Folder|*.folder",
            FileName = "Select Folder"
        };

        var folderBrowser = new System.Windows.Forms.FolderBrowserDialog
        {
            Description = "Select output folder for frame sequence"
        };

        if (folderBrowser.ShowDialog() != System.Windows.Forms.DialogResult.OK) return;

        try
        {
            IsRendering = true;
            StatusText = "Rendering frames...";

            var progress = new Progress<int>(percent => StatusText = $"Rendering: {percent}%");
            var frames = await _renderer.RenderKnobFramesAsync(CurrentKnob, progress);

            StatusText = "Exporting sequence...";
            await _exportService.ExportSequenceAsync(frames, folderBrowser.SelectedPath, CurrentKnob.Name);

            StatusText = $"Exported {frames.Count} frames to {folderBrowser.SelectedPath}";
        }
        catch (Exception ex)
        {
            StatusText = $"Export error: {ex.Message}";
        }
        finally
        {
            IsRendering = false;
        }
    }

    private async Task ExportAtlasAsync()
    {
        if (CurrentKnob == null) return;

        var dialog = new Microsoft.Win32.SaveFileDialog
        {
            Filter = "PNG Image (*.png)|*.png",
            FileName = $"{CurrentKnob.Name}_atlas.png"
        };

        if (dialog.ShowDialog() != true) return;

        try
        {
            IsRendering = true;
            StatusText = "Rendering frames...";

            var progress = new Progress<int>(percent => StatusText = $"Rendering: {percent}%");
            var frames = await _renderer.RenderKnobFramesAsync(CurrentKnob, progress);

            StatusText = "Exporting atlas...";
            await _exportService.ExportAtlasAsync(frames, dialog.FileName);

            StatusText = $"Exported atlas: {Path.GetFileName(dialog.FileName)}";
        }
        catch (Exception ex)
        {
            StatusText = $"Export error: {ex.Message}";
        }
        finally
        {
            IsRendering = false;
        }
    }

    #endregion
}
