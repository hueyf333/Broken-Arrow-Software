#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class UndoableCommand : public juce::UndoableAction
{
public:
    UndoableCommand() = default;
    ~UndoableCommand() override = default;

    bool perform() override = 0;
    bool undo() override = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UndoableCommand)
};

// Example command for paint strokes
class PaintCommand : public UndoableCommand
{
public:
    PaintCommand(juce::Image& targetImage, const juce::Image& beforeImage, const juce::Image& afterImage)
        : target(targetImage), before(beforeImage), after(afterImage)
    {
    }

    bool perform() override
    {
        target = after.createCopy();
        return true;
    }

    bool undo() override
    {
        target = before.createCopy();
        return true;
    }

private:
    juce::Image& target;
    juce::Image before;
    juce::Image after;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PaintCommand)
};

} // namespace KnobSmith
