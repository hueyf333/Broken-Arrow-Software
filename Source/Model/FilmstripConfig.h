#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

enum class FilmstripOrientation
{
    Vertical,
    Horizontal
};

class FilmstripConfig
{
public:
    FilmstripConfig();
    ~FilmstripConfig() = default;

    FilmstripOrientation getOrientation() const { return orientation; }
    void setOrientation(FilmstripOrientation newOrientation) { orientation = newOrientation; }

    int getFrameCount() const { return frameCount; }
    void setFrameCount(int count) { frameCount = juce::jmax(1, count); }

    int getFrameWidth() const { return frameWidth; }
    void setFrameWidth(int width) { frameWidth = juce::jmax(1, width); }

    int getFrameHeight() const { return frameHeight; }
    void setFrameHeight(int height) { frameHeight = juce::jmax(1, height); }

    int getPadding() const { return padding; }
    void setPadding(int newPadding) { padding = juce::jmax(0, newPadding); }

    int getStartFrame() const { return startFrame; }
    void setStartFrame(int frame) { startFrame = juce::jmax(0, frame); }

    int getEndFrame() const { return endFrame; }
    void setEndFrame(int frame) { endFrame = juce::jmax(0, frame); }

    float getStartAngle() const { return startAngle; }
    void setStartAngle(float angle) { startAngle = angle; }

    float getEndAngle() const { return endAngle; }
    void setEndAngle(float angle) { endAngle = angle; }

    juce::Point<float> getRotationCenter() const { return rotationCenter; }
    void setRotationCenter(juce::Point<float> center) { rotationCenter = center; }

    juce::var toVar() const;
    void fromVar(const juce::var& data);

private:
    FilmstripOrientation orientation{FilmstripOrientation::Vertical};
    int frameCount{64};
    int frameWidth{128};
    int frameHeight{128};
    int padding{0};
    int startFrame{0};
    int endFrame{63};
    float startAngle{-135.0f};
    float endAngle{135.0f};
    juce::Point<float> rotationCenter{0.5f, 0.5f}; // normalized 0-1

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmstripConfig)
};

} // namespace KnobSmith
