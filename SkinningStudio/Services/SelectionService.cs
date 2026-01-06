using SkinningStudio.Models;
using System.Collections.ObjectModel;

namespace SkinningStudio.Services
{
    public class SelectionService
    {
        private readonly ObservableCollection<UIElementModel> _selectedElements = new();

        public ObservableCollection<UIElementModel> SelectedElements => _selectedElements;

        public event EventHandler? SelectionChanged;

        public void Select(UIElementModel element, bool multiSelect = false)
        {
            if (!multiSelect)
            {
                ClearSelection();
            }

            if (!_selectedElements.Contains(element))
            {
                element.IsSelected = true;
                _selectedElements.Add(element);
                OnSelectionChanged();
            }
        }

        public void Deselect(UIElementModel element)
        {
            if (_selectedElements.Contains(element))
            {
                element.IsSelected = false;
                _selectedElements.Remove(element);
                OnSelectionChanged();
            }
        }

        public void ClearSelection()
        {
            foreach (var element in _selectedElements)
            {
                element.IsSelected = false;
            }
            _selectedElements.Clear();
            OnSelectionChanged();
        }

        public void SelectAll(IEnumerable<UIElementModel> elements)
        {
            ClearSelection();
            foreach (var element in elements)
            {
                element.IsSelected = true;
                _selectedElements.Add(element);
            }
            OnSelectionChanged();
        }

        private void OnSelectionChanged() => SelectionChanged?.Invoke(this, EventArgs.Empty);
    }
}
