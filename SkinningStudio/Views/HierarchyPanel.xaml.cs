using System.Windows.Controls;
using SkinningStudio.Models;
using SkinningStudio.ViewModels;

namespace SkinningStudio.Views
{
    public partial class HierarchyPanel : UserControl
    {
        public HierarchyPanel()
        {
            InitializeComponent();
            ElementTree.SelectedItemChanged += ElementTree_SelectedItemChanged;
        }

        private void ElementTree_SelectedItemChanged(object sender, System.Windows.RoutedPropertyChangedEventArgs<object> e)
        {
            if (DataContext is MainViewModel viewModel && e.NewValue is UIElementModel element)
            {
                viewModel.SelectionService.Select(element, false);
            }
        }
    }
}
