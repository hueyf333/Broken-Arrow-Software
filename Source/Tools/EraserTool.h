#pragma once

#include "Tool.h"

namespace KnobSmith
{

class EraserTool : public Tool
{
public:
    EraserTool();
    ~EraserTool() override = default;

    void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) override;

    juce::Cursor getCursor() const override;

    void setEraserSize(float size) { eraserSize = juce::jmax(1.0f, size); }
    float getEraserSize() const { return eraserSize; }

private:
    float eraserSize{20.0f};
    juce::Point<float> lastPoint;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EraserTool)
};

} // namespace KnobSmith
