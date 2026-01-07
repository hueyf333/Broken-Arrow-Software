#pragma once

#include <JuceHeader.h>
#include "../UI/MainComponent.h"
#include "MenuBarModel.h"

namespace KnobSmith
{

class MainWindow : public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);
    ~MainWindow() override = default;

    void closeButtonPressed() override;

    MainComponent& getMainComponent() { return *mainComponent; }

private:
    std::unique_ptr<MainComponent> mainComponent;
    std::unique_ptr<MenuBarModel> menuBarModel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

} // namespace KnobSmith
