/*
  ==============================================================================

    Output Stage - Output gain and soft-clip safety limiter

  ==============================================================================
*/

#include "OutputStage.h"

OutputStage::OutputStage()
{
}

void OutputStage::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    outputGain.reset(sampleRate, 0.02);
    reset();
}

void OutputStage::process(juce::dsp::AudioBlock<float>& block)
{
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            
            // Apply output gain
            float gain = outputGain.getNextValue();
            inputSample *= gain;
            
            // Apply soft-clip safety limiter
            inputSample = applySoftClipper(inputSample);
            
            channelData[sample] = inputSample;
        }
    }
}

void OutputStage::reset()
{
    outputGain.setCurrentAndTargetValue(1.0f);
}

void OutputStage::setOutputGain(float gainDb)
{
    outputGain.setTargetValue(juce::Decibels::decibelsToGain(gainDb));
}

float OutputStage::applySoftClipper(float input)
{
    // Gentle soft clipping to prevent digital clipping
    // Kicks in around 0.9, hard limit at 1.0
    
    float absInput = std::abs(input);
    
    if (absInput < 0.9f)
        return input;
    
    float sign = (input > 0.0f) ? 1.0f : -1.0f;
    
    if (absInput < 1.0f)
    {
        // Soft knee
        float excess = absInput - 0.9f;
        float compressed = 0.9f + excess * (1.0f - excess / 0.2f) * 0.5f;
        return sign * compressed;
    }
    
    // Hard limit
    return sign * 0.99f;
}
