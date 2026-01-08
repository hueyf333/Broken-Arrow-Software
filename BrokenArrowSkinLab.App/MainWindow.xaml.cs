using Microsoft.UI.Xaml;

namespace BrokenArrowSkinLab.App;

/// <summary>
/// An empty window that can be used on its own or navigated to within a Frame.
/// </summary>
public sealed partial class MainWindow : Window
{
    public MainWindow()
    {
        this.InitializeComponent();
        this.Title = "BrokenArrow SkinLab";
        this.ExtendsContentIntoTitleBar = true;
        this.SetTitleBar(null);
    }
}
