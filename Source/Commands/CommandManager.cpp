#include "CommandManager.h"

namespace KnobSmith
{

KnobSmithCommandManager::KnobSmithCommandManager()
{
}

void KnobSmithCommandManager::registerAllCommandsForTarget(juce::ApplicationCommandTarget* target)
{
    commandManager.registerAllCommandsForTarget(target);
}

} // namespace KnobSmith
