#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Cabinet Simulation
 * Simulates various bass cabinet configurations:
 * - 8x10 sealed (classic SVT-style)
 * - 4x10 ported (modern punch)
 * - 1x15 sealed (vintage thump)
 */
class CabinetSim
{
public:
    CabinetSim() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        // Prepare cabinet filters
        cab8x10Filter.prepare (spec);
        cab4x10Filter.prepare (spec);
        cab1x15Filter.prepare (spec);
        
        mixSmooth.reset (sampleRate, 0.05);
        mixSmooth.setCurrentAndTargetValue (1.0f);
        
        // Pre-allocate dry buffer for wet/dry mixing
        dryBuffer.setSize (static_cast<int> (spec.numChannels), 
                          static_cast<int> (spec.maximumBlockSize), false, false, true);
        
        updateCabinetFilters();
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        bool cabEnabled = apvts.getRawParameterValue ("cabEnable")->load();
        int cabType = static_cast<int> (apvts.getRawParameterValue ("cabType")->load());
        float cabMix = apvts.getRawParameterValue ("cabMix")->load() / 100.0f;
        
        if (!cabEnabled)
            return;
        
        mixSmooth.setTargetValue (cabMix);
        
        // Use pre-allocated dry buffer for mixing
        for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
            dryBuffer.copyFrom (static_cast<int> (ch), 0, block.getChannelPointer (ch),
                               static_cast<int> (block.getNumSamples()));
        
        // Apply selected cabinet simulation
        switch (cabType)
        {
            case 0: // 8x10 Sealed
                cab8x10Filter.process (context);
                break;
            case 1: // 4x10 Ported
                cab4x10Filter.process (context);
                break;
            case 2: // 1x15 Sealed
                cab1x15Filter.process (context);
                break;
        }
        
        // Mix wet/dry
        for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
        {
            auto* wetData = block.getChannelPointer (ch);
            const auto* dryData = dryBuffer.getReadPointer (static_cast<int> (ch));
            
            for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
            {
                float mix = mixSmooth.getNextValue();
                wetData[sample] = wetData[sample] * mix + dryData[sample] * (1.0f - mix);
            }
        }
    }
    
private:
    void updateCabinetFilters()
    {
        // 8x10 Sealed - tight, focused, classic
        // Low-pass at 5 kHz with resonance at 2 kHz
        *cab8x10Filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, 5000.0f, 0.5f);
        
        // 4x10 Ported - modern, punchy
        // Low-pass at 6 kHz with less resonance
        *cab4x10Filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, 6000.0f, 0.707f);
        
        // 1x15 Sealed - vintage, warm, deep
        // Low-pass at 3 kHz with high resonance
        *cab1x15Filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, 3000.0f, 0.4f);
    }
    
    double sampleRate = 44100.0;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> cab8x10Filter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> cab4x10Filter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> cab1x15Filter;
    
    juce::SmoothedValue<float> mixSmooth;
    juce::AudioBuffer<float> dryBuffer;  // Pre-allocated for wet/dry mixing
};
