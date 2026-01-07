#include "AssetBrowser.h"

namespace KnobSmith
{

AssetBrowser::AssetBrowser()
{
    currentRoot = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    
    juce::WildcardFileFilter fileFilter("*.png;*.jpg;*.jpeg;*.bmp;*.gif", juce::String(), "Image files");
    
    fileList = std::make_unique<juce::DirectoryContentsList>(&fileFilter, *this);
    fileList->setDirectory(currentRoot, true, true);
    
    fileTree = std::make_unique<juce::FileTreeComponent>(*fileList);
    addAndMakeVisible(fileTree.get());
}

void AssetBrowser::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff252526));
}

void AssetBrowser::resized()
{
    if (fileTree)
        fileTree->setBounds(getLocalBounds());
}

void AssetBrowser::selectionChanged()
{
}

void AssetBrowser::fileClicked(const juce::File& file, const juce::MouseEvent& e)
{
    juce::ignoreUnused(e);
    if (file.existsAsFile() && onFileSelected)
    {
        onFileSelected(file);
    }
}

void AssetBrowser::fileDoubleClicked(const juce::File& file)
{
    if (file.existsAsFile() && onFileSelected)
    {
        onFileSelected(file);
    }
}

void AssetBrowser::browserRootChanged(const juce::File& newRoot)
{
    currentRoot = newRoot;
}

} // namespace KnobSmith
