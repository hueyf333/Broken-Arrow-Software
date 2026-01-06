#include "core/CommandManager.h"

void CommandManager::ExecuteCommand(std::unique_ptr<Command> command) {
    if (!command) return;
    
    // Remove any commands after current index
    if (m_currentIndex < static_cast<int>(m_commands.size()) - 1) {
        m_commands.erase(m_commands.begin() + m_currentIndex + 1, m_commands.end());
    }
    
    command->Execute();
    m_commands.push_back(std::move(command));
    m_currentIndex++;
}

void CommandManager::Undo() {
    if (!CanUndo()) return;
    
    m_commands[m_currentIndex]->Undo();
    m_currentIndex--;
}

void CommandManager::Redo() {
    if (!CanRedo()) return;
    
    m_currentIndex++;
    m_commands[m_currentIndex]->Execute();
}

bool CommandManager::CanUndo() const {
    return m_currentIndex >= 0;
}

bool CommandManager::CanRedo() const {
    return m_currentIndex < static_cast<int>(m_commands.size()) - 1;
}

void CommandManager::Clear() {
    m_commands.clear();
    m_currentIndex = -1;
}

std::string CommandManager::GetUndoDescription() const {
    if (!CanUndo()) return "";
    return m_commands[m_currentIndex]->GetDescription();
}

std::string CommandManager::GetRedoDescription() const {
    if (!CanRedo()) return "";
    return m_commands[m_currentIndex + 1]->GetDescription();
}
