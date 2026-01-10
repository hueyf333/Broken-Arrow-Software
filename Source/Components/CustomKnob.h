#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class CustomKnob : public juce::Component
{
public:
    CustomKnob();
    ~CustomKnob() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    void setValue(float newValue);
    float getValue() const { return value; }
    
    std::function<void(float)> onValueChange;
    
private:
    float value { 0.5f };
    int lastMouseY { 0 };
};
