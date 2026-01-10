#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "UI/CustomLookAndFeel.h"
#include "UI/LevelMeter.h"

//==============================================================================
/**
    SuperBassman Audio Processor Editor
*/
class SuperBassmanAudioProcessorEditor : public juce::AudioProcessorEditor,
                                         private juce::Timer
{
public:
    SuperBassmanAudioProcessorEditor (SuperBassmanAudioProcessor&);
    ~SuperBassmanAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    // Reference to processor
    SuperBassmanAudioProcessor& audioProcessor;
    
    // Look and feel
    CustomLookAndFeel customLookAndFeel;
    
    // UI Components
    // Input section
    juce::Slider inputGainSlider;
    juce::Label inputGainLabel;
    juce::ToggleButton inputPadButton;
    juce::ToggleButton hpfEnableButton;
    juce::Slider hpfFreqSlider;
    
    // Channel selection
    juce::ComboBox channelSelector;
    juce::Label channelLabel;
    
    // Preamp
    juce::Slider preampGainSlider;
    juce::Label preampGainLabel;
    juce::Slider preampBiasSlider;
    juce::Label preampBiasLabel;
    
    // Tone stack
    juce::Slider bassSlider;
    juce::Label bassLabel;
    juce::Slider midSlider;
    juce::Label midLabel;
    juce::Slider trebleSlider;
    juce::Label trebleLabel;
    juce::Slider presenceSlider;
    juce::Label presenceLabel;
    
    // Overdrive
    juce::Slider driveSlider;
    juce::Label driveLabel;
    juce::Slider driveBlendSlider;
    juce::Label driveBlendLabel;
    juce::Slider driveToneSlider;
    juce::Label driveToneLabel;
    
    // Power amp
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    juce::Slider powerSagSlider;
    juce::Label powerSagLabel;
    
    // Cabinet
    juce::ToggleButton cabEnableButton;
    juce::ComboBox cabTypeSelector;
    juce::Slider cabMixSlider;
    juce::Label cabMixLabel;
    
    // Output
    juce::Slider outputGainSlider;
    juce::Label outputGainLabel;
    juce::ToggleButton limiterEnableButton;
    juce::ToggleButton bypassButton;
    
    // Meters
    LevelMeter inputMeter;
    LevelMeter outputMeter;
    
    // Attachments
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>> comboBoxAttachments;
    
    void setupSlider (juce::Slider& slider, juce::Label& label, const juce::String& labelText,
                      const juce::String& paramId);
    void setupToggleButton (juce::ToggleButton& button, const juce::String& buttonText,
                           const juce::String& paramId);
    void setupComboBox (juce::ComboBox& comboBox, const juce::String& paramId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperBassmanAudioProcessorEditor)
};
