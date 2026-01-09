/*
  ==============================================================================

    Tone Stack - Marshall-style interactive tone control
    Bass, Mid, Treble, Presence (not independent EQs)

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ToneStack
{
public:
    ToneStack();
    ~ToneStack() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setBass(float value); // 0-10 range
    void setMid(float value);
    void setTreble(float value);
    void setPresence(float value);

private:
    void updateFilters();
    
    juce::SmoothedValue<float> bass{ 5.0f };
    juce::SmoothedValue<float> mid{ 5.0f };
    juce::SmoothedValue<float> treble{ 5.0f };
    juce::SmoothedValue<float> presence{ 5.0f };
    
    double sampleRate = 44100.0;
    
    // Marshall-style tone stack filters
    juce::dsp::IIR::Filter<float> bassFilter[2];
    juce::dsp::IIR::Filter<float> midFilter[2];
    juce::dsp::IIR::Filter<float> trebleFilter[2];
    juce::dsp::IIR::Filter<float> presenceFilter[2];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToneStack)
};
