/*
  ==============================================================================

    Cabinet Simulator - IR loader with partitioned FFT convolution

  ==============================================================================
*/

#include "CabinetSimulator.h"

CabinetSimulator::CabinetSimulator()
{
}

void CabinetSimulator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    
    juce::dsp::ProcessSpec monoSpec = spec;
    monoSpec.numChannels = 1;
    
    convolutionL.prepare(monoSpec);
    convolutionR.prepare(monoSpec);
    
    // Load default IR
    loadFactoryIR(0);
    
    reset();
}

void CabinetSimulator::process(juce::dsp::AudioBlock<float>& block)
{
    if (block.getNumChannels() == 1)
    {
        // Mono processing
        convolutionL.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
    else
    {
        // Stereo processing
        auto leftBlock = block.getSingleChannelBlock(0);
        auto rightBlock = block.getSingleChannelBlock(1);
        
        convolutionL.process(juce::dsp::ProcessContextReplacing<float>(leftBlock));
        convolutionR.process(juce::dsp::ProcessContextReplacing<float>(rightBlock));
    }
}

void CabinetSimulator::reset()
{
    convolutionL.reset();
    convolutionR.reset();
}

void CabinetSimulator::setIRSelection(int irIndex)
{
    if (irIndex != currentIRIndex && irIndex >= 0 && irIndex < 3)
    {
        currentIRIndex = irIndex;
        loadFactoryIR(irIndex);
    }
}

void CabinetSimulator::loadUserIR(const juce::File& irFile)
{
    if (!irFile.existsAsFile())
        return;
    
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(irFile));
    
    if (reader != nullptr)
    {
        juce::AudioBuffer<float> irBuffer(static_cast<int>(reader->numChannels),
                                          static_cast<int>(reader->lengthInSamples));
        reader->read(&irBuffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
        
        // Load into convolution
        if (irBuffer.getNumChannels() == 1)
        {
            convolutionL.loadImpulseResponse(std::move(irBuffer), sampleRate, 
                                            juce::dsp::Convolution::Stereo::no,
                                            juce::dsp::Convolution::Trim::yes,
                                            juce::dsp::Convolution::Normalise::yes);
            convolutionR.loadImpulseResponse(irBuffer, sampleRate,
                                            juce::dsp::Convolution::Stereo::no,
                                            juce::dsp::Convolution::Trim::yes,
                                            juce::dsp::Convolution::Normalise::yes);
        }
        else
        {
            convolutionL.loadImpulseResponse(std::move(irBuffer), sampleRate,
                                            juce::dsp::Convolution::Stereo::yes,
                                            juce::dsp::Convolution::Trim::yes,
                                            juce::dsp::Convolution::Normalise::yes);
            convolutionR.copyStateFrom(convolutionL);
        }
    }
}

void CabinetSimulator::loadFactoryIR(int index)
{
    auto irBuffer = createFactoryIR(index, sampleRate);
    
    // Load the same IR for both channels
    juce::AudioBuffer<float> irBufferCopy(irBuffer);
    
    convolutionL.loadImpulseResponse(std::move(irBuffer), sampleRate,
                                    juce::dsp::Convolution::Stereo::no,
                                    juce::dsp::Convolution::Trim::yes,
                                    juce::dsp::Convolution::Normalise::yes);
    
    convolutionR.loadImpulseResponse(std::move(irBufferCopy), sampleRate,
                                    juce::dsp::Convolution::Stereo::no,
                                    juce::dsp::Convolution::Trim::yes,
                                    juce::dsp::Convolution::Normalise::yes);
}

juce::AudioBuffer<float> CabinetSimulator::createFactoryIR(int index, double sr)
{
    // Create synthetic cabinet IRs
    // In a real implementation, these would be loaded from embedded WAV files
    
    int irLength = static_cast<int>(sr * 0.1); // 100ms IR
    juce::AudioBuffer<float> ir(1, irLength);
    ir.clear();
    
    auto* irData = ir.getWritePointer(0);
    juce::Random random;
    
    switch (index)
    {
        case 0: // 4x12 Vintage - darker, more midrange
        {
            for (int i = 0; i < irLength; ++i)
            {
                float t = static_cast<float>(i) / static_cast<float>(sr);
                
                // Exponential decay
                float envelope = std::exp(-t * 30.0f);
                
                // Resonances at typical cabinet frequencies
                float resonance = std::sin(juce::MathConstants<float>::twoPi * 120.0f * t) * 0.3f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 240.0f * t) * 0.2f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 800.0f * t) * 0.15f;
                
                // Add some noise for realism
                float noise = (random.nextFloat() * 2.0f - 1.0f) * 0.05f;
                
                irData[i] = (resonance + noise) * envelope;
            }
            break;
        }
        
        case 1: // 4x12 Modern - brighter, tighter
        {
            for (int i = 0; i < irLength; ++i)
            {
                float t = static_cast<float>(i) / static_cast<float>(sr);
                
                // Faster decay (tighter)
                float envelope = std::exp(-t * 40.0f);
                
                // Higher frequency resonances
                float resonance = std::sin(juce::MathConstants<float>::twoPi * 180.0f * t) * 0.25f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 400.0f * t) * 0.2f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 1200.0f * t) * 0.2f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 3500.0f * t) * 0.1f;
                
                float noise = (random.nextFloat() * 2.0f - 1.0f) * 0.04f;
                
                irData[i] = (resonance + noise) * envelope;
            }
            break;
        }
        
        case 2: // 2x12 Combo - more open, less bass
        {
            for (int i = 0; i < irLength; ++i)
            {
                float t = static_cast<float>(i) / static_cast<float>(sr);
                
                // Medium decay
                float envelope = std::exp(-t * 35.0f);
                
                // More midrange-focused
                float resonance = std::sin(juce::MathConstants<float>::twoPi * 150.0f * t) * 0.2f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 650.0f * t) * 0.3f;
                resonance += std::sin(juce::MathConstants<float>::twoPi * 2000.0f * t) * 0.15f;
                
                float noise = (random.nextFloat() * 2.0f - 1.0f) * 0.06f;
                
                irData[i] = (resonance + noise) * envelope;
            }
            break;
        }
    }
    
    // Normalize IR
    float maxVal = ir.getMagnitude(0, 0, irLength);
    if (maxVal > 0.0f)
        ir.applyGain(1.0f / maxVal);
    
    return ir;
}
