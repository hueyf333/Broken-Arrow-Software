#include "ImageImporter.h"

namespace KnobSmith
{

juce::String ImageImporter::lastError;

juce::String ImageImporter::getSupportedFormats()
{
    return "PNG, JPG, JPEG, BMP, GIF (*.png;*.jpg;*.jpeg;*.bmp;*.gif)";
}

bool ImageImporter::isFormatSupported(const juce::String& extension)
{
    auto ext = extension.toLowerCase();
    return ext == ".png" || ext == ".jpg" || ext == ".jpeg" || 
           ext == ".bmp" || ext == ".gif" || ext == ".tiff" || ext == ".webp";
}

juce::Image ImageImporter::convertToRGBA(const juce::Image& source)
{
    if (!source.isValid())
        return juce::Image();

    if (source.getFormat() == juce::Image::ARGB)
        return source.createCopy();

    juce::Image rgba(juce::Image::ARGB, source.getWidth(), source.getHeight(), true);
    juce::Graphics g(rgba);
    g.drawImageAt(source, 0, 0);
    return rgba;
}

juce::Image ImageImporter::importImage(const juce::File& file)
{
    lastError = juce::String();

    if (!file.existsAsFile())
    {
        lastError = "File does not exist: " + file.getFullPathName();
        return juce::Image();
    }

    auto extension = file.getFileExtension().toLowerCase();
    
    if (!isFormatSupported(extension))
    {
        lastError = "Unsupported file format: " + extension;
        return juce::Image();
    }

    try
    {
        juce::Image loadedImage;
        
        if (extension == ".png")
        {
            juce::PNGImageFormat pngFormat;
            juce::FileInputStream stream(file);
            if (stream.openedOk())
                loadedImage = pngFormat.decodeImage(stream);
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            juce::JPEGImageFormat jpegFormat;
            juce::FileInputStream stream(file);
            if (stream.openedOk())
                loadedImage = jpegFormat.decodeImage(stream);
        }
        else if (extension == ".gif")
        {
            juce::GIFImageFormat gifFormat;
            juce::FileInputStream stream(file);
            if (stream.openedOk())
                loadedImage = gifFormat.decodeImage(stream);
        }
        else
        {
            // For BMP, TIFF, WEBP - try using JUCE's generic image loading
            loadedImage = juce::ImageFileFormat::loadFrom(file);
            
            if (!loadedImage.isValid())
            {
                lastError = "Format " + extension + " codec not available. Only PNG, JPG, and GIF are guaranteed to be supported.";
                return juce::Image();
            }
        }

        if (!loadedImage.isValid())
        {
            lastError = "Failed to decode image from file";
            return juce::Image();
        }

        // Convert to RGBA format
        auto rgbaImage = convertToRGBA(loadedImage);
        
        if (!rgbaImage.isValid())
        {
            lastError = "Failed to convert image to RGBA format";
            return juce::Image();
        }

        return rgbaImage;
    }
    catch (const std::exception& e)
    {
        lastError = "Exception while importing image: " + juce::String(e.what());
        return juce::Image();
    }
}

} // namespace KnobSmith
