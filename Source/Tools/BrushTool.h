#pragma once

#include "Tool.h"

namespace KnobSmith
{

class BrushTool : public Tool
{
public:
    BrushTool();
    ~BrushTool() override = default;

    void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) override;

    juce::Cursor getCursor() const override;

    void setBrushSize(float size) { brushSize = juce::jmax(1.0f, size); }
    float getBrushSize() const { return brushSize; }

    void setBrushHardness(float hardness) { brushHardness = juce::jlimit(0.0f, 1.0f, hardness); }
    float getBrushHardness() const { return brushHardness; }

    void setOpacity(float opacity) { this->opacity = juce::jlimit(0.0f, 1.0f, opacity); }
    float getOpacity() const { return opacity; }

    void setColour(juce::Colour colour) { this->colour = colour; }
    juce::Colour getColour() const { return colour; }

private:
    float brushSize{10.0f};
    float brushHardness{0.8f};
    float opacity{1.0f};
    juce::Colour colour{juce::Colours::black};
    juce::Point<float> lastPoint;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrushTool)
};

} // namespace KnobSmith
