#include "PowerAmp.h"

PowerAmp::PowerAmp()
{
}

void PowerAmp::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    
    // Output filter simulates transformer and speaker interaction
    outputFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 7000.0);
    
    reset();
}

void PowerAmp::reset()
{
    outputFilter.reset();
    sagAmount = 0.0f;
}

float PowerAmp::applySag(float input)
{
    // Power supply sag simulation based on signal level
    float inputLevel = std::abs(input);
    float targetSag = inputLevel * drive * 0.2f; // Sag increases with drive and level
    
    float coeff = (targetSag > sagAmount) ? sagAttack : sagRelease;
    sagAmount += coeff * (targetSag - sagAmount);
    
    // Apply compression due to sag
    float sagReduction = 1.0f - (sagAmount * 0.3f);
    return input * sagReduction;
}

float PowerAmp::applySaturation(float input)
{
    // Power tube saturation using asymmetric clipping
    float driven = input * (1.0f + drive * 8.0f);
    
    // Soft clipping with asymmetric characteristic
    if (driven > 0.0f)
    {
        return std::tanh(driven * 1.2f) * 0.95f;
    }
    else
    {
        return std::tanh(driven * 1.5f) * 0.98f;
    }
}

float PowerAmp::processSample(float input)
{
    // Apply sag first (power supply compression)
    float output = applySag(input);
    
    // Power tube saturation
    output = applySaturation(output);
    
    // Output transformer filtering
    output = outputFilter.processSample(output);
    
    // Apply master volume
    output *= master;
    
    // Denormal protection
    if (std::abs(output) < 1e-15f)
        output = 0.0f;
    
    return output;
}

void PowerAmp::setDrive(float newDrive)
{
    drive = juce::jlimit(0.0f, 1.0f, newDrive);
}

void PowerAmp::setMaster(float newMaster)
{
    master = juce::jlimit(0.0f, 1.0f, newMaster);
}
