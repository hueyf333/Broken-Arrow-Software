#pragma once

#include <juce_dsp/juce_dsp.h>

class PowerAmp
{
public:
    PowerAmp();
    ~PowerAmp() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    float processSample(float input);
    
    void setDrive(float drive);     // 0-1
    void setMaster(float master);   // 0-1

private:
    float applySaturation(float input);
    float applySag(float input);
    
    float drive { 0.5f };
    float master { 0.7f };
    float sampleRate { 44100.0f };
    
    // Sag simulation (power supply compression)
    float sagAmount { 0.0f };
    static constexpr float sagAttack = 0.001f;
    static constexpr float sagRelease = 0.05f;
    
    // Output stage filtering
    juce::dsp::IIR::Filter<float> outputFilter;
};
