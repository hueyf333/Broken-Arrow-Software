#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class RecentFiles
{
public:
    RecentFiles();
    ~RecentFiles() = default;

    void addFile(const juce::File& file);
    void removeFile(const juce::File& file);
    void clear();
    
    int getNumFiles() const { return recentFiles.getNumFiles(); }
    juce::File getFile(int index) const { return recentFiles.getFile(index); }
    
    juce::StringArray getFilePaths() const;

private:
    juce::RecentlyOpenedFilesList recentFiles;
    static constexpr int maxRecentFiles = 10;

    void saveToSettings();
    void loadFromSettings();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecentFiles)
};

} // namespace KnobSmith
