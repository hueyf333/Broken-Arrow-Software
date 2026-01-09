/*
  ==============================================================================

    Input Stage - Input conditioning with trim and noise gate

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class InputStage
{
public:
    InputStage();
    ~InputStage() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setInputTrim(float trimDb);
    void setGateEnabled(bool enabled);
    void setGateThreshold(float thresholdDb);

private:
    juce::SmoothedValue<float> inputGain{ 1.0f };
    juce::SmoothedValue<float> gateThreshold{ 0.01f };
    
    bool gateEnabled = false;
    float gateEnvelope = 0.0f;
    
    double sampleRate = 44100.0;
    
    static constexpr float gateAttackMs = 1.0f;
    static constexpr float gateReleaseMs = 100.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputStage)
};
