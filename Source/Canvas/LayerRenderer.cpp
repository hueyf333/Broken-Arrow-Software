#include "LayerRenderer.h"

namespace KnobSmith
{

juce::Image LayerRenderer::renderLayers(const std::vector<const Layer*>& layers,
                                       int width, int height,
                                       juce::Colour backgroundColour)
{
    juce::Image result(juce::Image::ARGB, width, height, true);
    
    if (!backgroundColour.isTransparent())
    {
        result.clear(result.getBounds(), backgroundColour);
    }
    
    juce::Graphics g(result);
    
    for (auto* layer : layers)
    {
        if (layer && layer->isVisible() && layer->getImage().isValid())
        {
            g.setOpacity(layer->getOpacity());
            g.drawImageTransformed(layer->getImage(), layer->getTransform());
        }
    }
    
    return result;
}

juce::Image LayerRenderer::renderLayer(const Layer& layer)
{
    if (!layer.getImage().isValid())
        return juce::Image();
    
    juce::Image result = layer.getImage().createCopy();
    
    // Apply opacity if not 1.0
    if (layer.getOpacity() < 1.0f)
    {
        result.multiplyAllAlphas(layer.getOpacity());
    }
    
    return result;
}

} // namespace KnobSmith
