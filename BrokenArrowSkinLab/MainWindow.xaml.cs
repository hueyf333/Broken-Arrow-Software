using Microsoft.UI.Xaml;
using BrokenArrowSkinLab.ViewModels;

namespace BrokenArrowSkinLab;

public sealed partial class MainWindow : Window
{
    public MainViewModel ViewModel { get; }

    public MainWindow()
    {
        this.InitializeComponent();
        ViewModel = new MainViewModel();
        
        Title = "Broken Arrow Skin Lab";
        ExtendsContentIntoTitleBar = true;
        SetTitleBar(null);
    }

    private void OnExitClick(object sender, RoutedEventArgs e)
    {
        Application.Current.Exit();
    }
}
