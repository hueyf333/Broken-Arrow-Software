#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Output Stage
 * - Output gain control
 * - Soft limiter to prevent clipping
 * - Final safety processing
 */
class OutputStage
{
public:
    OutputStage() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        outputGainSmooth.reset (sampleRate, 0.05);
        outputGainSmooth.setCurrentAndTargetValue (1.0f);
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        float outputGainDb = apvts.getRawParameterValue ("outputGain")->load();
        bool limiterEnabled = apvts.getRawParameterValue ("limiterEnable")->load();
        
        float outputGainLinear = juce::Decibels::decibelsToGain (outputGainDb);
        outputGainSmooth.setTargetValue (outputGainLinear);
        
        for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getChannelPointer (channel);
            
            for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float signal = channelData[sample];
                
                // Apply output gain
                signal *= outputGainSmooth.getNextValue();
                
                // Apply limiter if enabled
                if (limiterEnabled)
                {
                    signal = softLimiter (signal);
                }
                
                // Final safety clipping
                signal = juce::jlimit (-1.0f, 1.0f, signal);
                
                channelData[sample] = signal;
            }
        }
    }
    
private:
    float softLimiter (float input)
    {
        // Soft limiting with very gentle knee
        float threshold = 0.9f;
        float absInput = std::abs (input);
        
        if (absInput < threshold)
            return input;
        
        float sign = input >= 0.0f ? 1.0f : -1.0f;
        float excess = absInput - threshold;
        
        // Very soft compression above threshold
        float limited = threshold + excess * 0.2f;
        
        // Hard limit at 1.0
        limited = std::min (limited, 0.99f);
        
        return sign * limited;
    }
    
    double sampleRate = 44100.0;
    juce::SmoothedValue<float> outputGainSmooth;
};
