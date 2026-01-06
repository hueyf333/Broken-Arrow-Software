#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace SkinningStudio {

// Command interface for undo/redo
class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual std::string GetDescription() const = 0;
};

class CommandManager {
public:
    CommandManager();
    ~CommandManager();

    void ExecuteCommand(std::unique_ptr<Command> command);
    void Undo();
    void Redo();
    
    bool CanUndo() const;
    bool CanRedo() const;
    
    std::string GetUndoDescription() const;
    std::string GetRedoDescription() const;
    
    void Clear();

private:
    std::vector<std::unique_ptr<Command>> m_undoStack;
    std::vector<std::unique_ptr<Command>> m_redoStack;
    size_t m_maxStackSize;
};

} // namespace SkinningStudio
