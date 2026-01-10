#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_formats/juce_audio_formats.h>

class CabSimulator
{
public:
    CabSimulator();
    ~CabSimulator() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    float processSample(float input, int channel);
    
    void setMix(float mixAmount);  // 0 = dry, 1 = wet
    void loadImpulseResponse(const juce::File& irFile);

private:
    void createDefaultIR();
    
    float mix { 1.0f };
    float sampleRate { 44100.0f };
    
    // Convolution for IR
    juce::dsp::Convolution convolution;
    bool useConvolution { false };
    
    // Backup resonant filter network if no IR
    juce::dsp::IIR::Filter<float> resonance1[2];
    juce::dsp::IIR::Filter<float> resonance2[2];
    juce::dsp::IIR::Filter<float> lowPass[2];
    juce::dsp::IIR::Filter<float> highPass[2];
};
