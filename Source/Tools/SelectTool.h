#pragma once

#include "Tool.h"

namespace KnobSmith
{

class SelectTool : public Tool
{
public:
    SelectTool();
    ~SelectTool() override = default;

    void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) override;

    juce::Cursor getCursor() const override;

    juce::Rectangle<int> getSelection() const { return selection; }
    void setSelection(juce::Rectangle<int> newSelection) { selection = newSelection; }

private:
    juce::Rectangle<int> selection;
    juce::Point<int> dragStart;
    bool isDragging{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectTool)
};

} // namespace KnobSmith
