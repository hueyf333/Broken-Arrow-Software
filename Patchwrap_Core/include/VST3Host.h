#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

namespace Steinberg {
namespace Vst {
    class IComponent;
    class IEditController;
    class IAudioProcessor;
}
}

namespace Patchwrap {

class ParameterBridge;

// VST3 plugin information
struct VST3PluginInfo {
    std::string path;
    std::string name;
    std::string vendor;
    std::string category;
    std::string version;
    uint32_t numInputs;
    uint32_t numOutputs;
    bool supportsMono;
    bool supportsStereo;
};

// Audio buffer format
struct AudioBuffer {
    float** channels;
    uint32_t numChannels;
    uint32_t numFrames;
};

// MIDI event
struct MidiEvent {
    uint32_t sampleOffset;
    uint8_t data[4];
    uint8_t size;
};

// VST3 host implementation for loading and running VST3 plugins
class VST3Host {
public:
    VST3Host();
    ~VST3Host();

    // Scan for VST3 plugins in standard locations
    std::vector<VST3PluginInfo> scanPlugins();

    // Load a VST3 plugin
    bool loadPlugin(const std::string& path);

    // Unload current plugin
    void unloadPlugin();

    // Check if plugin is loaded
    bool isPluginLoaded() const { return pluginLoaded; }

    // Get current plugin info
    const VST3PluginInfo& getPluginInfo() const { return pluginInfo; }

    // Setup processing
    bool setupProcessing(double sampleRate, uint32_t maxBlockSize, bool stereo);

    // Activate/deactivate processing
    bool setProcessing(bool active);

    // Process audio (realtime safe)
    void processAudio(AudioBuffer& input, AudioBuffer& output, 
                     const std::vector<MidiEvent>& midiEvents);

    // Get/set latency
    uint32_t getLatencySamples() const { return latencySamples; }

    // Parameter management
    uint32_t getNumParameters() const;
    std::string getParameterName(uint32_t index) const;
    std::string getParameterDisplay(uint32_t index, double normalizedValue) const;
    double getParameterValue(uint32_t index) const;
    void setParameterValue(uint32_t index, double normalizedValue);

    // State management
    bool saveState(std::vector<uint8_t>& data);
    bool loadState(const std::vector<uint8_t>& data);

    // Editor
    bool hasEditor() const;
    void* createEditor(void* parent);
    void closeEditor();
    void getEditorSize(uint32_t& width, uint32_t& height);
    void setEditorSize(uint32_t width, uint32_t height);

    // Connect parameter bridge for bidirectional updates
    void setParameterBridge(ParameterBridge* bridge);

private:
    bool pluginLoaded;
    VST3PluginInfo pluginInfo;
    uint32_t latencySamples;
    double currentSampleRate;
    uint32_t maxBlockSize;
    bool isStereo;
    bool processingActive;

    ParameterBridge* paramBridge;

    // VST3 SDK interfaces (opaque pointers to avoid header dependency)
    void* component;        // IComponent*
    void* controller;       // IEditController*
    void* audioProcessor;   // IAudioProcessor*
    void* plugView;         // IPlugView*

    // Private implementation details
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace Patchwrap
