#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Power Amp Stage
 * - Power tube saturation and compression
 * - Power supply sag simulation
 * - Master volume control
 */
class PowerAmp
{
public:
    PowerAmp() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        masterVolumeSmooth.reset (sampleRate, 0.05);
        sagSmooth.reset (sampleRate, 0.1);
        compressionSmooth.reset (sampleRate, 0.05);
        
        masterVolumeSmooth.setCurrentAndTargetValue (5.0f);
        sagSmooth.setCurrentAndTargetValue (3.0f);
        compressionSmooth.setCurrentAndTargetValue (3.0f);
        
        // Envelope follower for sag
        envelopeState = 0.0f;
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        float masterVolume = apvts.getRawParameterValue ("masterVolume")->load();
        float sag = apvts.getRawParameterValue ("powerSag")->load();
        float compression = apvts.getRawParameterValue ("powerCompression")->load();
        
        masterVolumeSmooth.setTargetValue (masterVolume);
        sagSmooth.setTargetValue (sag);
        compressionSmooth.setTargetValue (compression);
        
        for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getChannelPointer (channel);
            
            for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float input = channelData[sample];
                float currentVolume = masterVolumeSmooth.getNextValue();
                float currentSag = sagSmooth.getNextValue();
                float currentCompression = compressionSmooth.getNextValue();
                
                // Envelope follower for power supply sag
                float inputAbs = std::abs (input);
                float attackCoeff = 0.01f;
                float releaseCoeff = 0.001f;
                
                if (inputAbs > envelopeState)
                    envelopeState += attackCoeff * (inputAbs - envelopeState);
                else
                    envelopeState += releaseCoeff * (inputAbs - envelopeState);
                
                // Power supply sag reduces gain during high input
                float sagAmount = currentSag / 10.0f;
                float sagReduction = 1.0f - (envelopeState * sagAmount * 0.3f);
                sagReduction = juce::jlimit (0.7f, 1.0f, sagReduction);
                
                // Apply master volume and sag
                float signal = input * (currentVolume / 10.0f) * sagReduction;
                
                // Power amp compression
                float compressionAmount = currentCompression / 10.0f;
                signal = applyPowerCompression (signal, compressionAmount);
                
                // Power tube saturation
                signal = powerTubeSaturation (signal);
                
                channelData[sample] = signal;
            }
        }
    }
    
private:
    float applyPowerCompression (float input, float amount)
    {
        // Soft knee compression
        float threshold = 0.5f;
        float ratio = 1.0f + amount * 3.0f; // 1:1 to 4:1
        
        float absInput = std::abs (input);
        if (absInput < threshold)
            return input;
        
        float sign = input >= 0.0f ? 1.0f : -1.0f;
        float excess = absInput - threshold;
        float compressed = threshold + excess / ratio;
        
        return sign * compressed;
    }
    
    float powerTubeSaturation (float input)
    {
        // Power tube saturation with softer knee
        return std::tanh (input * 1.5f) * 0.8f;
    }
    
    double sampleRate = 44100.0;
    float envelopeState = 0.0f;
    
    juce::SmoothedValue<float> masterVolumeSmooth;
    juce::SmoothedValue<float> sagSmooth;
    juce::SmoothedValue<float> compressionSmooth;
};
