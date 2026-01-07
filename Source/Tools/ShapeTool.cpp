#include "ShapeTool.h"

namespace KnobSmith
{

ShapeTool::ShapeTool() : Tool(Type::Shape)
{
}

void ShapeTool::mouseDown(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(targetImage);
    startPoint = e.getPosition();
    endPoint = startPoint;
    isDrawing = true;
}

void ShapeTool::mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage)
{
    juce::ignoreUnused(targetImage);
    endPoint = e.getPosition();
}

void ShapeTool::mouseUp(const juce::MouseEvent& e, juce::Image& targetImage)
{
    endPoint = e.getPosition();
    isDrawing = false;

    if (!targetImage.isValid())
        return;

    juce::Graphics g(targetImage);
    g.setColour(colour);

    auto bounds = juce::Rectangle<int>(startPoint, endPoint);

    switch (shapeType)
    {
        case ShapeType::Line:
        {
            g.drawLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y, strokeWidth);
            break;
        }
        case ShapeType::Rectangle:
        {
            if (filled)
                g.fillRect(bounds);
            else
                g.drawRect(bounds, static_cast<int>(strokeWidth));
            break;
        }
        case ShapeType::Ellipse:
        {
            if (filled)
                g.fillEllipse(bounds.toFloat());
            else
                g.drawEllipse(bounds.toFloat(), strokeWidth);
            break;
        }
    }
}

juce::Cursor ShapeTool::getCursor() const
{
    return juce::MouseCursor::CrosshairCursor;
}

} // namespace KnobSmith
