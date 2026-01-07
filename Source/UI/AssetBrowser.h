#pragma once

#include <JuceHeader.h>

namespace KnobSmith
{

class AssetBrowser : public juce::Component,
                     private juce::FileBrowserListener
{
public:
    AssetBrowser();
    ~AssetBrowser() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void selectionChanged() override;
    void fileClicked(const juce::File& file, const juce::MouseEvent& e) override;
    void fileDoubleClicked(const juce::File& file) override;
    void browserRootChanged(const juce::File& newRoot) override;

    std::function<void(const juce::File&)> onFileSelected;

private:
    juce::File currentRoot;
    std::unique_ptr<juce::DirectoryContentsList> fileList;
    std::unique_ptr<juce::FileTreeComponent> fileTree;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssetBrowser)
};

} // namespace KnobSmith
