#include "EraserTool.h"
#include "../Utils/ImageUtils.h"

namespace KnobSmith
{

EraserTool::EraserTool() : Tool(Type::Eraser)
{
}

void EraserTool::mouseDown(const juce::MouseEvent& e, juce::Image& targetImage)
{
    lastPoint = e.position;
    
    if (targetImage.isValid())
    {
        juce::Graphics g(targetImage);
        g.setColour(juce::Colours::transparentBlack);
        g.fillEllipse(lastPoint.x - eraserSize / 2, lastPoint.y - eraserSize / 2, eraserSize, eraserSize);
    }
}

void EraserTool::mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage)
{
    auto currentPoint = e.position;
    
    if (targetImage.isValid())
    {
        ImageUtils::drawBrushStroke(targetImage, lastPoint, currentPoint, 
                                   juce::Colours::transparentBlack, eraserSize, 1.0f, 1.0f);
    }
    
    lastPoint = currentPoint;
}

void EraserTool::mouseUp(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(e, targetImage);
}

juce::Cursor EraserTool::getCursor() const
{
    return juce::MouseCursor::CrosshairCursor;
}

} // namespace KnobSmith
