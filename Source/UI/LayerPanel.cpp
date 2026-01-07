#include "LayerPanel.h"

namespace KnobSmith
{

LayerPanel::LayerPanel()
{
    addAndMakeVisible(layerList);
    layerList.setModel(this);
    layerList.setRowHeight(30);
}

void LayerPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff252526));
}

void LayerPanel::resized()
{
    layerList.setBounds(getLocalBounds().reduced(4));
}

void LayerPanel::setLayers(const std::vector<Layer*>& layers)
{
    currentLayers = layers;
    layerList.updateContent();
    repaint();
}

void LayerPanel::refresh()
{
    layerList.updateContent();
    repaint();
}

int LayerPanel::getNumRows()
{
    return static_cast<int>(currentLayers.size());
}

void LayerPanel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (juce::isPositiveAndBelow(rowNumber, static_cast<int>(currentLayers.size())))
    {
        auto* layer = currentLayers[static_cast<size_t>(rowNumber)];
        
        if (rowIsSelected)
            g.fillAll(juce::Colour(0xff094771));
        
        g.setColour(juce::Colours::white);
        g.drawText(layer->getName(), 30, 0, width - 30, height, juce::Justification::centredLeft);
        
        // Draw visibility checkbox
        g.setColour(layer->isVisible() ? juce::Colours::green : juce::Colours::red);
        g.fillRect(5, height / 2 - 6, 12, 12);
    }
}

void LayerPanel::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
    if (e.x < 25 && onLayerVisibilityChanged)
    {
        if (juce::isPositiveAndBelow(row, static_cast<int>(currentLayers.size())))
        {
            auto* layer = currentLayers[static_cast<size_t>(row)];
            layer->setVisible(!layer->isVisible());
            onLayerVisibilityChanged(row, layer->isVisible());
            refresh();
        }
    }
    else if (onLayerSelected)
    {
        onLayerSelected(row);
    }
}

} // namespace KnobSmith
