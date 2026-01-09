#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_core/juce_core.h>

class CabSimulator
{
public:
    CabSimulator();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);
    
    void setEnabled(bool enabled);
    
private:
    void createImpulseResponse();
    
    juce::dsp::Convolution convolution;
    bool isEnabled{false};
    double sampleRate{44100.0};
};
