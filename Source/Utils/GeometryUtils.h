#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class GeometryUtils
{
public:
    static juce::Rectangle<float> applyTransform(const juce::Rectangle<float>& rect, 
                                                 const juce::AffineTransform& transform);
    
    static juce::AffineTransform createRotationTransform(float angleDegrees, juce::Point<float> center);
    
    static juce::AffineTransform createScaleTransform(float scaleX, float scaleY, juce::Point<float> center);
    
    static juce::AffineTransform createTranslationTransform(float dx, float dy);
    
    static float getDistance(juce::Point<float> p1, juce::Point<float> p2);
    
    static float getAngle(juce::Point<float> from, juce::Point<float> to);
};

} // namespace KnobSmith
