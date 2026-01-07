#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class ExportSettings
{
public:
    ExportSettings();
    ~ExportSettings() = default;

    enum class Scale
    {
        x1 = 1,
        x2 = 2,
        x4 = 4
    };

    Scale getScale() const { return scale; }
    void setScale(Scale newScale) { scale = newScale; }
    int getScaleMultiplier() const { return static_cast<int>(scale); }

    bool getTransparentBackground() const { return transparentBackground; }
    void setTransparentBackground(bool transparent) { transparentBackground = transparent; }

    bool getPremultiplyAlpha() const { return premultiplyAlpha; }
    void setPremultiplyAlpha(bool premultiply) { premultiplyAlpha = premultiply; }

    juce::var toVar() const;
    void fromVar(const juce::var& data);

private:
    Scale scale{Scale::x1};
    bool transparentBackground{true};
    bool premultiplyAlpha{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExportSettings)
};

} // namespace KnobSmith
