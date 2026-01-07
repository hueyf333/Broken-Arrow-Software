#pragma once

#include <JuceHeader.h>
#include "../Model/Project.h"

namespace KnobSmith
{

class ProjectSerializer
{
public:
    static bool saveProject(const Project& project, const juce::File& file);
    static bool loadProject(Project& project, const juce::File& file);
    
    static juce::String getLastError() { return lastError; }

private:
    static juce::String lastError;
};

} // namespace KnobSmith
