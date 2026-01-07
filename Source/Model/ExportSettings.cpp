#include "ExportSettings.h"

namespace KnobSmith
{

ExportSettings::ExportSettings()
{
}

juce::var ExportSettings::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("scale", static_cast<int>(scale));
    obj->setProperty("transparentBackground", transparentBackground);
    obj->setProperty("premultiplyAlpha", premultiplyAlpha);
    return juce::var(obj);
}

void ExportSettings::fromVar(const juce::var& data)
{
    if (auto* obj = data.getDynamicObject())
    {
        scale = static_cast<Scale>(static_cast<int>(obj->getProperty("scale")));
        transparentBackground = obj->getProperty("transparentBackground");
        premultiplyAlpha = obj->getProperty("premultiplyAlpha");
    }
}

} // namespace KnobSmith
