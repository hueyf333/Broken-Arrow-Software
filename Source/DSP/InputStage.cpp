/*
  ==============================================================================

    Input Stage - Input conditioning with trim and noise gate

  ==============================================================================
*/

#include "InputStage.h"

InputStage::InputStage()
{
}

void InputStage::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    inputGain.reset(sampleRate, 0.02); // 20ms smoothing
    gateThreshold.reset(sampleRate, 0.02);
    
    reset();
}

void InputStage::process(juce::dsp::AudioBlock<float>& block)
{
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            
            // Apply input trim
            float gain = inputGain.getNextValue();
            inputSample *= gain;
            
            // Apply noise gate if enabled
            if (gateEnabled)
            {
                float threshold = gateThreshold.getNextValue();
                float absLevel = std::abs(inputSample);
                
                // Simple envelope follower
                float attackCoeff = std::exp(-1.0f / (gateAttackMs * 0.001f * static_cast<float>(sampleRate)));
                float releaseCoeff = std::exp(-1.0f / (gateReleaseMs * 0.001f * static_cast<float>(sampleRate)));
                
                if (absLevel > gateEnvelope)
                    gateEnvelope = attackCoeff * gateEnvelope + (1.0f - attackCoeff) * absLevel;
                else
                    gateEnvelope = releaseCoeff * gateEnvelope + (1.0f - releaseCoeff) * absLevel;
                
                // Gate attenuation
                float gateGain = (gateEnvelope > threshold) ? 1.0f : 0.0f;
                inputSample *= gateGain;
            }
            
            channelData[sample] = inputSample;
        }
    }
}

void InputStage::reset()
{
    inputGain.setCurrentAndTargetValue(1.0f);
    gateThreshold.setCurrentAndTargetValue(0.01f);
    gateEnvelope = 0.0f;
}

void InputStage::setInputTrim(float trimDb)
{
    inputGain.setTargetValue(juce::Decibels::decibelsToGain(trimDb));
}

void InputStage::setGateEnabled(bool enabled)
{
    gateEnabled = enabled;
}

void InputStage::setGateThreshold(float thresholdDb)
{
    gateThreshold.setTargetValue(juce::Decibels::decibelsToGain(thresholdDb));
}
