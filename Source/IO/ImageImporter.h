#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class ImageImporter
{
public:
    static juce::Image importImage(const juce::File& file);
    static bool isFormatSupported(const juce::String& extension);
    static juce::String getLastError() { return lastError; }
    static juce::String getSupportedFormats();

private:
    static juce::Image convertToRGBA(const juce::Image& source);
    static juce::String lastError;
};

} // namespace KnobSmith
