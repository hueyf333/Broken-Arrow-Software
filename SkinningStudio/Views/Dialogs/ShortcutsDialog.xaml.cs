using System.Windows;

namespace SkinningStudio.Views.Dialogs
{
    public partial class ShortcutsDialog : Window
    {
        public ShortcutsDialog()
        {
            InitializeComponent();
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
