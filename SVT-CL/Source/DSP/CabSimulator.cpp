#include "CabSimulator.h"

CabSimulator::CabSimulator()
{
}

void CabSimulator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    convolution.prepare(spec);
    createImpulseResponse();
}

void CabSimulator::reset()
{
    convolution.reset();
}

void CabSimulator::process(juce::AudioBuffer<float>& buffer)
{
    if (!isEnabled)
        return;
        
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    convolution.process(context);
}

void CabSimulator::setEnabled(bool enabled)
{
    isEnabled = enabled;
}

void CabSimulator::createImpulseResponse()
{
    // Create a simple synthetic impulse response that simulates an 8x10 bass cabinet
    // In a production version, this would load an actual cabinet IR
    const int irLength = (int)(sampleRate * 0.05); // 50ms IR
    
    juce::AudioBuffer<float> impulse(1, irLength);
    impulse.clear();
    
    auto* impulseData = impulse.getWritePointer(0);
    
    // Initial transient
    impulseData[0] = 1.0f;
    
    // Create a decaying resonance that mimics cabinet response
    float decay = 0.0f;
    
    for (int i = 1; i < irLength; ++i)
    {
        // Mix of frequencies typical of 8x10 cab
        float t = (float)i / (float)sampleRate;
        decay = std::exp(-t * 25.0f);
        
        // Multiple resonant modes
        float freq1 = 80.0f * 2.0f * juce::MathConstants<float>::pi;   // Low fundamental
        float freq2 = 220.0f * 2.0f * juce::MathConstants<float>::pi;  // Low-mid resonance
        float freq3 = 600.0f * 2.0f * juce::MathConstants<float>::pi;  // Mid resonance
        
        float sample = decay * (
            0.5f * std::sin(freq1 * t) +
            0.3f * std::sin(freq2 * t) +
            0.2f * std::sin(freq3 * t)
        );
        
        // Add some early reflections
        if (i > irLength / 20 && i < irLength / 5)
        {
            sample += 0.3f * decay * std::sin(freq1 * t * 1.5f);
        }
        
        impulseData[i] = sample;
    }
    
    // Normalize
    float maxVal = impulse.getMagnitude(0, 0, irLength);
    if (maxVal > 0.0f)
        impulse.applyGain(0.7f / maxVal);
    
    // Load into convolution
    convolution.loadImpulseResponse(std::move(impulse), sampleRate, 
        juce::dsp::Convolution::Stereo::no, 
        juce::dsp::Convolution::Trim::no, 
        juce::dsp::Convolution::Normalise::no);
}
