#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class ImageUtils
{
public:
    static juce::Image compositeImages(const std::vector<const juce::Image*>& images);
    static juce::Image rotateImage(const juce::Image& source, float angleDegrees, juce::Point<float> center);
    static juce::Image scaleImage(const juce::Image& source, float scaleFactor);
    static juce::Image cropImage(const juce::Image& source, juce::Rectangle<int> bounds);
    
    static void drawBrushStroke(juce::Image& target, juce::Point<float> from, juce::Point<float> to,
                               juce::Colour colour, float size, float hardness, float opacity);
    
    static void floodFill(juce::Image& target, juce::Point<int> position, juce::Colour fillColour);

private:
    static juce::Colour getPixelColour(const juce::Image& image, int x, int y);
    static void setPixelColour(juce::Image& image, int x, int y, juce::Colour colour);
};

} // namespace KnobSmith
