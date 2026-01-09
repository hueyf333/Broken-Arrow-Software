/*
  ==============================================================================

    Output Stage - Output gain and soft-clip safety limiter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OutputStage
{
public:
    OutputStage();
    ~OutputStage() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setOutputGain(float gainDb);

private:
    float applySoftClipper(float input);
    
    juce::SmoothedValue<float> outputGain{ 1.0f };
    double sampleRate = 44100.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputStage)
};
