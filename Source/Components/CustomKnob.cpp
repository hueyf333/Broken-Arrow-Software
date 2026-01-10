#include "CustomKnob.h"

CustomKnob::CustomKnob()
{
}

void CustomKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    
    // Draw knob body
    g.setColour(juce::Colour(0xff3a3a3a));
    g.fillEllipse(bounds);
    
    // Draw border
    g.setColour(juce::Colour(0xff5a5a5a));
    g.drawEllipse(bounds, 2.0f);
    
    // Draw indicator
    auto centre = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f;
    auto angle = juce::MathConstants<float>::pi * 1.5f + value * juce::MathConstants<float>::pi * 1.5f;
    
    auto indicatorEnd = centre + juce::Point<float>(std::cos(angle), std::sin(angle)) * (radius * 0.7f);
    
    g.setColour(juce::Colours::white);
    g.drawLine(centre.x, centre.y, indicatorEnd.x, indicatorEnd.y, 3.0f);
}

void CustomKnob::resized()
{
}

void CustomKnob::mouseDown(const juce::MouseEvent& event)
{
    lastMouseY = event.y;
}

void CustomKnob::mouseDrag(const juce::MouseEvent& event)
{
    int deltaY = lastMouseY - event.y;
    lastMouseY = event.y;
    
    float sensitivity = 0.01f;
    value = juce::jlimit(0.0f, 1.0f, value + deltaY * sensitivity);
    
    if (onValueChange)
        onValueChange(value);
    
    repaint();
}

void CustomKnob::setValue(float newValue)
{
    value = juce::jlimit(0.0f, 1.0f, newValue);
    repaint();
}
