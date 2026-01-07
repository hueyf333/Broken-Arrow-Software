#pragma once

#include <JuceHeader.h>
#include "../UI/MainComponent.h"
#include "../Commands/CommandIDs.h"

namespace KnobSmith
{

class MenuBarModel : public juce::MenuBarModel
{
public:
    MenuBarModel(MainComponent& mainComp);
    ~MenuBarModel() override = default;

    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
    MainComponent& mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBarModel)
};

} // namespace KnobSmith
