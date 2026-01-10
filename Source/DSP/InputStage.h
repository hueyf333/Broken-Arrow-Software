#pragma once

#include <juce_dsp/juce_dsp.h>

class InputStage
{
public:
    InputStage();
    ~InputStage() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    float processSample(float input);
    
    void setPad(float padDb);
    void setTrim(float trimDb);
    void setGateThreshold(float thresholdDb);
    void setGateEnabled(bool enabled);

private:
    // DC blocking filter
    juce::dsp::IIR::Filter<float> dcBlocker;
    
    // Gate parameters
    float gateThreshold { -60.0f };
    float gateLevel { 0.0f };
    bool gateEnabled { false };
    static constexpr float gateAttack = 0.001f;  // 1ms
    static constexpr float gateRelease = 0.1f;   // 100ms
    
    // Gain parameters
    float padGain { 1.0f };
    float trimGain { 1.0f };
    
    float sampleRate { 44100.0f };
    
    // Gate envelope detector
    float envelopeFollower { 0.0f };
};
