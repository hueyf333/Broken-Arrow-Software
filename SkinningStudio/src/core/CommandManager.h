#pragma once

#include <memory>
#include <vector>
#include <string>

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual std::string GetDescription() const = 0;
};

class CommandManager {
public:
    void ExecuteCommand(std::unique_ptr<Command> command);
    void Undo();
    void Redo();
    
    bool CanUndo() const;
    bool CanRedo() const;
    
    void Clear();
    
    std::string GetUndoDescription() const;
    std::string GetRedoDescription() const;
    
private:
    std::vector<std::unique_ptr<Command>> m_commands;
    int m_currentIndex = -1;
};
