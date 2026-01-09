#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>

class PowerAmp
{
public:
    PowerAmp();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);
    
    void setMaster(float level);
    
private:
    float powerAmpSaturation(float input, float drive);
    
    juce::dsp::Oversampling<float> oversampler{2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR};
    juce::dsp::Limiter<float> outputLimiter;
    juce::dsp::Compressor<float> compressor;
    
    juce::SmoothedValue<float> masterSmoothed{0.0f};
    
    double sampleRate{44100.0};
};
