#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "DSP/InputStage.h"
#include "DSP/TubePreamp.h"
#include "DSP/ToneStack.h"
#include "DSP/PowerAmp.h"
#include "DSP/CabSimulator.h"
#include "DSP/Oversampler.h"

class AmpegSVTAudioProcessor : public juce::AudioProcessor
{
public:
    AmpegSVTAudioProcessor();
    ~AmpegSVTAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter tree
    juce::AudioProcessorValueTreeState apvts;
    
    // Metering
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<float> outputLevel { 0.0f };
    std::atomic<bool> clipping { false };

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void updateParameters();
    
    // DSP modules
    InputStage inputStage;
    TubePreamp tubePreamp;
    ToneStack toneStack;
    PowerAmp powerAmp;
    CabSimulator cabSimulator;
    Oversampler oversampler;
    
    // Parameter smoothing
    juce::SmoothedValue<float> inputGainSmooth;
    juce::SmoothedValue<float> outputGainSmooth;
    juce::SmoothedValue<float> driveSmooth;
    juce::SmoothedValue<float> masterSmooth;
    juce::SmoothedValue<float> bassSmooth;
    juce::SmoothedValue<float> midSmooth;
    juce::SmoothedValue<float> trebleSmooth;
    
    // Output protection
    juce::dsp::Limiter<float> outputLimiter;
    
    // Bypass
    bool bypassed { false };
    juce::AudioBuffer<float> bypassBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpegSVTAudioProcessor)
};
