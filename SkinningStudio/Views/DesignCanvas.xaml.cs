using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using SkinningStudio.Models;
using SkinningStudio.ViewModels;

namespace SkinningStudio.Views
{
    public partial class DesignCanvas : UserControl
    {
        private Point? _dragStartPoint;
        private UIElementModel? _draggedElement;

        public DesignCanvas()
        {
            InitializeComponent();
            MainCanvas.MouseLeftButtonDown += MainCanvas_MouseLeftButtonDown;
            MainCanvas.MouseLeftButtonUp += MainCanvas_MouseLeftButtonUp;
            MainCanvas.MouseMove += MainCanvas_MouseMove;
        }

        private void MainCanvas_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (DataContext is MainViewModel viewModel)
            {
                var position = e.GetPosition(MainCanvas);
                
                var hitElement = FindElementAtPosition(viewModel.Project.RootElements, position);
                
                if (hitElement != null && !hitElement.IsLocked)
                {
                    bool isMultiSelect = Keyboard.Modifiers.HasFlag(ModifierKeys.Control) || 
                                        Keyboard.Modifiers.HasFlag(ModifierKeys.Shift);
                    
                    viewModel.SelectionService.Select(hitElement, isMultiSelect);
                    
                    _dragStartPoint = position;
                    _draggedElement = hitElement;
                    MainCanvas.CaptureMouse();
                }
                else
                {
                    viewModel.SelectionService.ClearSelection();
                }
            }
        }

        private void MainCanvas_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            _dragStartPoint = null;
            _draggedElement = null;
            MainCanvas.ReleaseMouseCapture();
        }

        private void MainCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            if (_dragStartPoint.HasValue && _draggedElement != null && e.LeftButton == MouseButtonState.Pressed)
            {
                var currentPosition = e.GetPosition(MainCanvas);
                var offset = currentPosition - _dragStartPoint.Value;

                if (DataContext is MainViewModel viewModel && viewModel.SnapToGrid)
                {
                    offset.X = Math.Round(offset.X / 20) * 20;
                    offset.Y = Math.Round(offset.Y / 20) * 20;
                }

                if (offset.X != 0 || offset.Y != 0)
                {
                    _draggedElement.X += offset.X;
                    _draggedElement.Y += offset.Y;
                    _dragStartPoint = currentPosition;

                    if (DataContext is MainViewModel vm)
                    {
                        vm.Project.IsDirty = true;
                    }
                }
            }
        }

        private UIElementModel? FindElementAtPosition(IEnumerable<UIElementModel> elements, Point position)
        {
            foreach (var element in elements.Reverse())
            {
                if (element.IsVisible && 
                    position.X >= element.X && position.X <= element.X + element.Width &&
                    position.Y >= element.Y && position.Y <= element.Y + element.Height)
                {
                    return element;
                }
            }
            return null;
        }
    }
}
