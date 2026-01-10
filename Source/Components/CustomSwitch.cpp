#include "CustomSwitch.h"

CustomSwitch::CustomSwitch()
{
}

void CustomSwitch::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    
    // Draw switch background
    g.setColour(state ? juce::Colour(0xff4a6a4a) : juce::Colour(0xff2a2a2a));
    g.fillRoundedRectangle(bounds, 5.0f);
    
    // Draw border
    g.setColour(juce::Colour(0xff5a5a5a));
    g.drawRoundedRectangle(bounds, 5.0f, 2.0f);
    
    // Draw toggle indicator
    float toggleX = state ? (bounds.getWidth() - 20.0f) : 5.0f;
    g.setColour(juce::Colours::white);
    g.fillEllipse(toggleX, 5.0f, 15.0f, bounds.getHeight() - 10.0f);
}

void CustomSwitch::mouseDown(const juce::MouseEvent& event)
{
    state = !state;
    
    if (onStateChange)
        onStateChange(state);
    
    repaint();
}

void CustomSwitch::setState(bool newState)
{
    state = newState;
    repaint();
}
