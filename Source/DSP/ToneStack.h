#pragma once

#include <juce_dsp/juce_dsp.h>

class ToneStack
{
public:
    ToneStack();
    ~ToneStack() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    float processSample(float input);
    
    // Tone controls
    void setBass(float value);      // 0-1
    void setMid(float value);       // 0-1
    void setTreble(float value);    // 0-1
    
    // Mid frequency selector (Heritage SVT centers)
    enum class MidFrequency
    {
        Hz220,
        Hz450,
        Hz800,
        Hz1600,
        Hz3000
    };
    
    void setMidFrequency(MidFrequency freq);
    
    // Voicing switches
    void setBrightSwitch(bool enabled);
    void setUltraLoSwitch(bool enabled);
    void setUltraHiSwitch(bool enabled);

private:
    void updateFilters();
    
    float bass { 0.5f };
    float mid { 0.5f };
    float treble { 0.5f };
    
    MidFrequency midFreq { MidFrequency::Hz800 };
    
    bool brightEnabled { false };
    bool ultraLoEnabled { false };
    bool ultraHiEnabled { false };
    
    float sampleRate { 44100.0f };
    
    // Filter banks
    juce::dsp::IIR::Filter<float> bassFilter;
    juce::dsp::IIR::Filter<float> midFilter;
    juce::dsp::IIR::Filter<float> trebleFilter;
    juce::dsp::IIR::Filter<float> brightFilter;
    juce::dsp::IIR::Filter<float> ultraLoFilter;
    juce::dsp::IIR::Filter<float> ultraHiFilter;
};
