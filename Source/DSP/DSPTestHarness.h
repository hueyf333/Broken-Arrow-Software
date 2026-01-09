/*
  ==============================================================================

    DSP Test Harness - Offline validation of DSP modules

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/InputStage.h"
#include "DSP/PreampStage.h"
#include "DSP/ToneStack.h"
#include "DSP/PowerAmp.h"
#include "DSP/CabinetSimulator.h"
#include "DSP/NoiseGenerator.h"
#include "DSP/OutputStage.h"

class DSPTestHarness
{
public:
    DSPTestHarness();
    ~DSPTestHarness() = default;

    // Run all tests
    bool runAllTests();

    // Individual test methods
    bool testInputStage();
    bool testPreampStage();
    bool testToneStack();
    bool testPowerAmp();
    bool testCabinetSimulator();
    bool testNoiseGenerator();
    bool testOutputStage();
    bool testFullChain();

private:
    void logMessage(const juce::String& message);
    bool checkStability(const juce::AudioBuffer<float>& buffer);
    bool checkLevels(const juce::AudioBuffer<float>& buffer, float maxDb = 6.0f);

    juce::StringArray testLog;
};
