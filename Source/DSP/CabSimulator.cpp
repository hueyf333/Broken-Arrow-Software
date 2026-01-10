#include "CabSimulator.h"

CabSimulator::CabSimulator()
{
}

void CabSimulator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    
    // Prepare convolution
    convolution.prepare(spec);
    
    // Create default IR or use filter network
    createDefaultIR();
    
    // Configure resonant filter network (bass cab characteristics)
    for (int ch = 0; ch < 2; ++ch)
    {
        // First resonance around 80 Hz (cab resonance)
        resonance1[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate, 80.0, 2.5, juce::Decibels::decibelsToGain(6.0f));
        
        // Second resonance around 200 Hz
        resonance2[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate, 200.0, 1.8, juce::Decibels::decibelsToGain(3.0f));
        
        // Low-pass for speaker roll-off
        lowPass[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 4500.0, 0.707);
        
        // High-pass to remove sub-bass
        highPass[ch].coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 50.0, 0.707);
    }
    
    reset();
}

void CabSimulator::reset()
{
    convolution.reset();
    
    for (int ch = 0; ch < 2; ++ch)
    {
        resonance1[ch].reset();
        resonance2[ch].reset();
        lowPass[ch].reset();
        highPass[ch].reset();
    }
}

void CabSimulator::createDefaultIR()
{
    // Create a simple bass cab IR using filtered impulse
    const int irLength = 2048;
    juce::AudioBuffer<float> ir(2, irLength);
    
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* channelData = ir.getWritePointer(ch);
        
        // Create impulse
        channelData[0] = 1.0f;
        
        // Add exponential decay with resonances
        for (int i = 1; i < irLength; ++i)
        {
            float t = static_cast<float>(i) / sampleRate;
            float decay = std::exp(-t * 200.0f); // Fast decay
            float resonance = std::sin(2.0f * juce::MathConstants<float>::pi * 80.0f * t) * 0.3f +
                            std::sin(2.0f * juce::MathConstants<float>::pi * 200.0f * t) * 0.2f;
            channelData[i] = decay * resonance * 0.1f;
        }
    }
    
    // Load into convolution
    convolution.loadImpulseResponse(std::move(ir), sampleRate, 
                                    juce::dsp::Convolution::Stereo::yes,
                                    juce::dsp::Convolution::Trim::yes,
                                    juce::dsp::Convolution::Normalise::yes);
    useConvolution = true;
}

float CabSimulator::processSample(float input, int channel)
{
    float dry = input;
    float wet = input;
    
    if (useConvolution)
    {
        // Process through convolution (this would need block processing in real implementation)
        // For single sample, we use the filter network
        wet = highPass[channel].processSample(wet);
        wet = resonance1[channel].processSample(wet);
        wet = resonance2[channel].processSample(wet);
        wet = lowPass[channel].processSample(wet);
    }
    else
    {
        // Use filter network
        wet = highPass[channel].processSample(wet);
        wet = resonance1[channel].processSample(wet);
        wet = resonance2[channel].processSample(wet);
        wet = lowPass[channel].processSample(wet);
    }
    
    // Mix dry and wet
    return dry * (1.0f - mix) + wet * mix;
}

void CabSimulator::setMix(float mixAmount)
{
    mix = juce::jlimit(0.0f, 1.0f, mixAmount);
}

void CabSimulator::loadImpulseResponse(const juce::File& irFile)
{
    if (!irFile.existsAsFile())
        return;
    
    // Load audio file
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(irFile));
    
    if (reader != nullptr)
    {
        juce::AudioBuffer<float> ir(static_cast<int>(reader->numChannels),
                                    static_cast<int>(reader->lengthInSamples));
        reader->read(&ir, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
        
        convolution.loadImpulseResponse(std::move(ir), reader->sampleRate,
                                       juce::dsp::Convolution::Stereo::yes,
                                       juce::dsp::Convolution::Trim::yes,
                                       juce::dsp::Convolution::Normalise::yes);
        useConvolution = true;
    }
}
