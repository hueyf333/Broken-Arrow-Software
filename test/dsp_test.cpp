#include "../SVT-CL/Source/DSP/SVTProcessor.h"
#include <iostream>
#include <cmath>

// Simple test to verify DSP functionality
int main()
{
    std::cout << "=== SVT-CL DSP Test ===" << std::endl;
    
    SVTProcessor processor;
    
    // Test at different sample rates
    double sampleRates[] = {44100.0, 48000.0, 96000.0};
    
    for (double sr : sampleRates)
    {
        std::cout << "\nTesting at " << sr << " Hz..." << std::endl;
        
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sr;
        spec.maximumBlockSize = 512;
        spec.numChannels = 2;
        
        processor.prepare(spec);
        processor.reset();
        
        // Set some parameters
        processor.setGain(20.0f);
        processor.setBass(0.7f);
        processor.setMid(0.6f);
        processor.setTreble(0.5f);
        processor.setMaster(0.0f);
        
        // Create a test buffer with a sine wave
        juce::AudioBuffer<float> buffer(2, 512);
        
        for (int ch = 0; ch < 2; ++ch)
        {
            auto* data = buffer.getWritePointer(ch);
            for (int i = 0; i < 512; ++i)
            {
                // 100 Hz sine wave
                data[i] = 0.5f * std::sin(2.0f * juce::MathConstants<float>::pi * 100.0f * i / sr);
            }
        }
        
        // Process the buffer
        processor.process(buffer);
        
        // Check output isn't silent or NaN
        float maxLevel = buffer.getMagnitude(0, 0, 512);
        bool hasNaN = false;
        
        for (int ch = 0; ch < 2; ++ch)
        {
            auto* data = buffer.getReadPointer(ch);
            for (int i = 0; i < 512; ++i)
            {
                if (std::isnan(data[i]) || std::isinf(data[i]))
                {
                    hasNaN = true;
                    break;
                }
            }
        }
        
        std::cout << "  Max level: " << maxLevel << std::endl;
        std::cout << "  Input level: " << processor.getInputLevel() << std::endl;
        std::cout << "  Output level: " << processor.getOutputLevel() << std::endl;
        
        if (maxLevel > 0.0f && !hasNaN)
        {
            std::cout << "  ✓ Processing successful" << std::endl;
        }
        else
        {
            std::cout << "  ✗ Processing failed!" << std::endl;
            return 1;
        }
    }
    
    std::cout << "\n=== All DSP tests passed! ===" << std::endl;
    return 0;
}
