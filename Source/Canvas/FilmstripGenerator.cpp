#include "FilmstripGenerator.h"
#include "LayerRenderer.h"
#include "../Utils/ImageUtils.h"

namespace KnobSmith
{

float FilmstripGenerator::calculateRotationAngle(const FilmstripConfig& config, int frameIndex)
{
    float t = static_cast<float>(frameIndex) / static_cast<float>(config.getFrameCount() - 1);
    return config.getStartAngle() + t * (config.getEndAngle() - config.getStartAngle());
}

juce::Image FilmstripGenerator::generateFrame(const Project& project, int frameIndex)
{
    const auto& config = project.getFilmstripConfig();
    auto layers = project.getLayers();
    
    int width = config.getFrameWidth();
    int height = config.getFrameHeight();
    
    juce::Image frame(juce::Image::ARGB, width, height, true);
    juce::Graphics g(frame);
    
    // Render layers
    for (auto* layer : layers)
    {
        if (layer && layer->isVisible() && layer->getImage().isValid())
        {
            g.setOpacity(layer->getOpacity());
            
            // If this is the indicator layer, rotate it based on frame index
            if (layer->getType() == LayerType::IndicatorCap && frameIndex >= 0)
            {
                float angle = calculateRotationAngle(config, frameIndex);
                auto rotated = ImageUtils::rotateImage(layer->getImage(), angle, config.getRotationCenter());
                g.drawImageAt(rotated, 0, 0);
            }
            else
            {
                g.drawImageTransformed(layer->getImage(), layer->getTransform());
            }
        }
    }
    
    return frame;
}

juce::Image FilmstripGenerator::generateFilmstrip(const Project& project)
{
    const auto& config = project.getFilmstripConfig();
    
    int frameWidth = config.getFrameWidth();
    int frameHeight = config.getFrameHeight();
    int frameCount = config.getFrameCount();
    int padding = config.getPadding();
    
    int stripWidth, stripHeight;
    
    if (config.getOrientation() == FilmstripOrientation::Vertical)
    {
        stripWidth = frameWidth;
        stripHeight = frameHeight * frameCount + padding * (frameCount - 1);
    }
    else
    {
        stripWidth = frameWidth * frameCount + padding * (frameCount - 1);
        stripHeight = frameHeight;
    }
    
    juce::Image filmstrip(juce::Image::ARGB, stripWidth, stripHeight, true);
    juce::Graphics g(filmstrip);
    
    // Generate and composite each frame
    for (int i = 0; i < frameCount; ++i)
    {
        auto frame = generateFrame(project, i);
        
        int x = 0, y = 0;
        if (config.getOrientation() == FilmstripOrientation::Vertical)
        {
            y = i * (frameHeight + padding);
        }
        else
        {
            x = i * (frameWidth + padding);
        }
        
        g.drawImageAt(frame, x, y);
    }
    
    return filmstrip;
}

} // namespace KnobSmith
