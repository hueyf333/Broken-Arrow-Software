#pragma once

#include <JuceHeader.h>

/**
 * Overdrive Channel
 * - High-gain distortion stage
 * - Drive control
 * - Blend/mix with clean signal
 * - Tone shaping
 */
class OverdriveChannel
{
public:
    OverdriveChannel() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        // Tone filter for overdrive
        toneFilter.prepare (spec);
        
        driveSmooth.reset (sampleRate, 0.05);
        blendSmooth.reset (sampleRate, 0.05);
        toneSmooth.reset (sampleRate, 0.05);
        
        driveSmooth.setCurrentAndTargetValue (0.0f);
        blendSmooth.setCurrentAndTargetValue (0.0f);
        toneSmooth.setCurrentAndTargetValue (5.0f);
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        float drive = apvts.getRawParameterValue ("drive")->load();
        float blend = apvts.getRawParameterValue ("driveBlend")->load() / 100.0f;
        float tone = apvts.getRawParameterValue ("driveTone")->load();
        
        driveSmooth.setTargetValue (drive);
        blendSmooth.setTargetValue (blend);
        toneSmooth.setTargetValue (tone);
        
        // Update tone filter
        updateToneFilter (tone);
        
        // Process overdrive
        for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getChannelPointer (channel);
            
            for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float input = channelData[sample];
                float currentDrive = driveSmooth.getNextValue();
                float currentBlend = blendSmooth.getNextValue();
                
                // Drive stage
                float driven = applyOverdrive (input, currentDrive);
                
                // Blend with clean signal
                channelData[sample] = input * (1.0f - currentBlend) + driven * currentBlend;
            }
        }
        
        // Apply tone shaping
        toneFilter.process (context);
    }
    
private:
    float applyOverdrive (float input, float drive)
    {
        // Map drive 0-10 to gain 1-50
        float driveGain = 1.0f + drive * 4.9f;
        float signal = input * driveGain;
        
        // Hard clipping with smoothing
        float threshold = 0.7f;
        if (std::abs (signal) < threshold)
        {
            return signal;
        }
        else
        {
            // Soft clip above threshold
            float sign = signal >= 0.0f ? 1.0f : -1.0f;
            float excess = std::abs (signal) - threshold;
            float compressed = threshold + (1.0f - threshold) * std::tanh (excess);
            return sign * compressed * 0.7f; // Compensate gain
        }
    }
    
    void updateToneFilter (float tone)
    {
        if (std::abs (tone - lastTone) > 0.01f)
        {
            // Tone control: low-pass filter, darker at lower values
            float cutoff = 500.0f + tone * 1500.0f; // 500 Hz - 15 kHz
            *toneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, cutoff, 0.707f);
            lastTone = tone;
        }
    }
    
    double sampleRate = 44100.0;
    float lastTone = 5.0f;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> toneFilter;
    
    juce::SmoothedValue<float> driveSmooth;
    juce::SmoothedValue<float> blendSmooth;
    juce::SmoothedValue<float> toneSmooth;
};
