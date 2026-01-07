#pragma once

#include <JuceHeader.h>
#include "CommandIDs.h"

namespace KnobSmith
{

class KnobSmithCommandManager
{
public:
    KnobSmithCommandManager();
    ~KnobSmithCommandManager() = default;

    juce::ApplicationCommandManager& getCommandManager() { return commandManager; }

    void registerAllCommandsForTarget(juce::ApplicationCommandTarget* target);

private:
    juce::ApplicationCommandManager commandManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobSmithCommandManager)
};

} // namespace KnobSmith
