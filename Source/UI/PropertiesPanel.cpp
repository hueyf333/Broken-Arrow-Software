#include "PropertiesPanel.h"

namespace KnobSmith
{

PropertiesPanel::PropertiesPanel()
{
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(propertyPanel);
    
    titleLabel.setText("Properties", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
}

void PropertiesPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff252526));
}

void PropertiesPanel::resized()
{
    auto bounds = getLocalBounds().reduced(4);
    titleLabel.setBounds(bounds.removeFromTop(30));
    propertyPanel.setBounds(bounds);
}

void PropertiesPanel::showToolProperties(const juce::String& toolName)
{
    titleLabel.setText("Tool: " + toolName, juce::dontSendNotification);
    propertyPanel.clear();
}

void PropertiesPanel::showFilmstripSettings()
{
    titleLabel.setText("Filmstrip Settings", juce::dontSendNotification);
    propertyPanel.clear();
}

void PropertiesPanel::showExportSettings()
{
    titleLabel.setText("Export Settings", juce::dontSendNotification);
    propertyPanel.clear();
}

} // namespace KnobSmith
