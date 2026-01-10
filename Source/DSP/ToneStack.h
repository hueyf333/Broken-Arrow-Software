#pragma once

#include <JuceHeader.h>

/**
 * Fender-style Bass Tone Stack
 * Interactive EQ with bass, mid, treble controls
 * Based on classic Fender bass amp topology
 */
class ToneStack
{
public:
    ToneStack() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        
        // Prepare filters
        bassFilter.prepare (spec);
        midFilter.prepare (spec);
        trebleFilter.prepare (spec);
        presenceFilter.prepare (spec);
        
        // Initialize smoothed values
        bassSmooth.reset (sampleRate, 0.05);
        midSmooth.reset (sampleRate, 0.05);
        trebleSmooth.reset (sampleRate, 0.05);
        presenceSmooth.reset (sampleRate, 0.05);
        
        bassSmooth.setCurrentAndTargetValue (5.0f);
        midSmooth.setCurrentAndTargetValue (5.0f);
        trebleSmooth.setCurrentAndTargetValue (5.0f);
        presenceSmooth.setCurrentAndTargetValue (5.0f);
        
        updateFilters();
    }
    
    void process (juce::dsp::ProcessContextReplacing<float>& context,
                  juce::AudioProcessorValueTreeState& apvts)
    {
        auto& block = context.getOutputBlock();
        
        float bass = apvts.getRawParameterValue ("bass")->load();
        float mid = apvts.getRawParameterValue ("mid")->load();
        float treble = apvts.getRawParameterValue ("treble")->load();
        float presence = apvts.getRawParameterValue ("presence")->load();
        
        bassSmooth.setTargetValue (bass);
        midSmooth.setTargetValue (mid);
        trebleSmooth.setTargetValue (treble);
        presenceSmooth.setTargetValue (presence);
        
        // Update filter coefficients if controls changed
        if (shouldUpdateFilters())
        {
            updateFilters();
        }
        
        // Apply tone stack filters
        bassFilter.process (context);
        midFilter.process (context);
        trebleFilter.process (context);
        presenceFilter.process (context);
    }
    
private:
    void updateFilters()
    {
        float bassVal = bassSmooth.getTargetValue();
        float midVal = midSmooth.getTargetValue();
        float trebleVal = trebleSmooth.getTargetValue();
        float presenceVal = presenceSmooth.getTargetValue();
        
        // Bass: Low shelf centered at 100 Hz
        float bassGain = (bassVal - 5.0f) * 2.0f; // ±10 dB
        *bassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (
            sampleRate, 100.0f, 0.707f, juce::Decibels::decibelsToGain (bassGain));
        
        // Mid: Peaking filter at 500 Hz
        float midGain = (midVal - 5.0f) * 1.5f; // ±7.5 dB
        *midFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (
            sampleRate, 500.0f, 1.0f, juce::Decibels::decibelsToGain (midGain));
        
        // Treble: High shelf at 3 kHz
        float trebleGain = (trebleVal - 5.0f) * 2.0f; // ±10 dB
        *trebleFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (
            sampleRate, 3000.0f, 0.707f, juce::Decibels::decibelsToGain (trebleGain));
        
        // Presence: High shelf at 6 kHz
        float presenceGain = (presenceVal - 5.0f) * 1.5f; // ±7.5 dB
        *presenceFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (
            sampleRate, 6000.0f, 0.707f, juce::Decibels::decibelsToGain (presenceGain));
        
        lastBass = bassVal;
        lastMid = midVal;
        lastTreble = trebleVal;
        lastPresence = presenceVal;
    }
    
    bool shouldUpdateFilters()
    {
        return std::abs (bassSmooth.getTargetValue() - lastBass) > 0.01f ||
               std::abs (midSmooth.getTargetValue() - lastMid) > 0.01f ||
               std::abs (trebleSmooth.getTargetValue() - lastTreble) > 0.01f ||
               std::abs (presenceSmooth.getTargetValue() - lastPresence) > 0.01f;
    }
    
    double sampleRate = 44100.0;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> bassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> trebleFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> presenceFilter;
    
    juce::SmoothedValue<float> bassSmooth;
    juce::SmoothedValue<float> midSmooth;
    juce::SmoothedValue<float> trebleSmooth;
    juce::SmoothedValue<float> presenceSmooth;
    
    float lastBass = 5.0f;
    float lastMid = 5.0f;
    float lastTreble = 5.0f;
    float lastPresence = 5.0f;
};
