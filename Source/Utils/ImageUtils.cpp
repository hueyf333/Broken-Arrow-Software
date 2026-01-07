#include "ImageUtils.h"

namespace KnobSmith
{

juce::Image ImageUtils::compositeImages(const std::vector<const juce::Image*>& images)
{
    if (images.empty())
        return juce::Image();

    // Find the bounds needed
    int width = 0, height = 0;
    for (auto* img : images)
    {
        if (img && img->isValid())
        {
            width = juce::jmax(width, img->getWidth());
            height = juce::jmax(height, img->getHeight());
        }
    }

    if (width == 0 || height == 0)
        return juce::Image();

    juce::Image result(juce::Image::ARGB, width, height, true);
    juce::Graphics g(result);

    for (auto* img : images)
    {
        if (img && img->isValid())
        {
            g.drawImageAt(*img, 0, 0);
        }
    }

    return result;
}

juce::Image ImageUtils::rotateImage(const juce::Image& source, float angleDegrees, juce::Point<float> center)
{
    if (!source.isValid())
        return juce::Image();

    juce::Image result(source.getFormat(), source.getWidth(), source.getHeight(), true);
    juce::Graphics g(result);

    auto angleRadians = juce::degreesToRadians(angleDegrees);
    auto transform = juce::AffineTransform::rotation(angleRadians, 
                                                     center.x * source.getWidth(),
                                                     center.y * source.getHeight());

    g.drawImageTransformed(source, transform);
    return result;
}

juce::Image ImageUtils::scaleImage(const juce::Image& source, float scaleFactor)
{
    if (!source.isValid() || scaleFactor <= 0.0f)
        return juce::Image();

    int newWidth = static_cast<int>(source.getWidth() * scaleFactor);
    int newHeight = static_cast<int>(source.getHeight() * scaleFactor);

    juce::Image result(source.getFormat(), newWidth, newHeight, true);
    juce::Graphics g(result);
    g.drawImageTransformed(source, juce::AffineTransform::scale(scaleFactor));
    return result;
}

juce::Image ImageUtils::cropImage(const juce::Image& source, juce::Rectangle<int> bounds)
{
    if (!source.isValid())
        return juce::Image();

    bounds = bounds.getIntersection(source.getBounds());
    if (bounds.isEmpty())
        return juce::Image();

    return source.getClippedImage(bounds);
}

juce::Colour ImageUtils::getPixelColour(const juce::Image& image, int x, int y)
{
    if (!image.isValid() || !image.getBounds().contains(x, y))
        return juce::Colours::transparentBlack;

    return image.getPixelAt(x, y);
}

void ImageUtils::setPixelColour(juce::Image& image, int x, int y, juce::Colour colour)
{
    if (image.isValid() && image.getBounds().contains(x, y))
    {
        image.setPixelAt(x, y, colour);
    }
}

void ImageUtils::drawBrushStroke(juce::Image& target, juce::Point<float> from, juce::Point<float> to,
                                juce::Colour colour, float size, float hardness, float opacity)
{
    if (!target.isValid())
        return;

    juce::Graphics g(target);
    g.setColour(colour.withAlpha(opacity));

    // Simple brush implementation - draw line with round caps
    juce::Path path;
    path.startNewSubPath(from);
    path.lineTo(to);

    juce::PathStrokeType strokeType(size, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    g.strokePath(path, strokeType);
}

void ImageUtils::floodFill(juce::Image& target, juce::Point<int> position, juce::Colour fillColour)
{
    if (!target.isValid() || !target.getBounds().contains(position))
        return;

    auto targetColour = getPixelColour(target, position.x, position.y);
    
    if (targetColour == fillColour)
        return;

    // Simple flood fill using stack-based approach
    std::vector<juce::Point<int>> stack;
    stack.push_back(position);

    while (!stack.empty())
    {
        auto p = stack.back();
        stack.pop_back();

        if (!target.getBounds().contains(p))
            continue;

        if (getPixelColour(target, p.x, p.y) != targetColour)
            continue;

        setPixelColour(target, p.x, p.y, fillColour);

        stack.push_back({p.x + 1, p.y});
        stack.push_back({p.x - 1, p.y});
        stack.push_back({p.x, p.y + 1});
        stack.push_back({p.x, p.y - 1});
    }
}

} // namespace KnobSmith
