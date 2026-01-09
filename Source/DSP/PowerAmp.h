/*
  ==============================================================================

    Power Amp - Saturation, dynamic compression, and sag control

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PowerAmp
{
public:
    PowerAmp();
    ~PowerAmp() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setMaster(float value); // 0-10 range
    void setSagMode(int mode); // 0=Authentic, 1=Tight

private:
    float applySaturation(float input);
    float applyDynamicCompression(float input, float envelope);
    
    juce::SmoothedValue<float> master{ 5.0f };
    int sagMode = 0;
    
    double sampleRate = 44100.0;
    
    // Envelope follower for dynamic compression
    float envelope[2] = { 0.0f, 0.0f };
    
    // Sag simulation (power supply compression)
    float sagEnvelope = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerAmp)
};
