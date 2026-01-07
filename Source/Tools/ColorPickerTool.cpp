#include "ColorPickerTool.h"

namespace KnobSmith
{

ColorPickerTool::ColorPickerTool() : Tool(Type::ColorPicker)
{
}

void ColorPickerTool::mouseDown(const juce::MouseEvent& e, juce::Image& targetImage)
{
    if (targetImage.isValid())
    {
        auto position = e.getPosition();
        if (targetImage.getBounds().contains(position))
        {
            pickedColour = targetImage.getPixelAt(position.x, position.y);
            
            if (onColourPicked)
                onColourPicked(pickedColour);
        }
    }
}

void ColorPickerTool::mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage)
{
    mouseDown(e, targetImage);
}

void ColorPickerTool::mouseUp(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(e, targetImage);
}

juce::Cursor ColorPickerTool::getCursor() const
{
    return juce::MouseCursor::CrosshairCursor;
}

} // namespace KnobSmith
