#pragma once

#include <JuceHeader.h>
#include "../Model/Project.h"

namespace KnobSmith
{

class ImageExporter
{
public:
    // Export single composited image
    static bool exportPNG(const juce::Image& image, const juce::File& file, 
                         bool transparentBackground = true, bool premultiplyAlpha = false);
    
    // Export filmstrip from project
    static bool exportFilmstrip(const Project& project, const juce::File& file);
    
    // Export selected layer
    static bool exportLayer(const juce::Image& layerImage, const juce::File& file, 
                           bool transparentBackground = true);
    
    static juce::String getLastError() { return lastError; }

private:
    static juce::Image applyExportSettings(const juce::Image& source, 
                                          bool transparentBackground, 
                                          bool premultiplyAlpha,
                                          int scale);
    
    static juce::String lastError;
};

} // namespace KnobSmith
