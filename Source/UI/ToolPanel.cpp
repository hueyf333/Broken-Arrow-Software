#include "ToolPanel.h"

namespace KnobSmith
{

ToolPanel::ToolPanel()
{
    addAndMakeVisible(selectButton);
    addAndMakeVisible(brushButton);
    addAndMakeVisible(eraserButton);
    addAndMakeVisible(fillButton);
    addAndMakeVisible(shapeButton);
    addAndMakeVisible(pickerButton);

    selectButton.onClick = [this] { selectTool(Tool::Type::Select); };
    brushButton.onClick = [this] { selectTool(Tool::Type::Brush); };
    eraserButton.onClick = [this] { selectTool(Tool::Type::Eraser); };
    fillButton.onClick = [this] { selectTool(Tool::Type::Fill); };
    shapeButton.onClick = [this] { selectTool(Tool::Type::Shape); };
    pickerButton.onClick = [this] { selectTool(Tool::Type::ColorPicker); };
}

void ToolPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff252526));
}

void ToolPanel::resized()
{
    auto bounds = getLocalBounds().reduced(4);
    int buttonHeight = 30;
    int spacing = 4;

    selectButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(spacing);
    brushButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(spacing);
    eraserButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(spacing);
    fillButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(spacing);
    shapeButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(spacing);
    pickerButton.setBounds(bounds.removeFromTop(buttonHeight));
}

void ToolPanel::selectTool(Tool::Type type)
{
    if (onToolSelected)
        onToolSelected(type);
}

} // namespace KnobSmith
