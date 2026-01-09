/*
  ==============================================================================

    Preamp Stage - Cascaded non-linear tube-like stages
    Models JCM900 preamp: tight crunch, aggressive upper mids, fizzy edge

  ==============================================================================
*/

#include "PreampStage.h"

PreampStage::PreampStage()
{
}

void PreampStage::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    gain.reset(sampleRate, 0.02); // 20ms smoothing
    
    // DC blocker at 10 Hz
    auto dcCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 10.0);
    for (size_t ch = 0; ch < 2; ++ch)
    {
        dcBlocker[ch].coefficients = dcCoeffs;
        dcBlocker[ch].reset();
        
        // Pre-emphasis at 720 Hz for upper-mid presence
        auto preEmpCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 720.0, 0.7, 2.0);
        preEmphasis[ch].coefficients = preEmpCoeffs;
        preEmphasis[ch].reset();
    }
    
    reset();
}

void PreampStage::process(juce::dsp::AudioBlock<float>& block)
{
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        size_t channelIndex = std::min(channel, size_t(1));
        
        for (size_t sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            
            // Pre-emphasis for upper-mid character
            inputSample = preEmphasis[channelIndex].processSample(inputSample);
            
            float currentGain = gain.getNextValue();
            
            // Mode-dependent drive and bias settings
            float drive = 1.0f;
            float bias = 0.0f;
            
            switch (currentMode)
            {
                case 0: // Clean
                    drive = currentGain * 0.5f;
                    bias = 0.1f;
                    break;
                    
                case 1: // Crunch
                    drive = currentGain * 1.0f;
                    bias = 0.0f;
                    break;
                    
                case 2: // High Gain
                    drive = currentGain * 2.0f;
                    bias = -0.1f;
                    break;
            }
            
            // Cascaded tube stages (3 stages for JCM900-like character)
            float output = inputSample;
            
            // Stage 1 - Input gain stage
            output = applyTubeStage(output * drive, bias, 1.0f);
            
            // Stage 2 - Intermediate gain stage
            output = applyTubeStage(output * 1.2f, bias * 0.8f, 1.0f);
            
            // Stage 3 - Final preamp stage
            output = applyTubeStage(output * 1.1f, bias * 0.6f, 1.0f);
            
            // DC blocking
            output = dcBlocker[channelIndex].processSample(output);
            
            channelData[sample] = output;
        }
    }
}

void PreampStage::reset()
{
    gain.setCurrentAndTargetValue(5.0f);
    
    for (size_t ch = 0; ch < 2; ++ch)
    {
        dcBlocker[ch].reset();
        preEmphasis[ch].reset();
    }
}

void PreampStage::setGain(float newGain)
{
    gain.setTargetValue(newGain);
}

void PreampStage::setMode(int mode)
{
    currentMode = juce::jlimit(0, 2, mode);
}

float PreampStage::applyTubeStage(float input, float bias, float drive)
{
    // Asymmetric waveshaping for tube-like distortion
    // Biased tanh for warmth and harmonics
    
    float x = input * drive + bias;
    
    // Asymmetric soft clipping using modified tanh
    // This creates more even harmonics (warmer tone)
    float output = std::tanh(x);
    
    // Add slight asymmetry for more character
    if (x > 0.0f)
        output *= 1.0f + 0.1f * x; // Gentle compression on positive peaks
    else
        output *= 1.0f + 0.05f * std::abs(x); // Less compression on negative peaks
    
    // Normalize
    output = std::tanh(output);
    
    return output * 0.7f; // Scale down to prevent cascading buildup
}
