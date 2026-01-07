#include "ImageExporter.h"
#include "../Canvas/FilmstripGenerator.h"

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
        const auto& exportSettings = project.getExportSettings();
        int scale = exportSettings.getScaleMultiplier();
        
        // Use FilmstripGenerator to create the filmstrip
        auto filmstrip = FilmstripGenerator::generateFilmstrip(project);
        
        if (!filmstrip.isValid())
        {
            lastError = "Failed to generate filmstrip";
            return false;
        }
        
        // Apply export settings (scale)
        if (scale > 1)
        {
            int newWidth = filmstrip.getWidth() * scale;
            int newHeight = filmstrip.getHeight() * scale;
            juce::Image scaledFilmstrip(juce::Image::ARGB, newWidth, newHeight, true);
            juce::Graphics g(scaledFilmstrip);
            g.drawImageTransformed(filmstrip, juce::AffineTransform::scale(static_cast<float>(scale)));
            filmstrip = scaledFilmstrip;
        }
        
        // Apply background setting
        if (!exportSettings.getTransparentBackground())
        {
            juce::Image withBackground(juce::Image::ARGB, filmstrip.getWidth(), filmstrip.getHeight(), true);
            withBackground.clear(withBackground.getBounds(), juce::Colours::white);
            juce::Graphics g(withBackground);
            g.drawImageAt(filmstrip, 0, 0);
            filmstrip = withBackground;
        }
        
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
