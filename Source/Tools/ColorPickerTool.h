#pragma once

#include "Tool.h"

namespace KnobSmith
{

class ColorPickerTool : public Tool
{
public:
    ColorPickerTool();
    ~ColorPickerTool() override = default;

    void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) override;

    juce::Cursor getCursor() const override;

    juce::Colour getPickedColour() const { return pickedColour; }
    
    std::function<void(juce::Colour)> onColourPicked;

private:
    juce::Colour pickedColour{juce::Colours::black};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorPickerTool)
};

} // namespace KnobSmith
