using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;
using SkinningStudio.Models;

namespace SkinningStudio.Views.Controls
{
    public class SelectableElement : ContentControl
    {
        private Thumb? _resizeThumb;
        private const double HandleSize = 8;

        public static readonly DependencyProperty ElementModelProperty =
            DependencyProperty.Register("ElementModel", typeof(UIElementModel), typeof(SelectableElement),
                new PropertyMetadata(null, OnElementModelChanged));

        public UIElementModel? ElementModel
        {
            get => (UIElementModel?)GetValue(ElementModelProperty);
            set => SetValue(ElementModelProperty, value);
        }

        static SelectableElement()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(SelectableElement),
                new FrameworkPropertyMetadata(typeof(SelectableElement)));
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if (GetTemplateChild("PART_ResizeThumb") is Thumb resizeThumb)
            {
                _resizeThumb = resizeThumb;
                _resizeThumb.DragDelta += ResizeThumb_DragDelta;
            }
        }

        private void ResizeThumb_DragDelta(object sender, DragDeltaEventArgs e)
        {
            if (ElementModel != null && !ElementModel.IsLocked)
            {
                var newWidth = Math.Max(20, ElementModel.Width + e.HorizontalChange);
                var newHeight = Math.Max(20, ElementModel.Height + e.VerticalChange);

                ElementModel.Width = newWidth;
                ElementModel.Height = newHeight;
            }
        }

        private static void OnElementModelChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is SelectableElement element && e.NewValue is UIElementModel model)
            {
                element.Width = model.Width;
                element.Height = model.Height;
            }
        }
    }
}
