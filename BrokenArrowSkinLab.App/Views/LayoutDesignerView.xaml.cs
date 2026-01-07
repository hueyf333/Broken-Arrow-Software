using System.Globalization;
using System.Windows.Data;

namespace BrokenArrowSkinLab.App.Views;

public partial class LayoutDesignerView : System.Windows.Controls.UserControl
{
    public LayoutDesignerView()
    {
        InitializeComponent();
    }
}

public class BoolToOpacityConverter : IValueConverter
{
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
        return value is bool boolValue && boolValue ? 1.0 : 0.0;
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
        throw new NotImplementedException();
    }
}
