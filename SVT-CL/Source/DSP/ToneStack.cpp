#include "ToneStack.h"

ToneStack::ToneStack()
{
}

void ToneStack::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    filterChain.prepare(spec);
    
    bassSmoothed.reset(sampleRate, 0.05);
    bassSmoothed.setCurrentAndTargetValue(0.5f);
    midSmoothed.reset(sampleRate, 0.05);
    midSmoothed.setCurrentAndTargetValue(0.5f);
    trebleSmoothed.reset(sampleRate, 0.05);
    trebleSmoothed.setCurrentAndTargetValue(0.5f);
    ultraLoSmoothed.reset(sampleRate, 0.05);
    ultraLoSmoothed.setCurrentAndTargetValue(0.0f);
    ultraHiSmoothed.reset(sampleRate, 0.05);
    ultraHiSmoothed.setCurrentAndTargetValue(0.0f);
    
    updateFilters();
}

void ToneStack::reset()
{
    filterChain.reset();
    bassSmoothed.reset(sampleRate, 0.05);
    midSmoothed.reset(sampleRate, 0.05);
    trebleSmoothed.reset(sampleRate, 0.05);
    ultraLoSmoothed.reset(sampleRate, 0.05);
    ultraHiSmoothed.reset(sampleRate, 0.05);
}

void ToneStack::process(juce::AudioBuffer<float>& buffer)
{
    // Update filters if parameters have changed
    if (bassSmoothed.isSmoothing() || midSmoothed.isSmoothing() || 
        trebleSmoothed.isSmoothing() || ultraLoSmoothed.isSmoothing() || 
        ultraHiSmoothed.isSmoothing())
    {
        updateFilters();
    }
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filterChain.process(context);
}

void ToneStack::setBass(float value)
{
    bassSmoothed.setTargetValue(juce::jlimit(0.0f, 1.0f, value));
}

void ToneStack::setMid(float value)
{
    midSmoothed.setTargetValue(juce::jlimit(0.0f, 1.0f, value));
}

void ToneStack::setTreble(float value)
{
    trebleSmoothed.setTargetValue(juce::jlimit(0.0f, 1.0f, value));
}

void ToneStack::setMidFrequency(MidFrequency freq)
{
    midFreq = freq;
    updateFilters();
}

void ToneStack::setUltraLo(float value)
{
    ultraLoSmoothed.setTargetValue(juce::jlimit(0.0f, 1.0f, value));
}

void ToneStack::setUltraHi(float value)
{
    ultraHiSmoothed.setTargetValue(juce::jlimit(0.0f, 1.0f, value));
}

void ToneStack::updateFilters()
{
    // Bass control - low shelf at 80 Hz
    float bassGain = (bassSmoothed.getNextValue() - 0.5f) * 24.0f; // ±12 dB
    *filterChain.get<0>().coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 80.0, 0.707, juce::Decibels::decibelsToGain(bassGain));
    
    // Mid control - parametric peak at selected frequency
    float midFreqValue = 800.0f;
    switch (midFreq)
    {
        case Hz220:  midFreqValue = 220.0f; break;
        case Hz450:  midFreqValue = 450.0f; break;
        case Hz800:  midFreqValue = 800.0f; break;
        case Hz1600: midFreqValue = 1600.0f; break;
        case Hz3000: midFreqValue = 3000.0f; break;
    }
    
    float midGain = (midSmoothed.getNextValue() - 0.5f) * 24.0f; // ±12 dB
    *filterChain.get<1>().coefficients = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, midFreqValue, 1.5, juce::Decibels::decibelsToGain(midGain));
    
    // Treble control - high shelf at 5 kHz
    float trebleGain = (trebleSmoothed.getNextValue() - 0.5f) * 24.0f; // ±12 dB
    *filterChain.get<2>().coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 5000.0, 0.707, juce::Decibels::decibelsToGain(trebleGain));
    
    // Ultra Lo - low pass filter
    float ultraLoValue = ultraLoSmoothed.getNextValue();
    if (ultraLoValue > 0.01f)
    {
        float ultraLoCutoff = 40.0f + (1.0f - ultraLoValue) * 160.0f; // 40-200 Hz
        *filterChain.get<3>().coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
            sampleRate, ultraLoCutoff, 0.707);
    }
    else
    {
        // Bypass when off
        *filterChain.get<3>().coefficients = *juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, 1000.0);
    }
    
    // Ultra Hi - high pass filter
    float ultraHiValue = ultraHiSmoothed.getNextValue();
    if (ultraHiValue > 0.01f)
    {
        float ultraHiCutoff = 3000.0f + ultraHiValue * 5000.0f; // 3-8 kHz
        *filterChain.get<4>().coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass(
            sampleRate, ultraHiCutoff, 0.707);
    }
    else
    {
        // Bypass when off
        *filterChain.get<4>().coefficients = *juce::dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, 1000.0);
    }
}
