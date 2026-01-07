#pragma once

#include <JuceHeader.h>
#include "../Tools/Tool.h"

namespace KnobSmith
{

class ToolPanel : public juce::Component
{
public:
    ToolPanel();
    ~ToolPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    std::function<void(Tool::Type)> onToolSelected;

private:
    juce::TextButton selectButton{"Select"};
    juce::TextButton brushButton{"Brush"};
    juce::TextButton eraserButton{"Eraser"};
    juce::TextButton fillButton{"Fill"};
    juce::TextButton shapeButton{"Shape"};
    juce::TextButton pickerButton{"Picker"};

    void selectTool(Tool::Type type);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolPanel)
};

} // namespace KnobSmith
