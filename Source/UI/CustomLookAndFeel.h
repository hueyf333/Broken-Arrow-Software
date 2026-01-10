#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * Custom Look and Feel for Super Bassman
 * Vintage-inspired bass amp aesthetics
 */
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Set color scheme
        setColour (juce::Slider::thumbColourId, juce::Colours::orange);
        setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
        setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff3a3a3a));
        setColour (juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colour (0xff1a1a1a));
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff3a3a3a));
        
        setColour (juce::Label::textColourId, juce::Colours::lightgrey);
        
        setColour (juce::ToggleButton::textColourId, juce::Colours::lightgrey);
        setColour (juce::ToggleButton::tickColourId, juce::Colours::orange);
        setColour (juce::ToggleButton::tickDisabledColourId, juce::Colour (0xff3a3a3a));
        
        setColour (juce::ComboBox::backgroundColourId, juce::Colour (0xff1a1a1a));
        setColour (juce::ComboBox::textColourId, juce::Colours::white);
        setColour (juce::ComboBox::outlineColourId, juce::Colour (0xff3a3a3a));
        setColour (juce::ComboBox::arrowColourId, juce::Colours::orange);
    }
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10.0f);
        auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;
        
        // Background arc
        g.setColour (slider.findColour (juce::Slider::rotarySliderOutlineColourId));
        juce::Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    rotaryEndAngle,
                                    true);
        g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        // Value arc
        if (slider.isEnabled())
        {
            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                   bounds.getCentreY(),
                                   arcRadius,
                                   arcRadius,
                                   0.0f,
                                   rotaryStartAngle,
                                   toAngle,
                                   true);
            
            g.setColour (slider.findColour (juce::Slider::rotarySliderFillColourId));
            g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
        
        // Pointer
        juce::Path pointer;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 3.0f;
        pointer.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        pointer.applyTransform (juce::AffineTransform::rotation (toAngle).translated (bounds.getCentreX(), bounds.getCentreY()));
        
        g.setColour (slider.findColour (juce::Slider::thumbColourId));
        g.fillPath (pointer);
        
        // Center circle
        g.setColour (juce::Colour (0xff1a1a1a));
        g.fillEllipse (bounds.getCentreX() - 8.0f, bounds.getCentreY() - 8.0f, 16.0f, 16.0f);
    }
};
