#include "BrushTool.h"
#include "../Utils/ImageUtils.h"

namespace KnobSmith
{

BrushTool::BrushTool() : Tool(Type::Brush)
{
}

void BrushTool::mouseDown(const juce::MouseEvent& e, juce::Image& targetImage)
{
    lastPoint = e.position;
    
    if (targetImage.isValid())
    {
        juce::Graphics g(targetImage);
        g.setColour(colour.withAlpha(opacity));
        g.fillEllipse(lastPoint.x - brushSize / 2, lastPoint.y - brushSize / 2, brushSize, brushSize);
    }
}

void BrushTool::mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage)
{
    auto currentPoint = e.position;
    
    if (targetImage.isValid())
    {
        ImageUtils::drawBrushStroke(targetImage, lastPoint, currentPoint, colour, brushSize, brushHardness, opacity);
    }
    
    lastPoint = currentPoint;
}

void BrushTool::mouseUp(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(e, targetImage);
}

juce::Cursor BrushTool::getCursor() const
{
    return juce::MouseCursor::CrosshairCursor;
}

} // namespace KnobSmith
