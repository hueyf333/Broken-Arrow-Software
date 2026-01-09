#include "SVTProcessor.h"

SVTProcessor::SVTProcessor()
{
}

void SVTProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    preamp.prepare(spec);
    toneStack.prepare(spec);
    powerAmp.prepare(spec);
    cabSim.prepare(spec);
}

void SVTProcessor::reset()
{
    preamp.reset();
    toneStack.reset();
    powerAmp.reset();
    cabSim.reset();
    
    inputLevel.store(0.0f);
    outputLevel.store(0.0f);
    inputClipping.store(false);
    outputClipping.store(false);
}

void SVTProcessor::process(juce::AudioBuffer<float>& buffer)
{
    // Handle standby mode
    if (isStandby)
    {
        buffer.clear();
        standbyWarmupSamples = 0;
        return;
    }
    
    // Warm-up fade-in after leaving standby
    if (standbyWarmupSamples < standbyWarmupTime)
    {
        float fadeGain = (float)standbyWarmupSamples / (float)standbyWarmupTime;
        buffer.applyGain(fadeGain);
        standbyWarmupSamples += buffer.getNumSamples();
    }
    
    // Measure input level
    updateMetering(buffer, true);
    
    // Process through signal chain
    preamp.process(buffer);
    toneStack.process(buffer);
    powerAmp.process(buffer);
    cabSim.process(buffer);
    
    // Measure output level
    updateMetering(buffer, false);
    
    // Handle denormals
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        juce::FloatVectorOperations::disableDenormalisedNumberSupport();
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            if (std::abs(data[i]) < 1e-10f)
                data[i] = 0.0f;
        }
    }
}

void SVTProcessor::setGain(float gainDb)
{
    preamp.setGain(gainDb);
}

void SVTProcessor::setPad(bool usePad)
{
    preamp.setPad(usePad);
}

void SVTProcessor::setBass(float value)
{
    toneStack.setBass(value);
}

void SVTProcessor::setMid(float value)
{
    toneStack.setMid(value);
}

void SVTProcessor::setTreble(float value)
{
    toneStack.setTreble(value);
}

void SVTProcessor::setMidFrequency(ToneStack::MidFrequency freq)
{
    toneStack.setMidFrequency(freq);
}

void SVTProcessor::setUltraLo(float value)
{
    toneStack.setUltraLo(value);
}

void SVTProcessor::setUltraHi(float value)
{
    toneStack.setUltraHi(value);
}

void SVTProcessor::setMaster(float level)
{
    powerAmp.setMaster(level);
}

void SVTProcessor::setCabEnabled(bool enabled)
{
    cabSim.setEnabled(enabled);
}

void SVTProcessor::setStandby(bool standby)
{
    isStandby = standby;
    if (!standby)
        standbyWarmupSamples = 0;
}

void SVTProcessor::updateMetering(const juce::AudioBuffer<float>& buffer, bool isInput)
{
    float maxLevel = 0.0f;
    bool clipping = false;
    
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        const float channelMax = buffer.getMagnitude(ch, 0, buffer.getNumSamples());
        maxLevel = std::max(maxLevel, channelMax);
        
        if (channelMax > 0.95f)
            clipping = true;
    }
    
    if (isInput)
    {
        inputLevel.store(maxLevel);
        inputClipping.store(clipping);
    }
    else
    {
        outputLevel.store(maxLevel);
        outputClipping.store(clipping);
    }
}
