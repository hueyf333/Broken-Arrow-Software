/*
  ==============================================================================

    Cabinet Simulator - IR loader with partitioned FFT convolution

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CabinetSimulator
{
public:
    CabinetSimulator();
    ~CabinetSimulator() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();

    void setIRSelection(int irIndex); // 0-2 for factory IRs
    void loadUserIR(const juce::File& irFile);

private:
    void loadFactoryIR(int index);
    juce::AudioBuffer<float> createFactoryIR(int index, double sampleRate);
    
    juce::dsp::Convolution convolutionL;
    juce::dsp::Convolution convolutionR;
    
    int currentIRIndex = 0;
    double sampleRate = 44100.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabinetSimulator)
};
