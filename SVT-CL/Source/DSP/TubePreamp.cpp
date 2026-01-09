#include "TubePreamp.h"

TubePreamp::TubePreamp()
{
}

void TubePreamp::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    oversampler.initProcessing(spec.maximumBlockSize);
    
    gainSmoothed.reset(sampleRate, 0.05);
    
    // Initialize DC blockers for each stage
    for (auto& blocker : dcBlockers)
    {
        blocker.prepare(spec);
        *blocker.coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 10.0);
    }
}

void TubePreamp::reset()
{
    oversampler.reset();
    gainSmoothed.reset(sampleRate, 0.05);
    
    for (auto& blocker : dcBlockers)
        blocker.reset();
}

void TubePreamp::process(juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    
    // Oversample the processing
    juce::dsp::AudioBlock<float> block(buffer);
    auto oversampledBlock = oversampler.processSamplesUp(block);
    
    const int oversampledSamples = (int)oversampledBlock.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer((size_t)channel);
        
        for (int sample = 0; sample < oversampledSamples; ++sample)
        {
            float x = channelData[sample];
            
            // Apply pad attenuation
            x *= padAttenuation;
            
            // Get current gain value
            float gain = gainSmoothed.getNextValue();
            float gainLinear = juce::Decibels::decibelsToGain(gain);
            
            // Multi-stage tube preamp simulation
            // Each stage adds harmonics and compression
            for (int stage = 0; stage < numStages; ++stage)
            {
                float stageDrive = gainLinear * (1.0f + stage * 0.3f);
                x = processStage(x, stageDrive);
            }
            
            channelData[sample] = x;
        }
    }
    
    // Downsample
    oversampler.processSamplesDown(block);
    
    // Apply DC blocking to each channel
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto channelBlock = block.getSingleChannelBlock((size_t)channel);
        juce::dsp::ProcessContextReplacing<float> context(channelBlock);
        dcBlockers[0].process(context);
    }
}

void TubePreamp::setGain(float gainDb)
{
    gainSmoothed.setTargetValue(gainDb);
}

void TubePreamp::setPad(bool usePad)
{
    padAttenuation = usePad ? 0.5f : 1.0f;
}

float TubePreamp::processStage(float input, float drive)
{
    // Apply gain
    float x = input * drive;
    
    // Asymmetric waveshaping for tube-like behavior
    x = asymmetricWaveShaper(x, drive);
    
    // Slight attenuation between stages
    return x * 0.7f;
}

float TubePreamp::asymmetricWaveShaper(float x, float drive)
{
    // Normalize drive for consistent behavior
    float normDrive = juce::jlimit(0.1f, 10.0f, drive);
    
    // Asymmetric soft clipping that mimics tube characteristics
    // Positive and negative sides behave differently
    if (x > 0.0f)
    {
        // Positive side - harder clipping
        float threshold = 0.7f / normDrive;
        if (x < threshold)
            return x;
        else
            return threshold + (1.0f - threshold) * std::tanh((x - threshold) * 2.0f);
    }
    else
    {
        // Negative side - softer clipping
        float threshold = -0.8f / normDrive;
        if (x > threshold)
            return x;
        else
            return threshold + (1.0f + threshold) * std::tanh((x - threshold) * 1.5f);
    }
}
