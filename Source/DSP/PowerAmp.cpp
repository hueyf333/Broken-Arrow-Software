/*
  ==============================================================================

    Power Amp - Saturation, dynamic compression, and sag control

  ==============================================================================
*/

#include "PowerAmp.h"

PowerAmp::PowerAmp()
{
}

void PowerAmp::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    master.reset(sampleRate, 0.02);
    reset();
}

void PowerAmp::process(juce::dsp::AudioBlock<float>& block)
{
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        size_t channelIndex = std::min(channel, size_t(1));
        
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            float currentMaster = master.getNextValue();
            
            // Apply master gain
            inputSample *= currentMaster * 0.3f; // Scale down to reasonable level
            
            // Envelope follower for dynamics
            float absLevel = std::abs(inputSample);
            float attackCoeff = std::exp(-1.0f / (5.0f * 0.001f * static_cast<float>(sampleRate)));
            float releaseCoeff = std::exp(-1.0f / (50.0f * 0.001f * static_cast<float>(sampleRate)));
            
            if (absLevel > envelope[channelIndex])
                envelope[channelIndex] = attackCoeff * envelope[channelIndex] + (1.0f - attackCoeff) * absLevel;
            else
                envelope[channelIndex] = releaseCoeff * envelope[channelIndex] + (1.0f - releaseCoeff) * absLevel;
            
            // Power supply sag simulation
            if (sagMode == 0) // Authentic
            {
                // Sag envelope (slow attack, slow release)
                float sagAttack = std::exp(-1.0f / (10.0f * 0.001f * static_cast<float>(sampleRate)));
                float sagRelease = std::exp(-1.0f / (200.0f * 0.001f * static_cast<float>(sampleRate)));
                
                if (absLevel > sagEnvelope)
                    sagEnvelope = sagAttack * sagEnvelope + (1.0f - sagAttack) * absLevel;
                else
                    sagEnvelope = sagRelease * sagEnvelope + (1.0f - sagRelease) * absLevel;
                
                // Apply sag compression (reduces gain when signal is loud)
                float sagGain = 1.0f / (1.0f + sagEnvelope * 0.3f);
                inputSample *= sagGain;
            }
            // Tight mode: no sag
            
            // Dynamic compression
            inputSample = applyDynamicCompression(inputSample, envelope[channelIndex]);
            
            // Power amp saturation
            inputSample = applySaturation(inputSample);
            
            channelData[sample] = inputSample;
        }
    }
}

void PowerAmp::reset()
{
    master.setCurrentAndTargetValue(5.0f);
    envelope[0] = 0.0f;
    envelope[1] = 0.0f;
    sagEnvelope = 0.0f;
}

void PowerAmp::setMaster(float value)
{
    master.setTargetValue(value);
}

void PowerAmp::setSagMode(int mode)
{
    sagMode = juce::jlimit(0, 1, mode);
}

float PowerAmp::applySaturation(float input)
{
    // Power amp saturation using soft clipping
    // Slightly different character than preamp
    
    // Crossover distortion simulation (slight)
    float x = input;
    if (std::abs(x) < 0.05f)
        x *= 0.9f; // Slight deadband
    
    // Soft clipping with asymmetry
    float output = x;
    
    if (x > 0.0f)
        output = x / (1.0f + 0.7f * x);
    else
        output = x / (1.0f + 0.5f * std::abs(x));
    
    return output * 1.2f;
}

float PowerAmp::applyDynamicCompression(float input, float env)
{
    // Simple dynamic compression
    // Higher envelope = more compression
    
    if (env < 0.1f)
        return input;
    
    float threshold = 0.3f;
    float ratio = 3.0f;
    
    if (env > threshold)
    {
        float excess = env - threshold;
        float compression = excess / ratio;
        float targetGain = (threshold + compression) / env;
        return input * targetGain;
    }
    
    return input;
}
