#pragma once

#include <JuceHeader.h>

/**
 * Oversampling Engine
 * Provides 2x oversampling for nonlinear processing stages
 * to reduce aliasing artifacts
 */
class OversamplingEngine
{
public:
    OversamplingEngine() = default;
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        // Create 2x oversampler
        oversampler = std::make_unique<juce::dsp::Oversampling<float>> (
            static_cast<int> (spec.numChannels), 1, // 1 = 2x oversampling
            juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
            true, // use steep filter
            false); // don't normalize
        
        oversampler->initProcessing (spec.maximumBlockSize);
    }
    
    void reset()
    {
        if (oversampler)
            oversampler->reset();
    }
    
    template<typename ProcessFunction>
    void processWithOversampling (juce::dsp::ProcessContextReplacing<float>& context,
                                   ProcessFunction&& processFunc)
    {
        auto& inputBlock = context.getOutputBlock();
        
        // Upsample
        auto oversampledBlock = oversampler->processSamplesUp (inputBlock);
        
        // Process at higher sample rate
        juce::dsp::ProcessContextReplacing<float> oversampledContext (oversampledBlock);
        processFunc (oversampledContext);
        
        // Downsample
        oversampler->processSamplesDown (inputBlock);
    }
    
private:
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
};
