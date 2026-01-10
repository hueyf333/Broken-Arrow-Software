#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class AmpegSVTAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     private juce::Timer
{
public:
    AmpegSVTAudioProcessorEditor(AmpegSVTAudioProcessor&);
    ~AmpegSVTAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    void timerCallback() override;
    
    AmpegSVTAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpegSVTAudioProcessorEditor)
};
