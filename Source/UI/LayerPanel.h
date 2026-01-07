#pragma once

#include <JuceHeader.h>
#include "../Model/Layer.h"

namespace KnobSmith
{

class LayerPanel : public juce::Component,
                   private juce::ListBoxModel
{
public:
    LayerPanel();
    ~LayerPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setLayers(const std::vector<Layer*>& layers);
    void refresh();

    std::function<void(int)> onLayerSelected;
    std::function<void(int, bool)> onLayerVisibilityChanged;

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent& e) override;

private:
    juce::ListBox layerList;
    std::vector<Layer*> currentLayers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LayerPanel)
};

} // namespace KnobSmith
