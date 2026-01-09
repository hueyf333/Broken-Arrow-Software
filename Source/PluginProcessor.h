/*
  ==============================================================================

    Broken Arrow M900 - Marshall JCM900-Style Amp Plugin
    Copyright (c) 2026 Broken Arrow Software

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/PreampStage.h"
#include "DSP/ToneStack.h"
#include "DSP/PowerAmp.h"
#include "DSP/CabinetSimulator.h"
#include "DSP/NoiseGenerator.h"
#include "DSP/InputStage.h"
#include "DSP/OutputStage.h"

//==============================================================================
/**
 * Main audio processor for Broken Arrow M900 plugin
 */
class BrokenArrowM900AudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    BrokenArrowM900AudioProcessor();
    ~BrokenArrowM900AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter tree
    juce::AudioProcessorValueTreeState apvts;
    
    // Level meters (atomic for thread-safe access from UI)
    std::atomic<float> inputLevel{ 0.0f };
    std::atomic<float> outputLevel{ 0.0f };
    std::atomic<bool> isClipping{ false };

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    // DSP modules
    std::unique_ptr<InputStage> inputStage;
    std::unique_ptr<PreampStage> preampStage;
    std::unique_ptr<ToneStack> toneStack;
    std::unique_ptr<PowerAmp> powerAmp;
    std::unique_ptr<CabinetSimulator> cabinetSim;
    std::unique_ptr<NoiseGenerator> noiseGen;
    std::unique_ptr<OutputStage> outputStage;
    
    // Oversampling
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    int currentOversamplingFactor = 1;
    
    // Buffer for oversampled processing
    juce::AudioBuffer<float> oversampledBuffer;
    
    // Thread-safe parameter update
    void updateDSPParameters();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrokenArrowM900AudioProcessor)
};
