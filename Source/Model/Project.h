#pragma once

#include <JuceHeader.h>
#include "Layer.h"
#include "FilmstripConfig.h"
#include "ExportSettings.h"

namespace KnobSmith
{

class Project
{
public:
    Project();
    ~Project() = default;

    void clear();
    
    // Canvas properties
    int getCanvasWidth() const { return canvasWidth; }
    int getCanvasHeight() const { return canvasHeight; }
    void setCanvasSize(int width, int height);

    juce::Colour getBackgroundColour() const { return backgroundColour; }
    void setBackgroundColour(juce::Colour colour) { backgroundColour = colour; }

    // Layer management
    int getLayerCount() const { return layers.size(); }
    Layer* getLayer(int index);
    const Layer* getLayer(int index) const;
    Layer* getLayerByName(const juce::String& name);
    
    void addLayer(std::unique_ptr<Layer> layer);
    void removeLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    
    std::vector<Layer*> getLayers();
    std::vector<const Layer*> getLayers() const;

    // Control type
    enum class ControlType
    {
        Knob,
        Slider,
        Button,
        Custom
    };

    ControlType getControlType() const { return controlType; }
    void setControlType(ControlType type) { controlType = type; }

    // Filmstrip configuration
    FilmstripConfig& getFilmstripConfig() { return filmstripConfig; }
    const FilmstripConfig& getFilmstripConfig() const { return filmstripConfig; }

    // Export settings
    ExportSettings& getExportSettings() { return exportSettings; }
    const ExportSettings& getExportSettings() const { return exportSettings; }

    // Project file management
    juce::File getProjectFile() const { return projectFile; }
    void setProjectFile(const juce::File& file) { projectFile = file; }
    
    bool hasUnsavedChanges() const { return unsavedChanges; }
    void setUnsavedChanges(bool hasChanges) { unsavedChanges = hasChanges; }

    // Serialization
    juce::var toVar() const;
    bool fromVar(const juce::var& data);

private:
    int canvasWidth{512};
    int canvasHeight{512};
    juce::Colour backgroundColour{juce::Colours::darkgrey};
    
    std::vector<std::unique_ptr<Layer>> layers;
    ControlType controlType{ControlType::Knob};
    
    FilmstripConfig filmstripConfig;
    ExportSettings exportSettings;
    
    juce::File projectFile;
    bool unsavedChanges{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Project)
};

} // namespace KnobSmith
