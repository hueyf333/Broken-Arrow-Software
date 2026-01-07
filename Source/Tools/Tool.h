#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class Tool
{
public:
    enum class Type
    {
        Select,
        Brush,
        Eraser,
        Fill,
        Shape,
        ColorPicker
    };

    Tool(Type type) : toolType(type) {}
    virtual ~Tool() = default;

    Type getType() const { return toolType; }
    
    virtual void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) = 0;
    virtual void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) = 0;
    virtual void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) = 0;
    
    virtual juce::Cursor getCursor() const { return juce::MouseCursor::NormalCursor; }

protected:
    Type toolType;
};

} // namespace KnobSmith
