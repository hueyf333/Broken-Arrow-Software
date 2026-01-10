#pragma once

#include <JuceHeader.h>

/**
 * Channel Routing - handles channel selection and input conditioning
 * - Normal channel (flat response)
 * - Bright channel (high-frequency boost)
 * - Overdrive channel (routed to overdrive section)
 */
class ChannelRouting
{
public:
    ChannelRouting() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        // Bright channel high-shelf filter (+3dB @ 2kHz)
        brightFilter.prepare (spec);
        *brightFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (
            sampleRate, 2000.0f, 0.707f, juce::Decibels::decibelsToGain (3.0f));
        
        // Channel crossfade smoothing
        channelBlendSmooth.reset (sampleRate, 0.02); // 20ms crossfade
        channelBlendSmooth.setCurrentAndTargetValue (0.0f);
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        int channelType = static_cast<int> (apvts.getRawParameterValue ("channel")->load());
        
        // Apply bright filter if bright channel selected
        if (channelType == 1) // Bright
        {
            brightFilter.process (context);
        }
        
        // Store channel type for other stages
        currentChannel = channelType;
    }
    
    int getCurrentChannel() const { return currentChannel; }
    
private:
    double sampleRate = 44100.0;
    int currentChannel = 0;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> brightFilter;
    juce::SmoothedValue<float> channelBlendSmooth;
};
