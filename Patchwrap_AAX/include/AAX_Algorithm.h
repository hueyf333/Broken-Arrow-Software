#pragma once

#include <cstdint>

namespace Patchwrap {

class VST3Host;
class ParameterBridge;

// AAX algorithm context - data passed between parameter and algorithm
struct AAX_AlgorithmContext {
    // Input/output buffers
    const float* const* inputBuffers;
    float* const* outputBuffers;
    
    // Buffer info
    int32_t numInputChannels;
    int32_t numOutputChannels;
    int32_t numFrames;
    
    // Parameters (accessed from private data)
    bool bypass;
    float outputGain;
    
    // Pointers to VST3 host and parameter bridge
    VST3Host* vst3Host;
    ParameterBridge* paramBridge;
    
    // MIDI input (placeholder)
    const void* midiInput;
};

// AAX algorithm callback - this is the real-time audio processing function
// Must be real-time safe (no allocations, no locks, no I/O)
void AAX_AlgorithmProcessCallback(
    AAX_AlgorithmContext* const inInstancesBegin[],
    const void* inInstancesEnd);

} // namespace Patchwrap
