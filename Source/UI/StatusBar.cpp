#include "StatusBar.h"

namespace KnobSmith
{

StatusBar::StatusBar()
{
    addAndMakeVisible(statusLabel);
    addAndMakeVisible(cursorLabel);
    addAndMakeVisible(zoomLabel);
    addAndMakeVisible(toolLabel);
    
    statusLabel.setText("Ready", juce::dontSendNotification);
    cursorLabel.setText("Cursor: 0, 0", juce::dontSendNotification);
    zoomLabel.setText("Zoom: 100%", juce::dontSendNotification);
    toolLabel.setText("Tool: None", juce::dontSendNotification);
}

void StatusBar::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2d2d2d));
    g.setColour(juce::Colour(0xff3e3e42));
    g.drawHorizontalLine(0, 0.0f, static_cast<float>(getWidth()));
}

void StatusBar::resized()
{
    auto bounds = getLocalBounds().reduced(4);
    statusLabel.setBounds(bounds.removeFromLeft(300));
    toolLabel.setBounds(bounds.removeFromRight(150));
    zoomLabel.setBounds(bounds.removeFromRight(150));
    cursorLabel.setBounds(bounds.removeFromRight(150));
}

void StatusBar::setStatus(const juce::String& text)
{
    statusLabel.setText(text, juce::dontSendNotification);
}

void StatusBar::setCursorPosition(juce::Point<int> position)
{
    cursorLabel.setText("Cursor: " + juce::String(position.x) + ", " + juce::String(position.y), 
                       juce::dontSendNotification);
}

void StatusBar::setZoom(float zoom)
{
    zoomLabel.setText("Zoom: " + juce::String(static_cast<int>(zoom * 100)) + "%", 
                     juce::dontSendNotification);
}

void StatusBar::setSelectedTool(const juce::String& toolName)
{
    toolLabel.setText("Tool: " + toolName, juce::dontSendNotification);
}

} // namespace KnobSmith
