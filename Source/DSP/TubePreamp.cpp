#include "TubePreamp.h"

TubePreamp::TubePreamp()
{
}

void TubePreamp::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    
    // Configure low-pass filters for tube-like frequency response
    // Each stage rolls off progressively
    stage1LowPass.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 8000.0);
    stage2LowPass.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 6000.0);
    stage3LowPass.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 5000.0);
    
    reset();
}

void TubePreamp::reset()
{
    stage1LowPass.reset();
    stage2LowPass.reset();
    stage3LowPass.reset();
}

float TubePreamp::applyBiasing(float input)
{
    // Add bias offset for asymmetric clipping
    return input + (bias * 0.3f);
}

float TubePreamp::applyAsymmetricClipping(float input)
{
    // Asymmetric soft clipping using tanh with different curves for positive and negative
    float biased = applyBiasing(input);
    
    if (biased > 0.0f)
    {
        // Positive half - softer clipping
        return std::tanh(biased * (1.0f + drive * 2.0f)) * 0.9f;
    }
    else
    {
        // Negative half - harder clipping for asymmetry
        return std::tanh(biased * (1.0f + drive * 3.0f)) * 0.95f;
    }
}

float TubePreamp::processSample(float input)
{
    // Stage 1: Input gain and first clipping
    float stage1 = input * (1.0f + drive * 4.0f);
    stage1 = applyAsymmetricClipping(stage1 * 0.5f);
    stage1 = stage1LowPass.processSample(stage1);
    
    // Stage 2: Second gain and clipping with different characteristics
    float stage2 = stage1 * (1.0f + drive * 2.0f);
    stage2 = std::tanh(stage2);
    stage2 = stage2LowPass.processSample(stage2);
    
    // Stage 3: Final tube stage with subtle saturation
    float stage3 = stage2 * (1.0f + drive * 1.5f);
    stage3 = applyAsymmetricClipping(stage3 * 0.7f);
    stage3 = stage3LowPass.processSample(stage3);
    
    // Output with level compensation
    float output = stage3 * (1.0f / (1.0f + drive * 0.5f));
    
    // Denormal protection
    if (std::abs(output) < denormalThreshold)
        output = 0.0f;
    
    return output;
}

void TubePreamp::setDrive(float newDrive)
{
    drive = juce::jlimit(0.0f, 1.0f, newDrive);
}

void TubePreamp::setBias(float newBias)
{
    bias = juce::jlimit(-1.0f, 1.0f, newBias);
}
