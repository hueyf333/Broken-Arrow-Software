#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>
#include <array>

class TubePreamp
{
public:
    TubePreamp();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);
    
    void setGain(float gainDb);
    void setPad(bool usePad);
    
private:
    float processStage(float input, float drive);
    float asymmetricWaveShaper(float x, float drive);
    
    juce::dsp::Oversampling<float> oversampler{2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR};
    
    juce::SmoothedValue<float> gainSmoothed{0.0f};
    float padAttenuation{1.0f};
    
    double sampleRate{44100.0};
    
    static constexpr int numStages = 4;
    std::array<juce::dsp::IIR::Filter<float>, numStages> dcBlockers;
};
