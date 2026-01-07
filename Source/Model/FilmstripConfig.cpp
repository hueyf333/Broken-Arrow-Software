#include "FilmstripConfig.h"

namespace KnobSmith
{

FilmstripConfig::FilmstripConfig()
{
}

juce::var FilmstripConfig::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("orientation", static_cast<int>(orientation));
    obj->setProperty("frameCount", frameCount);
    obj->setProperty("frameWidth", frameWidth);
    obj->setProperty("frameHeight", frameHeight);
    obj->setProperty("padding", padding);
    obj->setProperty("startFrame", startFrame);
    obj->setProperty("endFrame", endFrame);
    obj->setProperty("startAngle", startAngle);
    obj->setProperty("endAngle", endAngle);
    obj->setProperty("rotationCenterX", rotationCenter.x);
    obj->setProperty("rotationCenterY", rotationCenter.y);
    return juce::var(obj);
}

void FilmstripConfig::fromVar(const juce::var& data)
{
    if (auto* obj = data.getDynamicObject())
    {
        orientation = static_cast<FilmstripOrientation>(static_cast<int>(obj->getProperty("orientation")));
        frameCount = obj->getProperty("frameCount");
        frameWidth = obj->getProperty("frameWidth");
        frameHeight = obj->getProperty("frameHeight");
        padding = obj->getProperty("padding");
        startFrame = obj->getProperty("startFrame");
        endFrame = obj->getProperty("endFrame");
        startAngle = obj->getProperty("startAngle");
        endAngle = obj->getProperty("endAngle");
        rotationCenter.x = obj->getProperty("rotationCenterX");
        rotationCenter.y = obj->getProperty("rotationCenterY");
    }
}

} // namespace KnobSmith
