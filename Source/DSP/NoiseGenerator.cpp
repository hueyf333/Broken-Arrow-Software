/*
  ==============================================================================

    Noise Generator - Subtle hiss and hum simulation

  ==============================================================================
*/

#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator()
{
}

void NoiseGenerator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    level.reset(sampleRate, 0.02);
    reset();
}

void NoiseGenerator::process(juce::dsp::AudioBlock<float>& block)
{
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float currentLevel = level.getNextValue();
            
            // Generate pink noise (more natural than white noise)
            float white = random.nextFloat() * 2.0f - 1.0f;
            
            // Paul Kellett's pink noise algorithm
            pinkState[0] = 0.99886f * pinkState[0] + white * 0.0555179f;
            pinkState[1] = 0.99332f * pinkState[1] + white * 0.0750759f;
            pinkState[2] = 0.96900f * pinkState[2] + white * 0.1538520f;
            pinkState[3] = 0.86650f * pinkState[3] + white * 0.3104856f;
            pinkState[4] = 0.55000f * pinkState[4] + white * 0.5329522f;
            pinkState[5] = -0.7616f * pinkState[5] - white * 0.0168980f;
            
            float pink = pinkState[0] + pinkState[1] + pinkState[2] + 
                        pinkState[3] + pinkState[4] + pinkState[5] + 
                        pinkState[6] + white * 0.5362f;
            pinkState[6] = white * 0.115926f;
            
            // Add 50/60 Hz hum
            float hum = std::sin(humPhase) * 0.3f; // 50 Hz
            hum += std::sin(humPhase * 1.2f) * 0.15f; // 60 Hz
            
            humPhase += juce::MathConstants<float>::twoPi * 50.0f / static_cast<float>(sampleRate);
            if (humPhase > juce::MathConstants<float>::twoPi)
                humPhase -= juce::MathConstants<float>::twoPi;
            
            // Mix noise and hum, scale by level
            float noise = (pink * 0.02f + hum * 0.01f) * currentLevel * 0.05f;
            
            // Add to existing signal
            channelData[sample] += noise;
        }
    }
}

void NoiseGenerator::reset()
{
    level.setCurrentAndTargetValue(0.1f);
    humPhase = 0.0f;
    
    for (int i = 0; i < 7; ++i)
        pinkState[i] = 0.0f;
}

void NoiseGenerator::setLevel(float newLevel)
{
    level.setTargetValue(newLevel);
}
