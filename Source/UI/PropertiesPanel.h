#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class PropertiesPanel : public juce::Component
{
public:
    PropertiesPanel();
    ~PropertiesPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void showToolProperties(const juce::String& toolName);
    void showFilmstripSettings();
    void showExportSettings();

private:
    juce::Label titleLabel;
    juce::PropertyPanel propertyPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertiesPanel)
};

} // namespace KnobSmith
