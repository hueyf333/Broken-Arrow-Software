#pragma once

#include <juce_dsp/juce_dsp.h>

class TubePreamp
{
public:
    TubePreamp();
    ~TubePreamp() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    float processSample(float input);
    
    void setDrive(float drive);  // 0-1
    void setBias(float bias);    // -1 to 1

private:
    float applyAsymmetricClipping(float input);
    float applyBiasing(float input);
    
    float drive { 0.5f };
    float bias { 0.0f };
    float sampleRate { 44100.0f };
    
    // Multi-stage filters for tube emulation
    juce::dsp::IIR::Filter<float> stage1LowPass;
    juce::dsp::IIR::Filter<float> stage2LowPass;
    juce::dsp::IIR::Filter<float> stage3LowPass;
    
    // Denormal protection
    static constexpr float denormalThreshold = 1e-15f;
};
