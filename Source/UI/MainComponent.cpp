#include "MainComponent.h"
#include "../Commands/CommandIDs.h"
#include "../IO/ProjectSerializer.h"
#include "../IO/ImageImporter.h"
#include "../IO/ImageExporter.h"
#include "../Canvas/FilmstripGenerator.h"

namespace KnobSmith
{

MainComponent::MainComponent() : canvas(project)
{
    setSize(1200, 800);

    addAndMakeVisible(canvas);
    addAndMakeVisible(toolPanel);
    addAndMakeVisible(layerPanel);
    addAndMakeVisible(propertiesPanel);
    addAndMakeVisible(assetBrowser);
    addAndMakeVisible(statusBar);

    // Set up tool panel callbacks
    toolPanel.onToolSelected = [this](Tool::Type type) { handleToolSelection(type); };

    // Set up layer panel callbacks
    layerPanel.onLayerSelected = [this](int index) { handleLayerSelection(index); };
    layerPanel.onLayerVisibilityChanged = [this](int, bool) { canvas.repaint(); };

    // Set up asset browser callbacks
    assetBrowser.onFileSelected = [this](const juce::File& file) {
        auto image = ImageImporter::importImage(file);
        if (image.isValid() && project.getLayerCount() > 0)
        {
            auto* layer = project.getLayer(0);
            if (layer)
            {
                layer->setImage(image);
                canvas.repaint();
            }
        }
    };

    // Set up canvas callbacks
    canvas.onStatusUpdate = [this](juce::Point<int> pos, juce::String hint) {
        statusBar.setCursorPosition(pos);
        statusBar.setStatus(hint);
    };

    // Register commands
    commandManager.registerAllCommandsForTarget(this);
    commandManager.setFirstCommandTarget(this);

    // Initialize with default tool and layer
    handleToolSelection(Tool::Type::Brush);
    if (project.getLayerCount() > 0)
        handleLayerSelection(1); // Select Base Art layer by default

    updateUI();
}

MainComponent::~MainComponent()
{
    commandManager.setFirstCommandTarget(nullptr);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    // Status bar at bottom
    statusBar.setBounds(bounds.removeFromBottom(30));

    // Tool panel on left
    toolPanel.setBounds(bounds.removeFromLeft(120));

    // Properties and asset browser on right
    auto rightPanel = bounds.removeFromRight(250);
    propertiesPanel.setBounds(rightPanel.removeFromTop(rightPanel.getHeight() / 2));
    assetBrowser.setBounds(rightPanel);

    // Layer panel on left
    layerPanel.setBounds(bounds.removeFromLeft(200));

    // Canvas in center
    canvas.setBounds(bounds);
}

void MainComponent::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    const juce::CommandID ids[] = {
        CommandIDs::fileNew,
        CommandIDs::fileOpen,
        CommandIDs::fileSave,
        CommandIDs::fileSaveAs,
        CommandIDs::fileImportImage,
        CommandIDs::fileExportPNG,
        CommandIDs::fileExportAs,
        CommandIDs::editUndo,
        CommandIDs::editRedo,
        CommandIDs::viewZoomIn,
        CommandIDs::viewZoomOut,
        CommandIDs::viewFitToScreen,
        CommandIDs::viewGrid,
        CommandIDs::viewSnap,
        CommandIDs::toolSelect,
        CommandIDs::toolBrush,
        CommandIDs::toolEraser,
        CommandIDs::toolFill,
        CommandIDs::filmstripGenerate
    };

    commands.addArray(ids, juce::numElementsInArray(ids));
}

void MainComponent::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandIDs::fileNew:
            result.setInfo("New", "Create a new project", "File", 0);
            result.addDefaultKeypress('n', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::fileOpen:
            result.setInfo("Open...", "Open an existing project", "File", 0);
            result.addDefaultKeypress('o', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::fileSave:
            result.setInfo("Save", "Save the current project", "File", 0);
            result.addDefaultKeypress('s', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::fileSaveAs:
            result.setInfo("Save As...", "Save the project with a new name", "File", 0);
            result.addDefaultKeypress('s', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier);
            break;
        case CommandIDs::fileImportImage:
            result.setInfo("Import Image...", "Import an image file", "File", 0);
            result.addDefaultKeypress('i', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::fileExportPNG:
            result.setInfo("Export PNG", "Export as PNG image", "File", 0);
            result.addDefaultKeypress('e', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::fileExportAs:
            result.setInfo("Export As...", "Export with options", "File", 0);
            break;
        case CommandIDs::editUndo:
            result.setInfo("Undo", "Undo last action", "Edit", 0);
            result.addDefaultKeypress('z', juce::ModifierKeys::commandModifier);
            result.setActive(undoManager.canUndo());
            break;
        case CommandIDs::editRedo:
            result.setInfo("Redo", "Redo last undone action", "Edit", 0);
            result.addDefaultKeypress('z', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier);
            result.setActive(undoManager.canRedo());
            break;
        case CommandIDs::viewZoomIn:
            result.setInfo("Zoom In", "Zoom in", "View", 0);
            result.addDefaultKeypress('+', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::viewZoomOut:
            result.setInfo("Zoom Out", "Zoom out", "View", 0);
            result.addDefaultKeypress('-', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::viewFitToScreen:
            result.setInfo("Fit to Screen", "Fit canvas to screen", "View", 0);
            result.addDefaultKeypress('0', juce::ModifierKeys::commandModifier);
            break;
        case CommandIDs::viewGrid:
            result.setInfo("Toggle Grid", "Show/hide grid", "View", 0);
            result.addDefaultKeypress('g', juce::ModifierKeys::commandModifier);
            result.setTicked(canvas.getGrid().isGridVisible());
            break;
        case CommandIDs::viewSnap:
            result.setInfo("Toggle Snap", "Enable/disable snap to grid", "View", 0);
            result.setTicked(canvas.getGrid().isSnapEnabled());
            break;
        case CommandIDs::toolSelect:
            result.setInfo("Select Tool", "Activate selection tool", "Tools", 0);
            result.addDefaultKeypress('v', 0);
            break;
        case CommandIDs::toolBrush:
            result.setInfo("Brush Tool", "Activate brush tool", "Tools", 0);
            result.addDefaultKeypress('b', 0);
            break;
        case CommandIDs::toolEraser:
            result.setInfo("Eraser Tool", "Activate eraser tool", "Tools", 0);
            result.addDefaultKeypress('e', 0);
            break;
        case CommandIDs::toolFill:
            result.setInfo("Fill Tool", "Activate fill tool", "Tools", 0);
            result.addDefaultKeypress('f', 0);
            break;
        case CommandIDs::filmstripGenerate:
            result.setInfo("Generate Filmstrip", "Generate filmstrip from current project", "Filmstrip", 0);
            break;
        default:
            break;
    }
}

bool MainComponent::perform(const juce::InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::fileNew: handleFileNew(); return true;
        case CommandIDs::fileOpen: handleFileOpen(); return true;
        case CommandIDs::fileSave: handleFileSave(); return true;
        case CommandIDs::fileSaveAs: handleFileSaveAs(); return true;
        case CommandIDs::fileImportImage: handleFileImportImage(); return true;
        case CommandIDs::fileExportPNG: handleFileExportPNG(); return true;
        case CommandIDs::fileExportAs: handleFileExportAs(); return true;
        case CommandIDs::editUndo: undoManager.undo(); canvas.repaint(); return true;
        case CommandIDs::editRedo: undoManager.redo(); canvas.repaint(); return true;
        case CommandIDs::viewZoomIn: canvas.zoomIn(); statusBar.setZoom(canvas.getZoom()); return true;
        case CommandIDs::viewZoomOut: canvas.zoomOut(); statusBar.setZoom(canvas.getZoom()); return true;
        case CommandIDs::viewFitToScreen: canvas.fitToScreen(); statusBar.setZoom(canvas.getZoom()); return true;
        case CommandIDs::viewGrid: 
            canvas.getGrid().setGridVisible(!canvas.getGrid().isGridVisible());
            canvas.repaint();
            return true;
        case CommandIDs::viewSnap:
            canvas.getGrid().setSnapEnabled(!canvas.getGrid().isSnapEnabled());
            return true;
        case CommandIDs::toolSelect: handleToolSelection(Tool::Type::Select); return true;
        case CommandIDs::toolBrush: handleToolSelection(Tool::Type::Brush); return true;
        case CommandIDs::toolEraser: handleToolSelection(Tool::Type::Eraser); return true;
        case CommandIDs::toolFill: handleToolSelection(Tool::Type::Fill); return true;
        case CommandIDs::filmstripGenerate:
        {
            auto filmstrip = FilmstripGenerator::generateFilmstrip(project);
            if (filmstrip.isValid())
            {
                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::InfoIcon,
                    "Filmstrip Generated",
                    "Filmstrip generated successfully with " + 
                    juce::String(project.getFilmstripConfig().getFrameCount()) + " frames");
            }
            return true;
        }
        default: return false;
    }
}

void MainComponent::handleFileNew()
{
    project.clear();
    updateUI();
    canvas.repaint();
    statusBar.setStatus("New project created");
}

void MainComponent::handleFileOpen()
{
    juce::FileChooser chooser("Open Project", juce::File(), "*.knob");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        if (ProjectSerializer::loadProject(project, file))
        {
            updateUI();
            canvas.repaint();
            statusBar.setStatus("Project loaded: " + file.getFileName());
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Load Failed",
                "Failed to load project: " + ProjectSerializer::getLastError());
        }
    }
}

void MainComponent::handleFileSave()
{
    auto file = project.getProjectFile();
    if (!file.existsAsFile())
    {
        handleFileSaveAs();
        return;
    }

    if (ProjectSerializer::saveProject(project, file))
    {
        project.setUnsavedChanges(false);
        statusBar.setStatus("Project saved");
    }
    else
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::WarningIcon,
            "Save Failed",
            "Failed to save project: " + ProjectSerializer::getLastError());
    }
}

void MainComponent::handleFileSaveAs()
{
    juce::FileChooser chooser("Save Project As", juce::File(), "*.knob");
    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult().withFileExtension(".knob");
        if (ProjectSerializer::saveProject(project, file))
        {
            project.setProjectFile(file);
            project.setUnsavedChanges(false);
            statusBar.setStatus("Project saved as: " + file.getFileName());
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Save Failed",
                "Failed to save project: " + ProjectSerializer::getLastError());
        }
    }
}

void MainComponent::handleFileImportImage()
{
    juce::FileChooser chooser("Import Image", juce::File(), ImageImporter::getSupportedFormats());
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto image = ImageImporter::importImage(file);
        
        if (image.isValid())
        {
            if (canvas.getActiveLayer())
            {
                canvas.getActiveLayer()->setImage(image);
                canvas.repaint();
                statusBar.setStatus("Image imported: " + file.getFileName());
            }
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Import Failed",
                "Failed to import image: " + ImageImporter::getLastError());
        }
    }
}

void MainComponent::handleFileExportPNG()
{
    juce::FileChooser chooser("Export PNG", juce::File(), "*.png");
    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult().withFileExtension(".png");
        auto layers = project.getLayers();
        
        std::vector<const juce::Image*> images;
        for (auto* layer : layers)
        {
            if (layer && layer->isVisible())
                images.push_back(&layer->getImage());
        }
        
        // Composite all visible layers
        juce::Image composited(juce::Image::ARGB, project.getCanvasWidth(), project.getCanvasHeight(), true);
        juce::Graphics g(composited);
        
        for (auto* layer : layers)
        {
            if (layer && layer->isVisible() && layer->getImage().isValid())
            {
                g.setOpacity(layer->getOpacity());
                g.drawImageAt(layer->getImage(), 0, 0);
            }
        }
        
        if (ImageExporter::exportPNG(composited, file, 
                                     project.getExportSettings().getTransparentBackground(),
                                     project.getExportSettings().getPremultiplyAlpha()))
        {
            statusBar.setStatus("Exported: " + file.getFileName());
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Export Failed",
                "Failed to export PNG: " + ImageExporter::getLastError());
        }
    }
}

void MainComponent::handleFileExportAs()
{
    juce::FileChooser chooser("Export Filmstrip", juce::File(), "*.png");
    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult().withFileExtension(".png");
        
        if (ImageExporter::exportFilmstrip(project, file))
        {
            statusBar.setStatus("Filmstrip exported: " + file.getFileName());
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Export Failed",
                "Failed to export filmstrip: " + ImageExporter::getLastError());
        }
    }
}

void MainComponent::handleToolSelection(Tool::Type type)
{
    Tool* tool = nullptr;
    juce::String toolName;

    switch (type)
    {
        case Tool::Type::Select:
            tool = &selectTool;
            toolName = "Select";
            break;
        case Tool::Type::Brush:
            tool = &brushTool;
            toolName = "Brush";
            break;
        case Tool::Type::Eraser:
            tool = &eraserTool;
            toolName = "Eraser";
            break;
        case Tool::Type::Fill:
            tool = &fillTool;
            toolName = "Fill";
            break;
        case Tool::Type::Shape:
            tool = &shapeTool;
            toolName = "Shape";
            break;
        case Tool::Type::ColorPicker:
            tool = &colorPickerTool;
            toolName = "Color Picker";
            break;
    }

    canvas.setActiveTool(tool);
    statusBar.setSelectedTool(toolName);
    propertiesPanel.showToolProperties(toolName);
}

void MainComponent::handleLayerSelection(int layerIndex)
{
    auto* layer = project.getLayer(layerIndex);
    canvas.setActiveLayer(layer);
    if (layer)
        statusBar.setStatus("Layer selected: " + layer->getName());
}

void MainComponent::updateUI()
{
    layerPanel.setLayers(project.getLayers());
    statusBar.setZoom(canvas.getZoom());
}

} // namespace KnobSmith
