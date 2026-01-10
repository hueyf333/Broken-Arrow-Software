#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Input Stage - handles input conditioning
 * - Input gain/trim
 * - Pad switch (-10dB)
 * - DC blocking filter
 * - Optional high-pass filter
 */
class InputStage
{
public:
    InputStage() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        // DC blocker - high-pass at ~5 Hz
        *dcBlocker.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 5.0);
        dcBlocker.prepare (spec);
        
        // High-pass filter
        highPassFilter.prepare (spec);
        updateHighPassFilter (40.0f);
        
        // Smoothed values
        inputGainSmooth.reset (sampleRate, 0.05);
        inputGainSmooth.setCurrentAndTargetValue (1.0f);
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        // Get parameters
        float inputGainDb = apvts.getRawParameterValue ("inputGain")->load();
        bool padEnabled = apvts.getRawParameterValue ("inputPad")->load();
        bool dcBlockEnabled = apvts.getRawParameterValue ("dcBlock")->load();
        bool hpfEnabled = apvts.getRawParameterValue ("hpfEnable")->load();
        float hpfFreq = apvts.getRawParameterValue ("hpfFreq")->load();
        
        // Calculate input gain
        float gainLinear = juce::Decibels::decibelsToGain (inputGainDb);
        if (padEnabled)
            gainLinear *= 0.316f; // -10dB pad
        
        inputGainSmooth.setTargetValue (gainLinear);
        
        // Apply input gain
        for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getChannelPointer (channel);
            
            for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
            {
                channelData[sample] *= inputGainSmooth.getNextValue();
            }
        }
        
        // DC blocking
        if (dcBlockEnabled)
        {
            dcBlocker.process (context);
        }
        
        // High-pass filter
        if (hpfEnabled)
        {
            updateHighPassFilter (hpfFreq);
            highPassFilter.process (context);
        }
    }
    
private:
    void updateHighPassFilter (float frequency)
    {
        if (std::abs (frequency - lastHpfFreq) > 0.1f)
        {
            *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, frequency, 0.707);
            lastHpfFreq = frequency;
        }
    }
    
    double sampleRate = 44100.0;
    float lastHpfFreq = 40.0f;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> dcBlocker;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;
    juce::SmoothedValue<float> inputGainSmooth;
};
