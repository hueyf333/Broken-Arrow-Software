#include "CommandManager.h"

namespace SkinningStudio {

CommandManager::CommandManager()
    : m_maxStackSize(100)
{
}

CommandManager::~CommandManager() = default;

void CommandManager::ExecuteCommand(std::unique_ptr<Command> command) {
    if (command) {
        command->Execute();
        m_undoStack.push_back(std::move(command));
        m_redoStack.clear();
        
        // Limit stack size
        if (m_undoStack.size() > m_maxStackSize) {
            m_undoStack.erase(m_undoStack.begin());
        }
    }
}

void CommandManager::Undo() {
    if (CanUndo()) {
        auto command = std::move(m_undoStack.back());
        m_undoStack.pop_back();
        command->Undo();
        m_redoStack.push_back(std::move(command));
    }
}

void CommandManager::Redo() {
    if (CanRedo()) {
        auto command = std::move(m_redoStack.back());
        m_redoStack.pop_back();
        command->Execute();
        m_undoStack.push_back(std::move(command));
    }
}

bool CommandManager::CanUndo() const {
    return !m_undoStack.empty();
}

bool CommandManager::CanRedo() const {
    return !m_redoStack.empty();
}

std::string CommandManager::GetUndoDescription() const {
    if (CanUndo()) {
        return m_undoStack.back()->GetDescription();
    }
    return "";
}

std::string CommandManager::GetRedoDescription() const {
    if (CanRedo()) {
        return m_redoStack.back()->GetDescription();
    }
    return "";
}

void CommandManager::Clear() {
    m_undoStack.clear();
    m_redoStack.clear();
}

} // namespace SkinningStudio
