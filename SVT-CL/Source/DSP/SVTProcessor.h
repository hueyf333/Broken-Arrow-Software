#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <atomic>
#include "TubePreamp.h"
#include "ToneStack.h"
#include "PowerAmp.h"
#include "CabSimulator.h"

class SVTProcessor
{
public:
    SVTProcessor();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);
    
    // Parameter setters
    void setGain(float gainDb);
    void setPad(bool usePad);
    void setBass(float value);
    void setMid(float value);
    void setTreble(float value);
    void setMidFrequency(ToneStack::MidFrequency freq);
    void setUltraLo(float value);
    void setUltraHi(float value);
    void setMaster(float level);
    void setCabEnabled(bool enabled);
    void setStandby(bool standby);
    
    // Metering
    float getInputLevel() const { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }
    bool getInputClipping() const { return inputClipping.load(); }
    bool getOutputClipping() const { return outputClipping.load(); }
    
private:
    void updateMetering(const juce::AudioBuffer<float>& buffer, bool isInput);
    
    TubePreamp preamp;
    ToneStack toneStack;
    PowerAmp powerAmp;
    CabSimulator cabSim;
    
    std::atomic<float> inputLevel{0.0f};
    std::atomic<float> outputLevel{0.0f};
    std::atomic<bool> inputClipping{false};
    std::atomic<bool> outputClipping{false};
    
    bool isStandby{false};
    int standbyWarmupSamples{0};
    static constexpr int standbyWarmupTime = 88200; // 2 seconds at 44.1kHz
};
