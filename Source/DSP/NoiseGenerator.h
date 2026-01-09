/*
  ==============================================================================

    Noise Generator - Subtle hiss and hum simulation

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class NoiseGenerator
{
public:
    NoiseGenerator();
    ~NoiseGenerator() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setLevel(float level); // 0-1 range

private:
    juce::SmoothedValue<float> level{ 0.1f };
    juce::Random random;
    
    double sampleRate = 44100.0;
    float humPhase = 0.0f;
    
    // Pink noise filter state
    float pinkState[7] = { 0.0f };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGenerator)
};
