#include "GeometryUtils.h"

namespace KnobSmith
{

juce::Rectangle<float> GeometryUtils::applyTransform(const juce::Rectangle<float>& rect,
                                                     const juce::AffineTransform& transform)
{
    auto topLeft = transform.transformPoint(rect.getTopLeft());
    auto topRight = transform.transformPoint(rect.getTopRight());
    auto bottomLeft = transform.transformPoint(rect.getBottomLeft());
    auto bottomRight = transform.transformPoint(rect.getBottomRight());

    auto minX = juce::jmin(topLeft.x, topRight.x, bottomLeft.x, bottomRight.x);
    auto maxX = juce::jmax(topLeft.x, topRight.x, bottomLeft.x, bottomRight.x);
    auto minY = juce::jmin(topLeft.y, topRight.y, bottomLeft.y, bottomRight.y);
    auto maxY = juce::jmax(topLeft.y, topRight.y, bottomLeft.y, bottomRight.y);

    return juce::Rectangle<float>(minX, minY, maxX - minX, maxY - minY);
}

juce::AffineTransform GeometryUtils::createRotationTransform(float angleDegrees, juce::Point<float> center)
{
    auto angleRadians = juce::degreesToRadians(angleDegrees);
    return juce::AffineTransform::rotation(angleRadians, center.x, center.y);
}

juce::AffineTransform GeometryUtils::createScaleTransform(float scaleX, float scaleY, juce::Point<float> center)
{
    return juce::AffineTransform::translation(-center.x, -center.y)
           .scaled(scaleX, scaleY)
           .translated(center.x, center.y);
}

juce::AffineTransform GeometryUtils::createTranslationTransform(float dx, float dy)
{
    return juce::AffineTransform::translation(dx, dy);
}

float GeometryUtils::getDistance(juce::Point<float> p1, juce::Point<float> p2)
{
    return p1.getDistanceFrom(p2);
}

float GeometryUtils::getAngle(juce::Point<float> from, juce::Point<float> to)
{
    auto dx = to.x - from.x;
    auto dy = to.y - from.y;
    return juce::radiansToDegrees(std::atan2(dy, dx));
}

} // namespace KnobSmith
