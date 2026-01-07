#include "ProjectSerializer.h"

namespace KnobSmith
{

juce::String ProjectSerializer::lastError;

bool ProjectSerializer::saveProject(const Project& project, const juce::File& file)
{
    try
    {
        auto projectData = project.toVar();
        auto json = juce::JSON::toString(projectData, true);
        
        if (file.replaceWithText(json))
        {
            lastError = juce::String();
            return true;
        }
        else
        {
            lastError = "Failed to write file: " + file.getFullPathName();
            return false;
        }
    }
    catch (const std::exception& e)
    {
        lastError = "Exception while saving: " + juce::String(e.what());
        return false;
    }
}

bool ProjectSerializer::loadProject(Project& project, const juce::File& file)
{
    try
    {
        if (!file.existsAsFile())
        {
            lastError = "File does not exist: " + file.getFullPathName();
            return false;
        }

        auto json = file.loadFileAsString();
        auto projectData = juce::JSON::parse(json);
        
        if (projectData.isVoid())
        {
            lastError = "Failed to parse JSON from file";
            return false;
        }

        if (project.fromVar(projectData))
        {
            project.setProjectFile(file);
            lastError = juce::String();
            return true;
        }
        else
        {
            lastError = "Failed to deserialize project data";
            return false;
        }
    }
    catch (const std::exception& e)
    {
        lastError = "Exception while loading: " + juce::String(e.what());
        return false;
    }
}

} // namespace KnobSmith
