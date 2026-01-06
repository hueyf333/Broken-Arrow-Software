using SkinningStudio.Models;

namespace SkinningStudio.Commands
{
    public class ModifyElementCommand : IUndoableCommand
    {
        private readonly UIElementModel _element;
        private readonly Action<UIElementModel> _doAction;
        private readonly Action<UIElementModel> _undoAction;
        private readonly string _description;

        public ModifyElementCommand(UIElementModel element, Action<UIElementModel> doAction, Action<UIElementModel> undoAction, string description)
        {
            _element = element;
            _doAction = doAction;
            _undoAction = undoAction;
            _description = description;
        }

        public string Description => _description;

        public void Execute() => _doAction(_element);

        public void Undo() => _undoAction(_element);
    }
}
