#include "ToneStack.h"

ToneStack::ToneStack()
{
}

void ToneStack::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    updateFilters();
    reset();
}

void ToneStack::reset()
{
    bassFilter.reset();
    midFilter.reset();
    trebleFilter.reset();
    brightFilter.reset();
    ultraLoFilter.reset();
    ultraHiFilter.reset();
}

void ToneStack::updateFilters()
{
    // Bass: Low shelf at 100 Hz
    float bassGain = (bass - 0.5f) * 24.0f; // ±12 dB range
    bassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 100.0, 0.707, juce::Decibels::decibelsToGain(bassGain));
    
    // Mid: Parametric peak at selected frequency
    float midGain = (mid - 0.5f) * 24.0f;
    float midFreqHz = 800.0f;
    
    switch (midFreq)
    {
        case MidFrequency::Hz220:  midFreqHz = 220.0f;  break;
        case MidFrequency::Hz450:  midFreqHz = 450.0f;  break;
        case MidFrequency::Hz800:  midFreqHz = 800.0f;  break;
        case MidFrequency::Hz1600: midFreqHz = 1600.0f; break;
        case MidFrequency::Hz3000: midFreqHz = 3000.0f; break;
    }
    
    midFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midFreqHz, 1.5, juce::Decibels::decibelsToGain(midGain));
    
    // Treble: High shelf at 3 kHz
    float trebleGain = (treble - 0.5f) * 24.0f;
    trebleFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 3000.0, 0.707, juce::Decibels::decibelsToGain(trebleGain));
    
    // Bright switch: High shelf boost at 5 kHz
    if (brightEnabled)
        brightFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 5000.0, 0.707, juce::Decibels::decibelsToGain(6.0f));
    else
        brightFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 5000.0, 0.707, 1.0f);
    
    // Ultra-Lo: Low shelf boost at 40 Hz
    if (ultraLoEnabled)
        ultraLoFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 40.0, 0.707, juce::Decibels::decibelsToGain(8.0f));
    else
        ultraLoFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 40.0, 0.707, 1.0f);
    
    // Ultra-Hi: High shelf boost at 8 kHz
    if (ultraHiEnabled)
        ultraHiFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.0, 0.707, juce::Decibels::decibelsToGain(6.0f));
    else
        ultraHiFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.0, 0.707, 1.0f);
}

float ToneStack::processSample(float input)
{
    float output = input;
    
    // Apply tone controls
    output = bassFilter.processSample(output);
    output = midFilter.processSample(output);
    output = trebleFilter.processSample(output);
    
    // Apply voicing switches
    output = brightFilter.processSample(output);
    output = ultraLoFilter.processSample(output);
    output = ultraHiFilter.processSample(output);
    
    return output;
}

void ToneStack::setBass(float value)
{
    bass = juce::jlimit(0.0f, 1.0f, value);
    updateFilters();
}

void ToneStack::setMid(float value)
{
    mid = juce::jlimit(0.0f, 1.0f, value);
    updateFilters();
}

void ToneStack::setTreble(float value)
{
    treble = juce::jlimit(0.0f, 1.0f, value);
    updateFilters();
}

void ToneStack::setMidFrequency(MidFrequency freq)
{
    midFreq = freq;
    updateFilters();
}

void ToneStack::setBrightSwitch(bool enabled)
{
    brightEnabled = enabled;
    updateFilters();
}

void ToneStack::setUltraLoSwitch(bool enabled)
{
    ultraLoEnabled = enabled;
    updateFilters();
}

void ToneStack::setUltraHiSwitch(bool enabled)
{
    ultraHiEnabled = enabled;
    updateFilters();
}
