#include "FillTool.h"
#include "../Utils/ImageUtils.h"

namespace KnobSmith
{

FillTool::FillTool() : Tool(Type::Fill)
{
}

void FillTool::mouseDown(const juce::MouseEvent& e, juce::Image& targetImage)
{
    if (targetImage.isValid())
    {
        auto position = e.getPosition();
        ImageUtils::floodFill(targetImage, position, fillColour);
    }
}

void FillTool::mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(e, targetImage);
}

void FillTool::mouseUp(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(e, targetImage);
}

juce::Cursor FillTool::getCursor() const
{
    return juce::MouseCursor::CrosshairCursor;
}

} // namespace KnobSmith
