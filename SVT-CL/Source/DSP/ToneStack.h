#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>

class ToneStack
{
public:
    enum MidFrequency
    {
        Hz220 = 0,
        Hz450,
        Hz800,
        Hz1600,
        Hz3000
    };
    
    ToneStack();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);
    
    void setBass(float value);
    void setMid(float value);
    void setTreble(float value);
    void setMidFrequency(MidFrequency freq);
    
    void setUltraLo(float value);
    void setUltraHi(float value);
    
private:
    void updateFilters();
    
    juce::dsp::ProcessorChain<
        juce::dsp::IIR::Filter<float>,  // Bass shelf
        juce::dsp::IIR::Filter<float>,  // Mid peak
        juce::dsp::IIR::Filter<float>,  // Treble shelf
        juce::dsp::IIR::Filter<float>,  // Ultra Lo
        juce::dsp::IIR::Filter<float>   // Ultra Hi
    > filterChain;
    
    juce::SmoothedValue<float> bassSmoothed{0.5f};
    juce::SmoothedValue<float> midSmoothed{0.5f};
    juce::SmoothedValue<float> trebleSmoothed{0.5f};
    juce::SmoothedValue<float> ultraLoSmoothed{0.0f};
    juce::SmoothedValue<float> ultraHiSmoothed{0.0f};
    
    MidFrequency midFreq{Hz800};
    double sampleRate{44100.0};
};
