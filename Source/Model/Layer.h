#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

enum class LayerType
{
    Background,
    BaseArt,
    IndicatorCap,
    GlowShadow,
    MarksTicks
};

class Layer
{
public:
    Layer(const juce::String& name, LayerType type, int width, int height);
    ~Layer() = default;

    const juce::String& getName() const { return name; }
    void setName(const juce::String& newName) { name = newName; }

    LayerType getType() const { return type; }
    
    bool isVisible() const { return visible; }
    void setVisible(bool shouldBeVisible) { visible = shouldBeVisible; }

    float getOpacity() const { return opacity; }
    void setOpacity(float newOpacity) { opacity = juce::jlimit(0.0f, 1.0f, newOpacity); }

    juce::Image& getImage() { return image; }
    const juce::Image& getImage() const { return image; }
    
    void setImage(const juce::Image& newImage);
    void clearImage();

    juce::AffineTransform getTransform() const { return transform; }
    void setTransform(const juce::AffineTransform& newTransform) { transform = newTransform; }

    juce::var toVar() const;
    static std::unique_ptr<Layer> fromVar(const juce::var& data);

private:
    juce::String name;
    LayerType type;
    bool visible{true};
    float opacity{1.0f};
    juce::Image image;
    juce::AffineTransform transform;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Layer)
};

} // namespace KnobSmith
