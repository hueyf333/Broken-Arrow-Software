#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class CustomSwitch : public juce::Component
{
public:
    CustomSwitch();
    ~CustomSwitch() override = default;
    
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    
    void setState(bool newState);
    bool getState() const { return state; }
    
    std::function<void(bool)> onStateChange;
    
private:
    bool state { false };
};
