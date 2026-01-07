#include "Project.h"

namespace KnobSmith
{

Project::Project()
{
    // Create default layers
    addLayer(std::make_unique<Layer>("Background", LayerType::Background, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Base Art", LayerType::BaseArt, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Indicator", LayerType::IndicatorCap, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Glow/Shadow", LayerType::GlowShadow, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Marks/Ticks", LayerType::MarksTicks, canvasWidth, canvasHeight));
}

void Project::clear()
{
    layers.clear();
    canvasWidth = 512;
    canvasHeight = 512;
    backgroundColour = juce::Colours::darkgrey;
    controlType = ControlType::Knob;
    filmstripConfig = FilmstripConfig();
    exportSettings = ExportSettings();
    projectFile = juce::File();
    unsavedChanges = false;
    
    // Recreate default layers
    addLayer(std::make_unique<Layer>("Background", LayerType::Background, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Base Art", LayerType::BaseArt, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Indicator", LayerType::IndicatorCap, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Glow/Shadow", LayerType::GlowShadow, canvasWidth, canvasHeight));
    addLayer(std::make_unique<Layer>("Marks/Ticks", LayerType::MarksTicks, canvasWidth, canvasHeight));
}

void Project::setCanvasSize(int width, int height)
{
    canvasWidth = juce::jmax(1, width);
    canvasHeight = juce::jmax(1, height);
    
    // Resize all layer images
    for (auto& layer : layers)
    {
        auto oldImage = layer->getImage();
        juce::Image newImage(juce::Image::ARGB, canvasWidth, canvasHeight, true);
        
        if (oldImage.isValid())
        {
            juce::Graphics g(newImage);
            g.drawImageAt(oldImage, 0, 0);
        }
        
        layer->setImage(newImage);
    }
}

Layer* Project::getLayer(int index)
{
    if (juce::isPositiveAndBelow(index, static_cast<int>(layers.size())))
        return layers[static_cast<size_t>(index)].get();
    return nullptr;
}

const Layer* Project::getLayer(int index) const
{
    if (juce::isPositiveAndBelow(index, static_cast<int>(layers.size())))
        return layers[static_cast<size_t>(index)].get();
    return nullptr;
}

Layer* Project::getLayerByName(const juce::String& name)
{
    for (auto& layer : layers)
    {
        if (layer->getName() == name)
            return layer.get();
    }
    return nullptr;
}

void Project::addLayer(std::unique_ptr<Layer> layer)
{
    if (layer)
    {
        layers.push_back(std::move(layer));
        unsavedChanges = true;
    }
}

void Project::removeLayer(int index)
{
    if (juce::isPositiveAndBelow(index, static_cast<int>(layers.size())))
    {
        layers.erase(layers.begin() + index);
        unsavedChanges = true;
    }
}

void Project::moveLayer(int fromIndex, int toIndex)
{
    if (juce::isPositiveAndBelow(fromIndex, static_cast<int>(layers.size())) &&
        juce::isPositiveAndBelow(toIndex, static_cast<int>(layers.size())) &&
        fromIndex != toIndex)
    {
        auto layer = std::move(layers[static_cast<size_t>(fromIndex)]);
        layers.erase(layers.begin() + fromIndex);
        layers.insert(layers.begin() + toIndex, std::move(layer));
        unsavedChanges = true;
    }
}

std::vector<Layer*> Project::getLayers()
{
    std::vector<Layer*> result;
    for (auto& layer : layers)
        result.push_back(layer.get());
    return result;
}

std::vector<const Layer*> Project::getLayers() const
{
    std::vector<const Layer*> result;
    for (auto& layer : layers)
        result.push_back(layer.get());
    return result;
}

juce::var Project::toVar() const
{
    auto* obj = new juce::DynamicObject();
    
    obj->setProperty("version", 1);
    obj->setProperty("canvasWidth", canvasWidth);
    obj->setProperty("canvasHeight", canvasHeight);
    obj->setProperty("backgroundColour", backgroundColour.toString());
    obj->setProperty("controlType", static_cast<int>(controlType));
    
    // Serialize layers
    juce::Array<juce::var> layersArray;
    for (const auto& layer : layers)
    {
        layersArray.add(layer->toVar());
    }
    obj->setProperty("layers", layersArray);
    
    // Serialize filmstrip config
    obj->setProperty("filmstripConfig", filmstripConfig.toVar());
    
    // Serialize export settings
    obj->setProperty("exportSettings", exportSettings.toVar());
    
    return juce::var(obj);
}

bool Project::fromVar(const juce::var& data)
{
    if (auto* obj = data.getDynamicObject())
    {
        canvasWidth = obj->getProperty("canvasWidth");
        canvasHeight = obj->getProperty("canvasHeight");
        backgroundColour = juce::Colour::fromString(obj->getProperty("backgroundColour").toString());
        controlType = static_cast<ControlType>(static_cast<int>(obj->getProperty("controlType")));
        
        // Deserialize layers
        layers.clear();
        if (auto* layersArray = obj->getProperty("layers").getArray())
        {
            for (const auto& layerVar : *layersArray)
            {
                if (auto layer = Layer::fromVar(layerVar))
                {
                    layers.push_back(std::move(layer));
                }
            }
        }
        
        // Deserialize filmstrip config
        filmstripConfig.fromVar(obj->getProperty("filmstripConfig"));
        
        // Deserialize export settings
        exportSettings.fromVar(obj->getProperty("exportSettings"));
        
        unsavedChanges = false;
        return true;
    }
    
    return false;
}

} // namespace KnobSmith
