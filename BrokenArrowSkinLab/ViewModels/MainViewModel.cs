using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using BrokenArrowSkinLab.Models;
using BrokenArrowSkinLab.Services;

namespace BrokenArrowSkinLab.ViewModels;

public partial class MainViewModel : ObservableObject
{
    private readonly ProjectSerializationService _serializationService;
    private readonly ExportService _exportService;

    [ObservableProperty]
    private SkinProject _currentProject;

    [ObservableProperty]
    private Layer? _selectedLayer;

    [ObservableProperty]
    private int _currentFrame;

    [ObservableProperty]
    private double _zoomLevel = 1.0;

    [ObservableProperty]
    private bool _isDirty;

    public ObservableCollection<Layer> CurrentFrameLayers { get; } = new();

    public MainViewModel()
    {
        _serializationService = new ProjectSerializationService();
        _exportService = new ExportService();
        _currentProject = new SkinProject();
        
        UpdateCurrentFrameLayers();
    }

    [RelayCommand]
    private void NewProject()
    {
        CurrentProject = new SkinProject();
        CurrentFrame = 0;
        UpdateCurrentFrameLayers();
        IsDirty = false;
    }

    [RelayCommand]
    private async void OpenProject()
    {
        // File picker logic would go here in the View
        // For now, this is a placeholder
    }

    [RelayCommand]
    private async void SaveProject()
    {
        if (string.IsNullOrEmpty(CurrentProject.FilePath))
        {
            await SaveProjectAs();
        }
        else
        {
            await _serializationService.SaveProjectAsync(CurrentProject, CurrentProject.FilePath);
            IsDirty = false;
        }
    }

    [RelayCommand]
    private async System.Threading.Tasks.Task SaveProjectAs()
    {
        // File picker logic would go here in the View
    }

    [RelayCommand]
    private void AddLayer()
    {
        var newLayer = new Layer
        {
            Name = $"Layer {CurrentFrameLayers.Count + 1}",
            Position = new System.Numerics.Vector2(CurrentProject.CanvasWidth / 2, CurrentProject.CanvasHeight / 2)
        };
        
        CurrentFrameLayers.Add(newLayer);
        CurrentProject.Frames[CurrentFrame].Add(newLayer);
        SelectedLayer = newLayer;
        IsDirty = true;
    }

    [RelayCommand]
    private void RemoveLayer()
    {
        if (SelectedLayer != null && CurrentFrameLayers.Contains(SelectedLayer))
        {
            var index = CurrentFrameLayers.IndexOf(SelectedLayer);
            CurrentFrameLayers.Remove(SelectedLayer);
            CurrentProject.Frames[CurrentFrame].Remove(SelectedLayer);
            
            if (CurrentFrameLayers.Count > 0)
            {
                SelectedLayer = CurrentFrameLayers[Math.Min(index, CurrentFrameLayers.Count - 1)];
            }
            else
            {
                SelectedLayer = null;
            }
            
            IsDirty = true;
        }
    }

    [RelayCommand]
    private void DuplicateLayer()
    {
        if (SelectedLayer != null)
        {
            var clone = SelectedLayer.Clone();
            var index = CurrentFrameLayers.IndexOf(SelectedLayer);
            CurrentFrameLayers.Insert(index + 1, clone);
            CurrentProject.Frames[CurrentFrame].Insert(index + 1, clone);
            SelectedLayer = clone;
            IsDirty = true;
        }
    }

    [RelayCommand]
    private void MoveLayerUp()
    {
        if (SelectedLayer != null)
        {
            var index = CurrentFrameLayers.IndexOf(SelectedLayer);
            if (index > 0)
            {
                CurrentFrameLayers.Move(index, index - 1);
                var layer = CurrentProject.Frames[CurrentFrame][index];
                CurrentProject.Frames[CurrentFrame].RemoveAt(index);
                CurrentProject.Frames[CurrentFrame].Insert(index - 1, layer);
                IsDirty = true;
            }
        }
    }

    [RelayCommand]
    private void MoveLayerDown()
    {
        if (SelectedLayer != null)
        {
            var index = CurrentFrameLayers.IndexOf(SelectedLayer);
            if (index < CurrentFrameLayers.Count - 1)
            {
                CurrentFrameLayers.Move(index, index + 1);
                var layer = CurrentProject.Frames[CurrentFrame][index];
                CurrentProject.Frames[CurrentFrame].RemoveAt(index);
                CurrentProject.Frames[CurrentFrame].Insert(index + 1, layer);
                IsDirty = true;
            }
        }
    }

    [RelayCommand]
    private void AddFrame()
    {
        // Copy current frame or create new empty frame
        var newFrame = CurrentProject.Frames[CurrentFrame]
            .Select(layer => layer.Clone())
            .ToList();
        
        CurrentProject.Frames.Add(newFrame);
        CurrentProject.FrameCount = CurrentProject.Frames.Count;
        CurrentFrame = CurrentProject.Frames.Count - 1;
        IsDirty = true;
    }

    [RelayCommand]
    private void RemoveFrame()
    {
        if (CurrentProject.Frames.Count > 1 && CurrentFrame >= 0 && CurrentFrame < CurrentProject.Frames.Count)
        {
            CurrentProject.Frames.RemoveAt(CurrentFrame);
            CurrentProject.FrameCount = CurrentProject.Frames.Count;
            CurrentFrame = Math.Min(CurrentFrame, CurrentProject.Frames.Count - 1);
            UpdateCurrentFrameLayers();
            IsDirty = true;
        }
    }

    [RelayCommand]
    private void ZoomIn()
    {
        ZoomLevel = Math.Min(ZoomLevel * 1.2, 10.0);
    }

    [RelayCommand]
    private void ZoomOut()
    {
        ZoomLevel = Math.Max(ZoomLevel / 1.2, 0.1);
    }

    [RelayCommand]
    private void ResetZoom()
    {
        ZoomLevel = 1.0;
    }

    partial void OnCurrentFrameChanged(int value)
    {
        UpdateCurrentFrameLayers();
    }

    private void UpdateCurrentFrameLayers()
    {
        CurrentFrameLayers.Clear();
        if (CurrentFrame >= 0 && CurrentFrame < CurrentProject.Frames.Count)
        {
            foreach (var layer in CurrentProject.Frames[CurrentFrame])
            {
                CurrentFrameLayers.Add(layer);
            }
        }
    }

    [RelayCommand]
    private async void ExportSingleFrame()
    {
        // File picker logic would go here
    }

    [RelayCommand]
    private async void ExportSpriteSheet()
    {
        // File picker logic would go here
    }
}
