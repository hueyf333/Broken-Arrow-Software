#include <JuceHeader.h>
#include "MainWindow.h"
#include "../UI/CustomLookAndFeel.h"

class KnobSmithApplication : public juce::JUCEApplication
{
public:
    KnobSmithApplication() {}

    const juce::String getApplicationName() override { return "KnobSmith"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);
        
        customLookAndFeel = std::make_unique<KnobSmith::CustomLookAndFeel>();
        juce::LookAndFeel::setDefaultLookAndFeel(customLookAndFeel.get());
        
        mainWindow = std::make_unique<KnobSmith::MainWindow>(getApplicationName());
    }

    void shutdown() override
    {
        mainWindow = nullptr;
        juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
        customLookAndFeel = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        juce::ignoreUnused(commandLine);
    }

private:
    std::unique_ptr<KnobSmith::MainWindow> mainWindow;
    std::unique_ptr<KnobSmith::CustomLookAndFeel> customLookAndFeel;
};

START_JUCE_APPLICATION(KnobSmithApplication)
