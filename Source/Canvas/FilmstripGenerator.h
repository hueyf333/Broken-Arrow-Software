#pragma once

#include <JuceHeader.h>
#include "../Model/Project.h"
#include "../Model/FilmstripConfig.h"

namespace KnobSmith
{

class FilmstripGenerator
{
public:
    static juce::Image generateFilmstrip(const Project& project);
    
    static juce::Image generateFrame(const Project& project, int frameIndex);

private:
    static float calculateRotationAngle(const FilmstripConfig& config, int frameIndex);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmstripGenerator)
    FilmstripGenerator() = delete;
};

} // namespace KnobSmith
