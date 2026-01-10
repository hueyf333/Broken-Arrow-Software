#include "InputStage.h"

InputStage::InputStage()
{
}

void InputStage::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    
    // Configure DC blocker (high-pass at 20 Hz)
    auto coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 20.0);
    dcBlocker.coefficients = coefficients;
    
    reset();
}

void InputStage::reset()
{
    dcBlocker.reset();
    envelopeFollower = 0.0f;
    gateLevel = 0.0f;
}

float InputStage::processSample(float input)
{
    // Apply pad attenuation
    float output = input * padGain;
    
    // DC blocking
    output = dcBlocker.processSample(output);
    
    // Gate
    if (gateEnabled)
    {
        float inputLevel = std::abs(output);
        float targetGate = (inputLevel > gateThreshold) ? 1.0f : 0.0f;
        
        float coeff = (targetGate > gateLevel) ? gateAttack : gateRelease;
        gateLevel += coeff * (targetGate - gateLevel);
        
        output *= gateLevel;
    }
    
    // Apply trim gain
    output *= trimGain;
    
    // Denormal protection
    if (std::abs(output) < 1e-15f)
        output = 0.0f;
    
    return output;
}

void InputStage::setPad(float padDb)
{
    padGain = juce::Decibels::decibelsToGain(padDb);
}

void InputStage::setTrim(float trimDb)
{
    trimGain = juce::Decibels::decibelsToGain(trimDb);
}

void InputStage::setGateThreshold(float thresholdDb)
{
    gateThreshold = juce::Decibels::decibelsToGain(thresholdDb);
}

void InputStage::setGateEnabled(bool enabled)
{
    gateEnabled = enabled;
}
