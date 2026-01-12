"""Command pattern implementation for undo/redo functionality"""

from typing import List, Optional, Any
from abc import ABC, abstractmethod
from .logger import logger


class Command(ABC):
    """Base class for all commands"""
    
    def __init__(self, description: str = ""):
        self.description = description
    
    @abstractmethod
    def execute(self) -> bool:
        """Execute the command. Returns True if successful."""
        pass
    
    @abstractmethod
    def undo(self) -> bool:
        """Undo the command. Returns True if successful."""
        pass
    
    def __str__(self) -> str:
        return self.description or self.__class__.__name__


class CommandHistory:
    """Manages command history for undo/redo"""
    
    def __init__(self, max_history: int = 100):
        self.max_history = max_history
        self.undo_stack: List[Command] = []
        self.redo_stack: List[Command] = []
        self._enabled = True
    
    def execute(self, command: Command) -> bool:
        """Execute a command and add to history"""
        if not self._enabled:
            return command.execute()
        
        try:
            success = command.execute()
            if success:
                self.undo_stack.append(command)
                self.redo_stack.clear()
                
                # Limit history size
                if len(self.undo_stack) > self.max_history:
                    self.undo_stack.pop(0)
                
                logger.debug(f"Command executed: {command}")
            return success
        except Exception as e:
            logger.error(f"Command execution failed: {command} - {e}")
            logger.exception(e)
            return False
    
    def undo(self) -> bool:
        """Undo the last command"""
        if not self.can_undo():
            return False
        
        command = self.undo_stack.pop()
        try:
            success = command.undo()
            if success:
                self.redo_stack.append(command)
                logger.debug(f"Command undone: {command}")
            else:
                # If undo failed, put it back
                self.undo_stack.append(command)
            return success
        except Exception as e:
            logger.error(f"Command undo failed: {command} - {e}")
            logger.exception(e)
            self.undo_stack.append(command)
            return False
    
    def redo(self) -> bool:
        """Redo the last undone command"""
        if not self.can_redo():
            return False
        
        command = self.redo_stack.pop()
        try:
            success = command.execute()
            if success:
                self.undo_stack.append(command)
                logger.debug(f"Command redone: {command}")
            else:
                # If redo failed, put it back
                self.redo_stack.append(command)
            return success
        except Exception as e:
            logger.error(f"Command redo failed: {command} - {e}")
            logger.exception(e)
            self.redo_stack.append(command)
            return False
    
    def can_undo(self) -> bool:
        """Check if undo is available"""
        return len(self.undo_stack) > 0
    
    def can_redo(self) -> bool:
        """Check if redo is available"""
        return len(self.redo_stack) > 0
    
    def clear(self):
        """Clear all history"""
        self.undo_stack.clear()
        self.redo_stack.clear()
        logger.debug("Command history cleared")
    
    def get_undo_description(self) -> Optional[str]:
        """Get description of command that would be undone"""
        if self.can_undo():
            return str(self.undo_stack[-1])
        return None
    
    def get_redo_description(self) -> Optional[str]:
        """Get description of command that would be redone"""
        if self.can_redo():
            return str(self.redo_stack[-1])
        return None
    
    def disable(self):
        """Temporarily disable command recording"""
        self._enabled = False
    
    def enable(self):
        """Re-enable command recording"""
        self._enabled = True


class CompoundCommand(Command):
    """A command that contains multiple sub-commands"""
    
    def __init__(self, description: str = "Compound Command"):
        super().__init__(description)
        self.commands: List[Command] = []
    
    def add_command(self, command: Command):
        """Add a sub-command"""
        self.commands.append(command)
    
    def execute(self) -> bool:
        """Execute all sub-commands"""
        for cmd in self.commands:
            if not cmd.execute():
                # Undo all previously executed commands
                for prev_cmd in reversed(self.commands[:self.commands.index(cmd)]):
                    prev_cmd.undo()
                return False
        return True
    
    def undo(self) -> bool:
        """Undo all sub-commands in reverse order"""
        for cmd in reversed(self.commands):
            if not cmd.undo():
                return False
        return True
