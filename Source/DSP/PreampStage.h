/*
  ==============================================================================

    Preamp Stage - Cascaded non-linear tube-like stages
    Models JCM900 preamp: tight crunch, aggressive upper mids, fizzy edge

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PreampStage
{
public:
    PreampStage();
    ~PreampStage() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setGain(float gain); // 0-10 range
    void setMode(int mode); // 0=Clean, 1=Crunch, 2=High Gain

private:
    float applyTubeStage(float input, float bias, float drive);
    
    juce::SmoothedValue<float> gain{ 5.0f };
    int currentMode = 1;
    
    double sampleRate = 44100.0;
    
    // High-pass filter to remove DC offset
    juce::dsp::IIR::Filter<float> dcBlocker[2];
    
    // Pre-emphasis filter for upper-mid character
    juce::dsp::IIR::Filter<float> preEmphasis[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreampStage)
};
