#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LevelMeter : public juce::Component,
                   private juce::Timer
{
public:
    LevelMeter();
    ~LevelMeter() override = default;
    
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
    
    void setLevel(float newLevel);
    
private:
    std::atomic<float> level { 0.0f };
    float displayLevel { 0.0f };
};
