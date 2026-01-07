#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class StatusBar : public juce::Component
{
public:
    StatusBar();
    ~StatusBar() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setStatus(const juce::String& text);
    void setCursorPosition(juce::Point<int> position);
    void setZoom(float zoom);
    void setSelectedTool(const juce::String& toolName);

private:
    juce::Label statusLabel;
    juce::Label cursorLabel;
    juce::Label zoomLabel;
    juce::Label toolLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBar)
};

} // namespace KnobSmith
