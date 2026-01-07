#pragma once

#include "Tool.h"

namespace KnobSmith
{

class FillTool : public Tool
{
public:
    FillTool();
    ~FillTool() override = default;

    void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) override;

    juce::Cursor getCursor() const override;

    void setFillColour(juce::Colour colour) { fillColour = colour; }
    juce::Colour getFillColour() const { return fillColour; }

private:
    juce::Colour fillColour{juce::Colours::white};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FillTool)
};

} // namespace KnobSmith
