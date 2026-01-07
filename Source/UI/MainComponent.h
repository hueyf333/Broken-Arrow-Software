#pragma once

#include <JuceHeader.h>
#include "../Model/Project.h"
#include "../Canvas/CanvasComponent.h"
#include "../Tools/SelectTool.h"
#include "../Tools/BrushTool.h"
#include "../Tools/EraserTool.h"
#include "../Tools/FillTool.h"
#include "../Tools/ShapeTool.h"
#include "../Tools/ColorPickerTool.h"
#include "ToolPanel.h"
#include "LayerPanel.h"
#include "PropertiesPanel.h"
#include "AssetBrowser.h"
#include "StatusBar.h"

namespace KnobSmith
{

class MainComponent : public juce::Component,
                      public juce::ApplicationCommandTarget
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Command target
    juce::ApplicationCommandTarget* getNextCommandTarget() override { return nullptr; }
    void getAllCommands(juce::Array<juce::CommandID>& commands) override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform(const juce::InvocationInfo& info) override;

    Project& getProject() { return project; }
    juce::ApplicationCommandManager& getCommandManager() { return commandManager; }

private:
    Project project;
    juce::ApplicationCommandManager commandManager;
    juce::UndoManager undoManager;

    // Tools
    SelectTool selectTool;
    BrushTool brushTool;
    EraserTool eraserTool;
    FillTool fillTool;
    ShapeTool shapeTool;
    ColorPickerTool colorPickerTool;

    // Components
    CanvasComponent canvas;
    ToolPanel toolPanel;
    LayerPanel layerPanel;
    PropertiesPanel propertiesPanel;
    AssetBrowser assetBrowser;
    StatusBar statusBar;

    void handleFileNew();
    void handleFileOpen();
    void handleFileSave();
    void handleFileSaveAs();
    void handleFileImportImage();
    void handleFileExportPNG();
    void handleFileExportAs();

    void handleToolSelection(Tool::Type type);
    void handleLayerSelection(int layerIndex);

    void updateUI();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

} // namespace KnobSmith
