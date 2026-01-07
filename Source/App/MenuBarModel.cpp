#include "MenuBarModel.h"

namespace KnobSmith
{

MenuBarModel::MenuBarModel(MainComponent& mainComp) : mainComponent(mainComp)
{
}

juce::StringArray MenuBarModel::getMenuBarNames()
{
    return {"File", "Edit", "View", "Tools", "Filmstrip", "Help"};
}

juce::PopupMenu MenuBarModel::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    auto& commandManager = mainComponent.getCommandManager();

    if (menuName == "File")
    {
        menu.addCommandItem(&commandManager, CommandIDs::fileNew);
        menu.addCommandItem(&commandManager, CommandIDs::fileOpen);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::fileSave);
        menu.addCommandItem(&commandManager, CommandIDs::fileSaveAs);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::fileImportImage);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::fileExportPNG);
        menu.addCommandItem(&commandManager, CommandIDs::fileExportAs);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, juce::StandardApplicationCommandIDs::quit);
    }
    else if (menuName == "Edit")
    {
        menu.addCommandItem(&commandManager, CommandIDs::editUndo);
        menu.addCommandItem(&commandManager, CommandIDs::editRedo);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, juce::StandardApplicationCommandIDs::cut);
        menu.addCommandItem(&commandManager, juce::StandardApplicationCommandIDs::copy);
        menu.addCommandItem(&commandManager, juce::StandardApplicationCommandIDs::paste);
        menu.addCommandItem(&commandManager, juce::StandardApplicationCommandIDs::del);
    }
    else if (menuName == "View")
    {
        menu.addCommandItem(&commandManager, CommandIDs::viewZoomIn);
        menu.addCommandItem(&commandManager, CommandIDs::viewZoomOut);
        menu.addCommandItem(&commandManager, CommandIDs::viewFitToScreen);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::viewGrid);
        menu.addCommandItem(&commandManager, CommandIDs::viewSnap);
    }
    else if (menuName == "Tools")
    {
        menu.addCommandItem(&commandManager, CommandIDs::toolSelect);
        menu.addCommandItem(&commandManager, CommandIDs::toolBrush);
        menu.addCommandItem(&commandManager, CommandIDs::toolEraser);
        menu.addCommandItem(&commandManager, CommandIDs::toolFill);
        menu.addCommandItem(&commandManager, CommandIDs::toolShapes);
        menu.addCommandItem(&commandManager, CommandIDs::toolColorPicker);
    }
    else if (menuName == "Filmstrip")
    {
        menu.addCommandItem(&commandManager, CommandIDs::filmstripSettings);
        menu.addCommandItem(&commandManager, CommandIDs::filmstripGenerate);
        menu.addCommandItem(&commandManager, CommandIDs::filmstripPreview);
    }
    else if (menuName == "Help")
    {
        menu.addCommandItem(&commandManager, CommandIDs::helpShortcuts);
        menu.addCommandItem(&commandManager, CommandIDs::helpAbout);
    }

    return menu;
}

void MenuBarModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    juce::ignoreUnused(menuItemID, topLevelMenuIndex);
}

} // namespace KnobSmith
