#include "ImageExporter.h"

namespace KnobSmith
{

juce::String ImageExporter::lastError;

juce::Image ImageExporter::applyExportSettings(const juce::Image& source, 
                                              bool transparentBackground,
                                              bool premultiplyAlpha,
                                              int scale)
{
    if (!source.isValid())
        return juce::Image();

    int width = source.getWidth() * scale;
    int height = source.getHeight() * scale;
    
    juce::Image result(juce::Image::ARGB, width, height, true);
    
    if (!transparentBackground)
    {
        result.clear(result.getBounds(), juce::Colours::white);
    }
    
    juce::Graphics g(result);
    g.drawImageTransformed(source, juce::AffineTransform::scale(static_cast<float>(scale)));
    
    if (premultiplyAlpha)
    {
        result.multiplyAllAlphas(1.0f); // This premultiplies the alpha
    }
    
    return result;
}

bool ImageExporter::exportPNG(const juce::Image& image, const juce::File& file,
                             bool transparentBackground, bool premultiplyAlpha)
{
    lastError = juce::String();

    if (!image.isValid())
    {
        lastError = "Invalid image";
        return false;
    }

    try
    {
        auto exportImage = applyExportSettings(image, transparentBackground, premultiplyAlpha, 1);
        
        juce::FileOutputStream stream(file);
        if (!stream.openedOk())
        {
            lastError = "Failed to open file for writing: " + file.getFullPathName();
            return false;
        }

        juce::PNGImageFormat pngFormat;
        if (!pngFormat.writeImageToStream(exportImage, stream))
        {
            lastError = "Failed to write PNG data to file";
            return false;
        }

        return true;
    }
    catch (const std::exception& e)
    {
        lastError = "Exception while exporting: " + juce::String(e.what());
        return false;
    }
}

bool ImageExporter::exportFilmstrip(const Project& project, const juce::File& file)
{
    lastError = juce::String();

    try
    {
        const auto& config = project.getFilmstripConfig();
        const auto& exportSettings = project.getExportSettings();
        
        int frameWidth = config.getFrameWidth();
        int frameHeight = config.getFrameHeight();
        int frameCount = config.getFrameCount();
        int padding = config.getPadding();
        int scale = exportSettings.getScaleMultiplier();
        
        // Calculate filmstrip dimensions
        int stripWidth, stripHeight;
        if (config.getOrientation() == FilmstripOrientation::Vertical)
        {
            stripWidth = (frameWidth + padding) * scale;
            stripHeight = (frameHeight * frameCount + padding * (frameCount - 1)) * scale;
        }
        else
        {
            stripWidth = (frameWidth * frameCount + padding * (frameCount - 1)) * scale;
            stripHeight = (frameHeight + padding) * scale;
        }
        
        juce::Image filmstrip(juce::Image::ARGB, stripWidth, stripHeight, true);
        
        if (!exportSettings.getTransparentBackground())
        {
            filmstrip.clear(filmstrip.getBounds(), juce::Colours::white);
        }
        
        // This is a placeholder - actual filmstrip generation would be done by FilmstripGenerator
        // For now, just export an empty filmstrip structure
        
        juce::FileOutputStream stream(file);
        if (!stream.openedOk())
        {
            lastError = "Failed to open file for writing: " + file.getFullPathName();
            return false;
        }

        juce::PNGImageFormat pngFormat;
        if (!pngFormat.writeImageToStream(filmstrip, stream))
        {
            lastError = "Failed to write PNG data to file";
            return false;
        }

        return true;
    }
    catch (const std::exception& e)
    {
        lastError = "Exception while exporting filmstrip: " + juce::String(e.what());
        return false;
    }
}

bool ImageExporter::exportLayer(const juce::Image& layerImage, const juce::File& file,
                               bool transparentBackground)
{
    return exportPNG(layerImage, file, transparentBackground, false);
}

} // namespace KnobSmith
