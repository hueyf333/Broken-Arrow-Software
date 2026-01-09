#include "AAX_Algorithm.h"
#include "VST3Host.h"
#include "ParameterBridge.h"
#include <cmath>
#include <cstring>
#include <algorithm>

namespace Patchwrap {

// This is the real-time audio processing callback
// MUST be real-time safe: no allocations, no locks, no I/O
void AAX_AlgorithmProcessCallback(
    AAX_AlgorithmContext* const inInstancesBegin[],
    const void* inInstancesEnd)
{
    // Get algorithm context
    AAX_AlgorithmContext* context = inInstancesBegin[0];
    
    if (!context) return;
    
    const int32_t numFrames = context->numFrames;
    const int32_t numInputChannels = context->numInputChannels;
    const int32_t numOutputChannels = context->numOutputChannels;
    
    // Check bypass
    if (context->bypass) {
        // Bypass: copy input to output
        int32_t channelsToCopy = std::min(numInputChannels, numOutputChannels);
        for (int32_t ch = 0; ch < channelsToCopy; ++ch) {
            memcpy(context->outputBuffers[ch], 
                   context->inputBuffers[ch],
                   numFrames * sizeof(float));
        }
        
        // Zero any extra output channels
        for (int32_t ch = channelsToCopy; ch < numOutputChannels; ++ch) {
            memset(context->outputBuffers[ch], 0, numFrames * sizeof(float));
        }
        return;
    }
    
    // Process through VST3 plugin if loaded
    if (context->vst3Host && context->vst3Host->isPluginLoaded()) {
        // Prepare audio buffers for VST3
        AudioBuffer inputBuffer;
        inputBuffer.channels = const_cast<float**>(context->inputBuffers);
        inputBuffer.numChannels = static_cast<uint32_t>(numInputChannels);
        inputBuffer.numFrames = static_cast<uint32_t>(numFrames);
        
        AudioBuffer outputBuffer;
        outputBuffer.channels = context->outputBuffers;
        outputBuffer.numChannels = static_cast<uint32_t>(numOutputChannels);
        outputBuffer.numFrames = static_cast<uint32_t>(numFrames);
        
        // TODO: Convert MIDI input
        std::vector<MidiEvent> midiEvents;
        
        // Process audio
        context->vst3Host->processAudio(inputBuffer, outputBuffer, midiEvents);
    }
    else {
        // No plugin: copy input to output (pass-through)
        int32_t channelsToCopy = std::min(numInputChannels, numOutputChannels);
        for (int32_t ch = 0; ch < channelsToCopy; ++ch) {
            memcpy(context->outputBuffers[ch],
                   context->inputBuffers[ch],
                   numFrames * sizeof(float));
        }
    }
    
    // Apply output gain (pre-computed linear gain should be stored in context in production)
    // TODO: For production, compute linearGain = powf(10.0f, gainDB / 20.0f) when gain changes
    // and store it in context to avoid expensive powf() call in audio thread
    if (context->outputGain != 0.0f) {
        float linearGain = powf(10.0f, context->outputGain / 20.0f);
        
        for (int32_t ch = 0; ch < numOutputChannels; ++ch) {
            float* buffer = context->outputBuffers[ch];
            for (int32_t i = 0; i < numFrames; ++i) {
                buffer[i] *= linearGain;
            }
        }
    }
}

} // namespace Patchwrap
