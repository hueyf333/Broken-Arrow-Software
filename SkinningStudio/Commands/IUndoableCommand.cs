namespace SkinningStudio.Commands
{
    public interface IUndoableCommand
    {
        void Execute();
        void Undo();
        string Description { get; }
    }
}
