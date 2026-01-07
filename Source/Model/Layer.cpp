#include "Layer.h"

namespace KnobSmith
{

Layer::Layer(const juce::String& name, LayerType type, int width, int height)
    : name(name), type(type), visible(true), opacity(1.0f)
{
    image = juce::Image(juce::Image::ARGB, width, height, true);
}

void Layer::setImage(const juce::Image& newImage)
{
    if (newImage.isValid())
    {
        image = newImage.createCopy();
    }
}

void Layer::clearImage()
{
    if (image.isValid())
    {
        image.clear(image.getBounds(), juce::Colours::transparentBlack);
    }
}

juce::var Layer::toVar() const
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("name", name);
    obj->setProperty("type", static_cast<int>(type));
    obj->setProperty("visible", visible);
    obj->setProperty("opacity", opacity);
    
    // Store transform as array
    juce::Array<juce::var> transformArray;
    transformArray.add(transform.mat00);
    transformArray.add(transform.mat01);
    transformArray.add(transform.mat02);
    transformArray.add(transform.mat10);
    transformArray.add(transform.mat11);
    transformArray.add(transform.mat12);
    obj->setProperty("transform", transformArray);
    
    // Encode image as base64 PNG
    if (image.isValid())
    {
        juce::MemoryOutputStream imageStream;
        juce::PNGImageFormat pngFormat;
        if (pngFormat.writeImageToStream(image, imageStream))
        {
            obj->setProperty("imageData", imageStream.getMemoryBlock().toBase64Encoding());
            obj->setProperty("imageWidth", image.getWidth());
            obj->setProperty("imageHeight", image.getHeight());
        }
    }
    
    return juce::var(obj);
}

std::unique_ptr<Layer> Layer::fromVar(const juce::var& data)
{
    if (auto* obj = data.getDynamicObject())
    {
        auto name = obj->getProperty("name").toString();
        auto type = static_cast<LayerType>(static_cast<int>(obj->getProperty("type")));
        int width = static_cast<int>(obj->getProperty("imageWidth"));
        int height = static_cast<int>(obj->getProperty("imageHeight"));
        
        auto layer = std::make_unique<Layer>(name, type, width > 0 ? width : 512, height > 0 ? height : 512);
        layer->setVisible(obj->getProperty("visible"));
        layer->setOpacity(obj->getProperty("opacity"));
        
        // Restore transform
        if (auto* transformArray = obj->getProperty("transform").getArray())
        {
            if (transformArray->size() == 6)
            {
                juce::AffineTransform t(
                    (*transformArray)[0], (*transformArray)[1], (*transformArray)[2],
                    (*transformArray)[3], (*transformArray)[4], (*transformArray)[5]
                );
                layer->setTransform(t);
            }
        }
        
        // Decode image from base64 PNG
        auto imageData = obj->getProperty("imageData").toString();
        if (imageData.isNotEmpty())
        {
            juce::MemoryBlock block;
            if (block.fromBase64Encoding(imageData))
            {
                juce::MemoryInputStream imageStream(block, false);
                juce::PNGImageFormat pngFormat;
                auto loadedImage = pngFormat.decodeImage(imageStream);
                if (loadedImage.isValid())
                {
                    layer->setImage(loadedImage);
                }
            }
        }
        
        return layer;
    }
    
    return nullptr;
}

} // namespace KnobSmith
