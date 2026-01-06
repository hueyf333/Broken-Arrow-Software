using SkinningStudio.Commands;

namespace SkinningStudio.Services
{
    public class UndoRedoService
    {
        private readonly Stack<IUndoableCommand> _undoStack = new();
        private readonly Stack<IUndoableCommand> _redoStack = new();

        public event EventHandler? StacksChanged;

        public bool CanUndo => _undoStack.Count > 0;
        public bool CanRedo => _redoStack.Count > 0;

        public void ExecuteCommand(IUndoableCommand command)
        {
            command.Execute();
            _undoStack.Push(command);
            _redoStack.Clear();
            OnStacksChanged();
        }

        public void Undo()
        {
            if (!CanUndo) return;

            var command = _undoStack.Pop();
            command.Undo();
            _redoStack.Push(command);
            OnStacksChanged();
        }

        public void Redo()
        {
            if (!CanRedo) return;

            var command = _redoStack.Pop();
            command.Execute();
            _undoStack.Push(command);
            OnStacksChanged();
        }

        public void Clear()
        {
            _undoStack.Clear();
            _redoStack.Clear();
            OnStacksChanged();
        }

        private void OnStacksChanged() => StacksChanged?.Invoke(this, EventArgs.Empty);
    }
}
