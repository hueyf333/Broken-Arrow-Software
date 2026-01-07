#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class GridOverlay
{
public:
    GridOverlay();
    ~GridOverlay() = default;

    void setGridSize(int size) { gridSize = juce::jmax(1, size); }
    int getGridSize() const { return gridSize; }

    void setSnapEnabled(bool enabled) { snapEnabled = enabled; }
    bool isSnapEnabled() const { return snapEnabled; }

    void setGridVisible(bool visible) { gridVisible = visible; }
    bool isGridVisible() const { return gridVisible; }

    juce::Point<int> snapToGrid(juce::Point<int> point) const;
    juce::Point<float> snapToGrid(juce::Point<float> point) const;

    void paint(juce::Graphics& g, juce::Rectangle<int> bounds);

private:
    int gridSize{16};
    bool snapEnabled{false};
    bool gridVisible{true};
    juce::Colour gridColour{juce::Colours::white.withAlpha(0.2f)};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridOverlay)
};

} // namespace KnobSmith
