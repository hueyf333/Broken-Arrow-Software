#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "DSP/InputStage.h"
#include "DSP/ChannelRouting.h"
#include "DSP/TubePreamp.h"
#include "DSP/ToneStack.h"
#include "DSP/OverdriveChannel.h"
#include "DSP/PowerAmp.h"
#include "DSP/CabinetSim.h"
#include "DSP/OutputStage.h"
#include "DSP/Oversampling.h"

//==============================================================================
/**
    SuperBassman Audio Processor
    Fender Super Bassman Bass Head emulation
*/
class SuperBassmanAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SuperBassmanAudioProcessor();
    ~SuperBassmanAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter tree
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    
    // Level metering
    float getInputLevel() const { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }
    bool getInputClipping() const { return inputClipping.load(); }
    bool getOutputClipping() const { return outputClipping.load(); }

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // DSP components
    InputStage inputStage;
    ChannelRouting channelRouting;
    TubePreamp tubePreamp;
    ToneStack toneStack;
    OverdriveChannel overdriveChannel;
    PowerAmp powerAmp;
    CabinetSim cabinetSim;
    OutputStage outputStage;
    OversamplingEngine oversamplingEngine;
    
    // Metering
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<float> outputLevel { 0.0f };
    std::atomic<bool> inputClipping { false };
    std::atomic<bool> outputClipping { false };
    
    // State
    double currentSampleRate = 44100.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperBassmanAudioProcessor)
};
