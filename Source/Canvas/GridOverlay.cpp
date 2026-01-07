#include "GridOverlay.h"

namespace KnobSmith
{

GridOverlay::GridOverlay()
{
}

juce::Point<int> GridOverlay::snapToGrid(juce::Point<int> point) const
{
    if (!snapEnabled)
        return point;

    int x = (point.x + gridSize / 2) / gridSize * gridSize;
    int y = (point.y + gridSize / 2) / gridSize * gridSize;
    return {x, y};
}

juce::Point<float> GridOverlay::snapToGrid(juce::Point<float> point) const
{
    if (!snapEnabled)
        return point;

    float x = std::round(point.x / gridSize) * gridSize;
    float y = std::round(point.y / gridSize) * gridSize;
    return {x, y};
}

void GridOverlay::paint(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    if (!gridVisible)
        return;

    g.setColour(gridColour);

    // Draw vertical lines
    for (int x = 0; x < bounds.getWidth(); x += gridSize)
    {
        g.drawVerticalLine(x, 0.0f, static_cast<float>(bounds.getHeight()));
    }

    // Draw horizontal lines
    for (int y = 0; y < bounds.getHeight(); y += gridSize)
    {
        g.drawHorizontalLine(y, 0.0f, static_cast<float>(bounds.getWidth()));
    }
}

} // namespace KnobSmith
