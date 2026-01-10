#include "Oversampler.h"

Oversampler::Oversampler()
{
}

void Oversampler::prepare(const juce::dsp::ProcessSpec& spec)
{
    if (oversamplingFactor > 1)
    {
        // Calculate oversampling order (0=2x, 1=4x, 2=8x)
        int order = 0;
        if (oversamplingFactor == 4) order = 1;
        else if (oversamplingFactor == 8) order = 2;
        
        oversampling = std::make_unique<juce::dsp::Oversampling<float>>(
            static_cast<int>(spec.numChannels),
            order,
            juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
            true  // use steep filter
        );
        
        oversampling->initProcessing(spec.maximumBlockSize);
    }
}

void Oversampler::reset()
{
    if (oversampling != nullptr)
        oversampling->reset();
}

void Oversampler::setOversamplingFactor(int factor)
{
    // Validate factor
    if (factor == 1 || factor == 2 || factor == 4 || factor == 8)
    {
        oversamplingFactor = factor;
    }
    else
    {
        oversamplingFactor = 2; // Default to 2x
    }
}
