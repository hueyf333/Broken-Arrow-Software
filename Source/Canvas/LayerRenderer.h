#pragma once

#include <JuceHeader.h>
#include "../Model/Layer.h"

namespace KnobSmith
{

class LayerRenderer
{
public:
    static juce::Image renderLayers(const std::vector<const Layer*>& layers, 
                                   int width, int height,
                                   juce::Colour backgroundColour = juce::Colours::transparentBlack);
    
    static juce::Image renderLayer(const Layer& layer);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LayerRenderer)
    LayerRenderer() = delete;
};

} // namespace KnobSmith
