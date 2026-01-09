/*
  ==============================================================================

    Broken Arrow M900 - Marshall JCM900-Style Amp Plugin
    Copyright (c) 2026 Broken Arrow Software

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * Custom rotary knob component styled for JCM900 amp
 */
class AmpKnob : public juce::Component
{
public:
    AmpKnob(const juce::String& labelText);
    ~AmpKnob() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider& getSlider() { return slider; }
    void setLabel(const juce::String& text) { label = text; repaint(); }

private:
    juce::Slider slider;
    juce::String label;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpKnob)
};

//==============================================================================
/**
 * Plugin editor with JCM900-styled interface
 */
class BrokenArrowM900AudioProcessorEditor : public juce::AudioProcessorEditor,
                                             private juce::Timer
{
public:
    BrokenArrowM900AudioProcessorEditor(BrokenArrowM900AudioProcessor&);
    ~BrokenArrowM900AudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    BrokenArrowM900AudioProcessor& audioProcessor;

    // Knobs
    AmpKnob inputTrimKnob{ "INPUT" };
    AmpKnob preampGainKnob{ "GAIN" };
    AmpKnob masterKnob{ "MASTER" };
    AmpKnob bassKnob{ "BASS" };
    AmpKnob midKnob{ "MID" };
    AmpKnob trebleKnob{ "TREBLE" };
    AmpKnob presenceKnob{ "PRESENCE" };
    AmpKnob outputKnob{ "OUTPUT" };

    // Toggles/Switches
    juce::ToggleButton gateToggle{ "GATE" };
    juce::ToggleButton cabToggle{ "CAB" };
    juce::ToggleButton noiseToggle{ "NOISE" };
    
    juce::ComboBox modeSelector;
    juce::Label modeLabel;
    
    juce::ComboBox oversamplingSelector;
    juce::Label oversamplingLabel;
    
    juce::ComboBox irSelector;
    juce::Label irLabel;

    // Meters
    float inputMeterLevel = 0.0f;
    float outputMeterLevel = 0.0f;
    bool clipIndicator = false;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputTrimAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preampGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cabAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> noiseAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oversamplingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> irAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrokenArrowM900AudioProcessorEditor)
};
