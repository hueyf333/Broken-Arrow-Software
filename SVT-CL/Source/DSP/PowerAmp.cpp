#include "PowerAmp.h"

PowerAmp::PowerAmp()
{
}

void PowerAmp::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    oversampler.initProcessing(spec.maximumBlockSize);
    
    masterSmoothed.reset(sampleRate, 0.05);
    masterSmoothed.setCurrentAndTargetValue(0.0f);
    
    // Configure compressor for power amp behavior
    compressor.prepare(spec);
    compressor.setThreshold(-10.0f);
    compressor.setRatio(4.0f);
    compressor.setAttack(5.0f);
    compressor.setRelease(100.0f);
    
    // Configure output limiter
    outputLimiter.prepare(spec);
    outputLimiter.setThreshold(0.95f);
    outputLimiter.setRelease(50.0f);
}

void PowerAmp::reset()
{
    oversampler.reset();
    masterSmoothed.reset(sampleRate, 0.05);
    compressor.reset();
    outputLimiter.reset();
}

void PowerAmp::process(juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    
    // Apply master volume
    float master = masterSmoothed.getNextValue();
    float masterGain = juce::Decibels::decibelsToGain(master);
    
    // Apply compression first
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    compressor.process(context);
    
    // Oversample for saturation
    auto oversampledBlock = oversampler.processSamplesUp(block);
    const int oversampledSamples = (int)oversampledBlock.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = oversampledBlock.getChannelPointer((size_t)channel);
        
        for (int sample = 0; sample < oversampledSamples; ++sample)
        {
            float x = channelData[sample] * masterGain;
            
            // Power amp saturation
            x = powerAmpSaturation(x, 1.5f);
            
            channelData[sample] = x;
        }
    }
    
    // Downsample
    oversampler.processSamplesDown(block);
    
    // Apply output limiter
    outputLimiter.process(context);
}

void PowerAmp::setMaster(float level)
{
    masterSmoothed.setTargetValue(level);
}

float PowerAmp::powerAmpSaturation(float input, float drive)
{
    // Power amp style saturation with asymmetric behavior
    float x = input * drive;
    
    // Soft knee compression leading to hard clipping
    float abs_x = std::abs(x);
    
    if (abs_x < 0.5f)
    {
        // Clean region
        return x;
    }
    else if (abs_x < 1.0f)
    {
        // Soft saturation region
        float sign = x > 0.0f ? 1.0f : -1.0f;
        float compressed = 0.5f + (abs_x - 0.5f) * (1.0f - (abs_x - 0.5f) * 0.5f);
        return sign * compressed;
    }
    else
    {
        // Hard limiting region
        float sign = x > 0.0f ? 1.0f : -1.0f;
        return sign * (1.0f - std::exp(-(abs_x - 1.0f)));
    }
}
