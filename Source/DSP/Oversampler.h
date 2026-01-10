#pragma once

#include <juce_dsp/juce_dsp.h>

class Oversampler
{
public:
    Oversampler();
    ~Oversampler() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    // Process a block with oversampling
    template<typename ProcessFunc>
    void process(juce::AudioBuffer<float>& buffer, ProcessFunc&& processFunction)
    {
        if (oversamplingFactor > 1 && oversampling != nullptr)
        {
            // Upsample
            juce::dsp::AudioBlock<float> block(buffer);
            auto oversampledBlock = oversampling->processSamplesUp(block);
            
            // Process at higher sample rate
            processFunction(oversampledBlock);
            
            // Downsample
            oversampling->processSamplesDown(block);
        }
        else
        {
            // No oversampling, process directly
            juce::dsp::AudioBlock<float> block(buffer);
            processFunction(block);
        }
    }
    
    void setOversamplingFactor(int factor);  // 1, 2, 4, or 8

private:
    int oversamplingFactor { 2 };
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;
};
