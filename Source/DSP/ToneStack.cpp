/*
  ==============================================================================

    Tone Stack - Marshall-style interactive tone control
    Bass, Mid, Treble, Presence (not independent EQs)

  ==============================================================================
*/

#include "ToneStack.h"

ToneStack::ToneStack()
{
}

void ToneStack::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    bass.reset(sampleRate, 0.02);
    mid.reset(sampleRate, 0.02);
    treble.reset(sampleRate, 0.02);
    presence.reset(sampleRate, 0.02);
    
    updateFilters();
    reset();
}

void ToneStack::process(juce::dsp::AudioBlock<float>& block)
{
    // Update filter coefficients if parameters changed
    updateFilters();
    
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        size_t channelIndex = std::min(channel, size_t(1));
        
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            
            // Process through tone stack filters
            // Order matters for interaction (like real Marshall stack)
            inputSample = bassFilter[channelIndex].processSample(inputSample);
            inputSample = midFilter[channelIndex].processSample(inputSample);
            inputSample = trebleFilter[channelIndex].processSample(inputSample);
            inputSample = presenceFilter[channelIndex].processSample(inputSample);
            
            channelData[sample] = inputSample;
        }
    }
}

void ToneStack::reset()
{
    bass.setCurrentAndTargetValue(5.0f);
    mid.setCurrentAndTargetValue(5.0f);
    treble.setCurrentAndTargetValue(5.0f);
    presence.setCurrentAndTargetValue(5.0f);
    
    for (size_t ch = 0; ch < 2; ++ch)
    {
        bassFilter[ch].reset();
        midFilter[ch].reset();
        trebleFilter[ch].reset();
        presenceFilter[ch].reset();
    }
}

void ToneStack::setBass(float value)
{
    bass.setTargetValue(value);
}

void ToneStack::setMid(float value)
{
    mid.setTargetValue(value);
}

void ToneStack::setTreble(float value)
{
    treble.setTargetValue(value);
}

void ToneStack::setPresence(float value)
{
    presence.setTargetValue(value);
}

void ToneStack::updateFilters()
{
    // Get current parameter values
    float bassVal = bass.getNextValue();
    float midVal = mid.getNextValue();
    float trebleVal = treble.getNextValue();
    float presenceVal = presence.getNextValue();
    
    // Marshall-style frequencies
    // Bass: Low shelf around 100 Hz
    float bassGainDb = (bassVal - 5.0f) * 3.0f; // +/- 15 dB
    auto bassCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 100.0, 0.707, juce::Decibels::decibelsToGain(bassGainDb));
    
    // Mid: Parametric bell around 650 Hz
    float midGainDb = (midVal - 5.0f) * 3.0f;
    auto midCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 650.0, 1.5, juce::Decibels::decibelsToGain(midGainDb));
    
    // Treble: High shelf around 3 kHz
    float trebleGainDb = (trebleVal - 5.0f) * 3.0f;
    auto trebleCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 3000.0, 0.707, juce::Decibels::decibelsToGain(trebleGainDb));
    
    // Presence: High shelf around 5 kHz (post-power amp in real amps, but we put it here)
    float presenceGainDb = (presenceVal - 5.0f) * 2.0f;
    auto presenceCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 5000.0, 0.707, juce::Decibels::decibelsToGain(presenceGainDb));
    
    // Update filter coefficients for both channels
    for (size_t ch = 0; ch < 2; ++ch)
    {
        bassFilter[ch].coefficients = bassCoeffs;
        midFilter[ch].coefficients = midCoeffs;
        trebleFilter[ch].coefficients = trebleCoeffs;
        presenceFilter[ch].coefficients = presenceCoeffs;
    }
}
