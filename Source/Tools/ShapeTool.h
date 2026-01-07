#pragma once

#include "Tool.h"

namespace KnobSmith
{

class ShapeTool : public Tool
{
public:
    enum class ShapeType
    {
        Line,
        Rectangle,
        Ellipse
    };

    ShapeTool();
    ~ShapeTool() override = default;

    void mouseDown(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseDrag(const juce::MouseEvent& e, juce::Image& targetImage) override;
    void mouseUp(const juce::MouseEvent& e, juce::Image& targetImage) override;

    juce::Cursor getCursor() const override;

    void setShapeType(ShapeType type) { shapeType = type; }
    ShapeType getShapeType() const { return shapeType; }

    void setColour(juce::Colour colour) { this->colour = colour; }
    juce::Colour getColour() const { return colour; }

    void setStrokeWidth(float width) { strokeWidth = juce::jmax(1.0f, width); }
    float getStrokeWidth() const { return strokeWidth; }

    void setFilled(bool filled) { this->filled = filled; }
    bool isFilled() const { return filled; }

private:
    ShapeType shapeType{ShapeType::Rectangle};
    juce::Colour colour{juce::Colours::black};
    float strokeWidth{2.0f};
    bool filled{false};
    juce::Point<int> startPoint;
    juce::Point<int> endPoint;
    bool isDrawing{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeTool)
};

} // namespace KnobSmith
