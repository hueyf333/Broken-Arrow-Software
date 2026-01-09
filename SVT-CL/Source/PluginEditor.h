#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class LevelMeter : public juce::Component, public juce::Timer
{
public:
    LevelMeter(SVTCLAudioProcessor& p, bool isInput)
        : processor(p), inputMeter(isInput)
    {
        startTimerHz(30);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        
        // Background
        g.setColour(juce::Colours::black);
        g.fillRect(bounds);
        
        // Get level
        float level = inputMeter ? processor.getSVTProcessor().getInputLevel() 
                                 : processor.getSVTProcessor().getOutputLevel();
        bool clipping = inputMeter ? processor.getSVTProcessor().getInputClipping() 
                                   : processor.getSVTProcessor().getOutputClipping();
        
        // Draw level bar
        float levelHeight = bounds.getHeight() * level;
        juce::Rectangle<float> levelRect(0, bounds.getHeight() - levelHeight, 
                                         bounds.getWidth(), levelHeight);
        
        // Color: green -> yellow -> red
        juce::Colour meterColor = juce::Colours::green;
        if (level > 0.7f)
            meterColor = juce::Colours::orange;
        if (level > 0.9f || clipping)
            meterColor = juce::Colours::red;
        
        g.setColour(meterColor);
        g.fillRect(levelRect);
        
        // Clip indicator
        if (clipping)
        {
            g.setColour(juce::Colours::red);
            g.fillRect(bounds.removeFromTop(5.0f));
        }
    }
    
    void timerCallback() override
    {
        repaint();
    }
    
private:
    SVTCLAudioProcessor& processor;
    bool inputMeter;
};

class SVTCLAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SVTCLAudioProcessorEditor(SVTCLAudioProcessor&);
    ~SVTCLAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SVTCLAudioProcessor& audioProcessor;
    
    // Sliders
    juce::Slider gainSlider, bassSlider, midSlider, trebleSlider;
    juce::Slider ultraLoSlider, ultraHiSlider, masterSlider;
    
    // Labels
    juce::Label gainLabel, bassLabel, midLabel, trebleLabel;
    juce::Label ultraLoLabel, ultraHiLabel, masterLabel, midFreqLabel;
    
    // Buttons
    juce::ToggleButton padButton{"PAD"};
    juce::ToggleButton cabSimButton{"CAB"};
    juce::ToggleButton standbyButton{"STANDBY"};
    
    // Mid frequency selector
    juce::ComboBox midFreqCombo;
    
    // Meters
    LevelMeter inputMeter;
    LevelMeter outputMeter;
    
    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ultraLoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ultraHiAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> padAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cabSimAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> standbyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midFreqAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVTCLAudioProcessorEditor)
};
