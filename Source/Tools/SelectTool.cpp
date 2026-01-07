#include "SelectTool.h"

namespace KnobSmith
{

SelectTool::SelectTool() : Tool(Type::Select)
{
}

void SelectTool::mouseDown(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(targetImage);
    dragStart = e.getPosition();
    isDragging = true;
    selection = juce::Rectangle<int>(dragStart, dragStart);
}

void SelectTool::mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(targetImage);
    if (isDragging)
    {
        selection = juce::Rectangle<int>(dragStart, e.getPosition());
    }
}

void SelectTool::mouseUp(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(e, targetImage);
    isDragging = false;
}

juce::Cursor SelectTool::getCursor() const
{
    return juce::MouseCursor::NormalCursor;
}

} // namespace KnobSmith
