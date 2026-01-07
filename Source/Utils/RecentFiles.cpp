#include "RecentFiles.h"

namespace KnobSmith
{

RecentFiles::RecentFiles()
{
    loadFromSettings();
}

void RecentFiles::addFile(const juce::File& file)
{
    recentFiles.addFile(file);
    recentFiles.removeNonExistentFiles();
    saveToSettings();
}

void RecentFiles::removeFile(const juce::File& file)
{
    recentFiles.removeFile(file);
    saveToSettings();
}

void RecentFiles::clear()
{
    recentFiles.clear();
    saveToSettings();
}

juce::StringArray RecentFiles::getFilePaths() const
{
    juce::StringArray paths;
    for (int i = 0; i < recentFiles.getNumFiles(); ++i)
    {
        paths.add(recentFiles.getFile(i).getFullPathName());
    }
    return paths;
}

void RecentFiles::saveToSettings()
{
    juce::PropertiesFile::Options options;
    options.applicationName = "KnobSmith";
    options.filenameSuffix = ".settings";
    options.folderName = "KnobSmith";
    options.osxLibrarySubFolder = "Application Support";
    
    juce::ApplicationProperties appProperties;
    appProperties.setStorageParameters(options);
    
    if (auto* props = appProperties.getUserSettings())
    {
        props->setValue("recentFiles", recentFiles.toString());
        props->saveIfNeeded();
    }
}

void RecentFiles::loadFromSettings()
{
    juce::PropertiesFile::Options options;
    options.applicationName = "KnobSmith";
    options.filenameSuffix = ".settings";
    options.folderName = "KnobSmith";
    options.osxLibrarySubFolder = "Application Support";
    
    juce::ApplicationProperties appProperties;
    appProperties.setStorageParameters(options);
    
    if (auto* props = appProperties.getUserSettings())
    {
        auto recentFilesString = props->getValue("recentFiles");
        if (recentFilesString.isNotEmpty())
        {
            recentFiles.restoreFromString(recentFilesString);
            recentFiles.removeNonExistentFiles();
        }
    }
}

} // namespace KnobSmith
