#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Tube Preamp - multi-stage tube saturation modeling
 * - Asymmetric soft clipping to simulate tube characteristics
 * - Bias control for harmonic shaping
 * - Multiple gain stages
 */
class TubePreamp
{
public:
    TubePreamp() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        gainSmooth.reset (sampleRate, 0.05);
        gainSmooth.setCurrentAndTargetValue (5.0f);
        
        biasSmooth.reset (sampleRate, 0.05);
        biasSmooth.setCurrentAndTargetValue (0.0f);
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        float gain = apvts.getRawParameterValue ("preampGain")->load();
        float bias = apvts.getRawParameterValue ("preampBias")->load();
        
        gainSmooth.setTargetValue (gain);
        biasSmooth.setTargetValue (bias);
        
        for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getChannelPointer (channel);
            
            for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float input = channelData[sample];
                float currentGain = gainSmooth.getNextValue();
                float currentBias = biasSmooth.getNextValue();
                
                // Apply gain and bias
                float signal = input * currentGain * 0.5f + currentBias * 0.1f;
                
                // Multi-stage tube saturation
                // Stage 1: Gentle saturation
                signal = tubeSaturation (signal, 1.5f);
                
                // Stage 2: Moderate saturation
                signal = tubeSaturation (signal, 1.2f);
                
                // Stage 3: Final shaping
                signal = tubeSaturation (signal, 1.1f);
                
                // Compensate gain
                channelData[sample] = signal * 0.7f;
            }
        }
    }
    
private:
    // Asymmetric tube saturation function
    float tubeSaturation (float input, float asymmetry)
    {
        // Soft clipping with asymmetry
        float sign = input >= 0.0f ? 1.0f : -1.0f;
        float absInput = std::abs (input);
        
        // Different curves for positive and negative
        float threshold = 0.7f;
        if (absInput < threshold)
        {
            return input;
        }
        else
        {
            float excess = absInput - threshold;
            float compressed = threshold + std::tanh (excess * asymmetry) / asymmetry;
            return sign * compressed;
        }
    }
    
    double sampleRate = 44100.0;
    juce::SmoothedValue<float> gainSmooth;
    juce::SmoothedValue<float> biasSmooth;
};
