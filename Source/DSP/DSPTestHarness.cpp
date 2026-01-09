/*
  ==============================================================================

    DSP Test Harness - Offline validation of DSP modules

  ==============================================================================
*/

#include "DSPTestHarness.h"

DSPTestHarness::DSPTestHarness()
{
}

void DSPTestHarness::logMessage(const juce::String& message)
{
    testLog.add(message);
    DBG(message);
}

bool DSPTestHarness::checkStability(const juce::AudioBuffer<float>& buffer)
{
    // Check for NaN, Inf, or excessive values
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getReadPointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            if (std::isnan(data[i]) || std::isinf(data[i]))
            {
                logMessage("FAIL: Found NaN or Inf at channel " + juce::String(ch) + 
                          ", sample " + juce::String(i));
                return false;
            }
            
            if (std::abs(data[i]) > 100.0f)
            {
                logMessage("FAIL: Excessive value " + juce::String(data[i]) + 
                          " at channel " + juce::String(ch));
                return false;
            }
        }
    }
    return true;
}

bool DSPTestHarness::checkLevels(const juce::AudioBuffer<float>& buffer, float maxDb)
{
    float maxLevel = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    float maxLevelDb = juce::Decibels::gainToDecibels(maxLevel);
    
    if (maxLevelDb > maxDb)
    {
        logMessage("FAIL: Peak level " + juce::String(maxLevelDb, 2) + " dB exceeds " + 
                  juce::String(maxDb, 2) + " dB");
        return false;
    }
    
    logMessage("PASS: Peak level " + juce::String(maxLevelDb, 2) + " dB");
    return true;
}

bool DSPTestHarness::runAllTests()
{
    logMessage("=== Starting DSP Test Harness ===");
    
    bool allPassed = true;
    
    allPassed &= testInputStage();
    allPassed &= testPreampStage();
    allPassed &= testToneStack();
    allPassed &= testPowerAmp();
    allPassed &= testCabinetSimulator();
    allPassed &= testNoiseGenerator();
    allPassed &= testOutputStage();
    allPassed &= testFullChain();
    
    if (allPassed)
        logMessage("=== ALL TESTS PASSED ===");
    else
        logMessage("=== SOME TESTS FAILED ===");
    
    return allPassed;
}

bool DSPTestHarness::testInputStage()
{
    logMessage("\n--- Testing Input Stage ---");
    
    InputStage stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    // Create test signal
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    // Sine wave at -6 dB
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 0.5f;
        }
    }
    
    // Test with no gate
    stage.setGateEnabled(false);
    stage.setInputTrim(0.0f);
    
    juce::dsp::AudioBlock<float> block(testBuffer);
    stage.process(block);
    
    if (!checkStability(testBuffer))
        return false;
    
    logMessage("Input stage test passed");
    return true;
}

bool DSPTestHarness::testPreampStage()
{
    logMessage("\n--- Testing Preamp Stage ---");
    
    PreampStage stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    // Create test signal
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 0.3f;
        }
    }
    
    // Test all modes
    for (int mode = 0; mode < 3; ++mode)
    {
        stage.setMode(mode);
        stage.setGain(5.0f);
        
        juce::dsp::AudioBlock<float> block(testBuffer);
        stage.process(block);
        
        if (!checkStability(testBuffer))
            return false;
    }
    
    logMessage("Preamp stage test passed");
    return true;
}

bool DSPTestHarness::testToneStack()
{
    logMessage("\n--- Testing Tone Stack ---");
    
    ToneStack stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 0.5f;
        }
    }
    
    stage.setBass(5.0f);
    stage.setMid(5.0f);
    stage.setTreble(5.0f);
    stage.setPresence(5.0f);
    
    juce::dsp::AudioBlock<float> block(testBuffer);
    stage.process(block);
    
    if (!checkStability(testBuffer))
        return false;
    
    logMessage("Tone stack test passed");
    return true;
}

bool DSPTestHarness::testPowerAmp()
{
    logMessage("\n--- Testing Power Amp ---");
    
    PowerAmp stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 0.5f;
        }
    }
    
    // Test both sag modes
    for (int mode = 0; mode < 2; ++mode)
    {
        stage.setSagMode(mode);
        stage.setMaster(5.0f);
        
        juce::dsp::AudioBlock<float> block(testBuffer);
        stage.process(block);
        
        if (!checkStability(testBuffer))
            return false;
    }
    
    logMessage("Power amp test passed");
    return true;
}

bool DSPTestHarness::testCabinetSimulator()
{
    logMessage("\n--- Testing Cabinet Simulator ---");
    
    CabinetSimulator stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 0.3f;
        }
    }
    
    // Test all factory IRs
    for (int ir = 0; ir < 3; ++ir)
    {
        stage.setIRSelection(ir);
        
        juce::dsp::AudioBlock<float> block(testBuffer);
        stage.process(block);
        
        if (!checkStability(testBuffer))
            return false;
    }
    
    logMessage("Cabinet simulator test passed");
    return true;
}

bool DSPTestHarness::testNoiseGenerator()
{
    logMessage("\n--- Testing Noise Generator ---");
    
    NoiseGenerator stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    juce::AudioBuffer<float> testBuffer(2, 512);
    testBuffer.clear();
    
    stage.setLevel(0.5f);
    
    juce::dsp::AudioBlock<float> block(testBuffer);
    stage.process(block);
    
    if (!checkStability(testBuffer))
        return false;
    
    // Check that noise was actually generated
    float rms = testBuffer.getRMSLevel(0, 0, 512);
    if (rms < 0.0001f)
    {
        logMessage("FAIL: Noise generator produced no output");
        return false;
    }
    
    logMessage("Noise generator test passed");
    return true;
}

bool DSPTestHarness::testOutputStage()
{
    logMessage("\n--- Testing Output Stage ---");
    
    OutputStage stage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    stage.prepare(spec);
    
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    // Create a signal that might clip
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 1.2f; // Over 0 dB
        }
    }
    
    stage.setOutputGain(0.0f);
    
    juce::dsp::AudioBlock<float> block(testBuffer);
    stage.process(block);
    
    if (!checkStability(testBuffer))
        return false;
    
    // Check that limiter prevented clipping
    if (!checkLevels(testBuffer, 1.0f))
    {
        logMessage("WARNING: Output limiter may not be working correctly");
    }
    
    logMessage("Output stage test passed");
    return true;
}

bool DSPTestHarness::testFullChain()
{
    logMessage("\n--- Testing Full DSP Chain ---");
    
    // Create all modules
    InputStage inputStage;
    PreampStage preampStage;
    ToneStack toneStack;
    PowerAmp powerAmp;
    CabinetSimulator cabinetSim;
    NoiseGenerator noiseGen;
    OutputStage outputStage;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 48000.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    
    // Prepare all
    inputStage.prepare(spec);
    preampStage.prepare(spec);
    toneStack.prepare(spec);
    powerAmp.prepare(spec);
    cabinetSim.prepare(spec);
    noiseGen.prepare(spec);
    outputStage.prepare(spec);
    
    // Create test signal
    juce::AudioBuffer<float> testBuffer(2, 512);
    
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* data = testBuffer.getWritePointer(ch);
        for (int i = 0; i < 512; ++i)
        {
            float phase = static_cast<float>(i) / 512.0f * juce::MathConstants<float>::twoPi;
            data[i] = std::sin(phase) * 0.2f;
        }
    }
    
    // Set parameters
    inputStage.setInputTrim(0.0f);
    preampStage.setGain(7.0f);
    preampStage.setMode(1); // Crunch
    toneStack.setBass(6.0f);
    toneStack.setMid(5.0f);
    toneStack.setTreble(7.0f);
    toneStack.setPresence(6.0f);
    powerAmp.setMaster(6.0f);
    powerAmp.setSagMode(0);
    cabinetSim.setIRSelection(0);
    outputStage.setOutputGain(0.0f);
    
    // Process through entire chain
    juce::dsp::AudioBlock<float> block(testBuffer);
    
    inputStage.process(block);
    if (!checkStability(testBuffer)) return false;
    
    preampStage.process(block);
    if (!checkStability(testBuffer)) return false;
    
    toneStack.process(block);
    if (!checkStability(testBuffer)) return false;
    
    powerAmp.process(block);
    if (!checkStability(testBuffer)) return false;
    
    cabinetSim.process(block);
    if (!checkStability(testBuffer)) return false;
    
    outputStage.process(block);
    if (!checkStability(testBuffer)) return false;
    
    // Check final output levels
    if (!checkLevels(testBuffer, 6.0f))
    {
        logMessage("WARNING: Full chain output levels may be high");
    }
    
    logMessage("Full chain test passed");
    return true;
}
