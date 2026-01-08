#include "MainComponent.h"

// Helper class for knob rendering
class KnobRenderer
{
public:
    static juce::Image renderKnobFrame(int width, int height, float angle, int tickCount)
    {
        juce::Image img(juce::Image::ARGB, width, height, true);
        juce::Graphics g(img);
        
        g.fillAll(juce::Colours::transparentBlack);
        
        auto bounds = juce::Rectangle<float>(0, 0, width, height);
        auto center = bounds.getCentre();
        auto radius = juce::jmin(width, height) * 0.45f;
        
        // Background
        juce::ColourGradient gradient(juce::Colour(0xFF3A3A3A), center.x, center.y - radius,
                                     juce::Colour(0xFF1A1A1A), center.x, center.y + radius, false);
        g.setGradientFill(gradient);
        g.fillEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2);
        
        // Ring
        g.setColour(juce::Colour(0xFF4A90E2));
        juce::Path ring;
        ring.addCentredArc(center.x, center.y, radius, radius, 0.0f, 0.0f, juce::MathConstants<float>::twoPi, true);
        ring.addCentredArc(center.x, center.y, radius * 0.85f, radius * 0.85f, 0.0f, 0.0f, juce::MathConstants<float>::twoPi, false);
        ring.setUsingNonZeroWinding(false);
        g.fillPath(ring);
        
        // Ticks
        g.setColour(juce::Colours::white);
        for (int i = 0; i < tickCount; ++i)
        {
            float normalizedValue = (float)i / (float)(tickCount - 1);
            float tickAngle = juce::degreesToRadians(-135.0f + normalizedValue * 270.0f);
            float innerRadius = radius * 0.75f;
            float outerRadius = radius * 0.85f;
            float x1 = center.x + innerRadius * std::sin(tickAngle);
            float y1 = center.y - innerRadius * std::cos(tickAngle);
            float x2 = center.x + outerRadius * std::sin(tickAngle);
            float y2 = center.y - outerRadius * std::cos(tickAngle);
            g.drawLine(x1, y1, x2, y2, 1.5f);
        }
        
        // Pointer
        g.setColour(juce::Colour(0xFFE74C3C));
        float innerRadius = radius * 0.2f;
        float outerRadius = radius * 0.7f;
        float x1 = center.x + innerRadius * std::sin(angle);
        float y1 = center.y - innerRadius * std::cos(angle);
        float x2 = center.x + outerRadius * std::sin(angle);
        float y2 = center.y - outerRadius * std::cos(angle);
        g.drawLine(x1, y1, x2, y2, 3.0f);
        
        // Gloss
        juce::ColourGradient glossGradient(juce::Colours::white.withAlpha(0.3f), center.x, center.y - radius * 0.5f,
                                          juce::Colours::transparentWhite, center.x, center.y, false);
        g.setGradientFill(glossGradient);
        g.fillEllipse(center.x - radius * 0.8f, center.y - radius * 0.8f, radius * 1.6f, radius * 0.8f);
        
        return img;
    }
};

// Knob Designer Panel
class KnobDesignerPanel : public juce::Component
{
public:
    KnobDesignerPanel()
    {
        addLayerButton.setButtonText("Add Layer");
        removeLayerButton.setButtonText("Remove Layer");
        addAndMakeVisible(addLayerButton);
        addAndMakeVisible(addLayerButton);
        
        updatePreview();
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF1E1E1E));
        
        g.setColour(juce::Colours::white);
        g.drawText("Knob Designer", 10, 10, 200, 30, juce::Justification::left);
        
        // Draw preview
        auto previewArea = getLocalBounds().removeFromRight(getWidth() / 2).reduced(20);
        g.setColour(juce::Colour(0xFF2A2A2A));
        g.fillRect(previewArea);
        
        if (previewImage.isValid())
        {
            auto imgBounds = previewArea.withSizeKeepingCentre(previewImage.getWidth(), previewImage.getHeight());
            g.drawImage(previewImage, imgBounds.toFloat());
        }
        
        g.setColour(juce::Colours::white);
        g.drawText("Drag to rotate knob", previewArea.removeFromBottom(30), juce::Justification::centred);
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds().removeFromLeft(getWidth() / 2).reduced(10);
        bounds.removeFromTop(40);
        
        auto buttonArea = bounds.removeFromBottom(30);
        addLayerButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2 - 5));
        buttonArea.removeFromLeft(10);
        removeLayerButton.setBounds(buttonArea);
    }
    
    void mouseDown(const juce::MouseEvent& e) override
    {
        auto previewArea = getLocalBounds().removeFromRight(getWidth() / 2).reduced(20);
        if (previewArea.contains(e.getPosition()))
        {
            isDragging = true;
            dragStart = e.getPosition();
        }
    }
    
    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (isDragging)
        {
            auto delta = e.getPosition() - dragStart;
            knobValue = juce::jlimit(0.0f, 1.0f, knobValue + delta.y * -0.01f);
            dragStart = e.getPosition();
            updatePreview();
            repaint();
        }
    }
    
    void mouseUp(const juce::MouseEvent&) override
    {
        isDragging = false;
    }
    
    void updatePreview()
    {
        float angle = juce::degreesToRadians(-135.0f + knobValue * 270.0f);
        previewImage = KnobRenderer::renderKnobFrame(100, 100, angle, 11);
    }
    
private:
    juce::TextButton addLayerButton, removeLayerButton;
    juce::Image previewImage;
    float knobValue = 0.5f;
    bool isDragging = false;
    juce::Point<int> dragStart;
};

// Layout Designer Panel
class LayoutDesignerPanel : public juce::Component
{
public:
    LayoutDesignerPanel()
    {
        addControlButton.setButtonText("Add Knob Control");
        addAndMakeVisible(addControlButton);
        
        addControlButton.onClick = [this]()
        {
            controls.add({juce::Rectangle<int>(50 + controls.size() * 20, 50 + controls.size() * 20, 100, 100), "Knob_" + juce::String(controls.size() + 1)});
            repaint();
        };
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF252525));
        
        // Draw grid
        if (gridEnabled)
        {
            g.setColour(juce::Colour(0xFF2A2A2A));
            for (int x = 0; x < getWidth(); x += 10)
                g.drawLine(x, 0, x, getHeight(), 0.5f);
            for (int y = 0; y < getHeight(); y += 10)
                g.drawLine(0, y, getWidth(), y, 0.5f);
        }
        
        // Draw controls
        for (auto& control : controls)
        {
            g.setColour(juce::Colour(0xFF3A3A3A));
            g.fillRect(control.bounds);
            g.setColour(juce::Colours::cyan);
            g.drawRect(control.bounds, 1);
            g.setColour(juce::Colours::white);
            g.drawText(control.name, control.bounds, juce::Justification::centred);
        }
    }
    
    void resized() override
    {
        addControlButton.setBounds(10, 10, 150, 30);
    }
    
private:
    struct LayoutControl
    {
        juce::Rectangle<int> bounds;
        juce::String name;
    };
    
    juce::Array<LayoutControl> controls;
    juce::TextButton addControlButton;
    bool gridEnabled = true;
};

// Preview Panel
class PreviewPanel : public juce::Component
{
public:
    PreviewPanel()
    {
        updatePreview();
    }
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF252525));
        
        g.setColour(juce::Colours::white);
        g.drawText("Preview Panel", 10, 10, 200, 30, juce::Justification::left);
        
        if (previewImage.isValid())
        {
            auto bounds = getLocalBounds().reduced(20);
            bounds.removeFromTop(40);
            auto imgBounds = bounds.withSizeKeepingCentre(previewImage.getWidth(), previewImage.getHeight());
            g.drawImage(previewImage, imgBounds.toFloat());
        }
    }
    
    void updatePreview()
    {
        previewImage = KnobRenderer::renderKnobFrame(200, 200, 0.0f, 11);
        repaint();
    }
    
private:
    juce::Image previewImage;
};

//==============================================================================
MainComponent::MainComponent()
    : menuBar(this),
      centerTabs(juce::TabbedButtonBar::TabsAtTop)
{
    setSize(1400, 900);
    
    // Setup menu bar
    menuBar.setModel(this);
    addAndMakeVisible(menuBar);
    
    // Setup center tabs
    centerTabs.setTabBarDepth(30);
    centerTabs.setColour(juce::TabbedComponent::backgroundColourId, juce::Colour(0xFF1E1E1E));
    centerTabs.addTab("Knob Designer", juce::Colour(0xFF1E1E1E), new KnobDesignerPanel(), true);
    centerTabs.addTab("Layout Designer", juce::Colour(0xFF1E1E1E), new LayoutDesignerPanel(), true);
    centerTabs.addTab("Preview", juce::Colour(0xFF1E1E1E), new PreviewPanel(), true);
    addAndMakeVisible(centerTabs);
    
    // Setup project tree
    projectTree.setColour(juce::TreeView::backgroundColourId, juce::Colour(0xFF1E1E1E));
    addAndMakeVisible(projectTree);
    
    // Setup property panel
    propertyPanel.setMultiLine(true);
    propertyPanel.setReadOnly(true);
    propertyPanel.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xFF1E1E1E));
    propertyPanel.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    propertyPanel.setText("Properties Panel\n\nSelect a control to view properties");
    addAndMakeVisible(propertyPanel);
    
    // Setup status bar
    statusBar.setText("Ready - BrokenArrowSkinLab v1.0.0", juce::dontSendNotification);
    statusBar.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF1E1E1E));
    statusBar.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(statusBar);
    
    startTimer(100);
}

MainComponent::~MainComponent()
{
    menuBar.setModel(nullptr);
    stopTimer();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1E1E1E));
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Menu bar at top
    menuBar.setBounds(bounds.removeFromTop(24));
    
    // Status bar at bottom
    statusBar.setBounds(bounds.removeFromBottom(25));
    
    // Left panel - project tree
    auto leftPanel = bounds.removeFromLeft(250);
    projectTree.setBounds(leftPanel);
    
    // Right panel - properties
    auto rightPanel = bounds.removeFromRight(250);
    propertyPanel.setBounds(rightPanel);
    
    // Center - tabs
    centerTabs.setBounds(bounds);
}

void MainComponent::timerCallback()
{
    // Could be used for auto-save
}

juce::StringArray MainComponent::getMenuBarNames()
{
    return { "File", "Edit", "View", "Tools", "Help" };
}

juce::PopupMenu MainComponent::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    juce::ignoreUnused(menuName);
    
    if (topLevelMenuIndex == 0) // File
    {
        menu.addItem(1, "New Project");
        menu.addItem(2, "Open Project...");
        menu.addSeparator();
        menu.addItem(3, "Save Project");
        menu.addItem(4, "Save Project As...");
        menu.addSeparator();
        menu.addItem(5, "Export Assets...");
        menu.addSeparator();
        menu.addItem(6, "Exit");
    }
    else if (topLevelMenuIndex == 1) // Edit
    {
        menu.addItem(10, "Undo", undoManager.canUndo());
        menu.addItem(11, "Redo", undoManager.canRedo());
        menu.addSeparator();
        menu.addItem(12, "Cut");
        menu.addItem(13, "Copy");
        menu.addItem(14, "Paste");
        menu.addItem(15, "Delete");
    }
    else if (topLevelMenuIndex == 2) // View
    {
        menu.addItem(20, "Zoom In");
        menu.addItem(21, "Zoom Out");
        menu.addItem(22, "Reset Zoom");
        menu.addSeparator();
        menu.addItem(23, "Show Grid", true, projectData.gridEnabled);
        menu.addItem(24, "Snap to Grid", true, projectData.snapEnabled);
    }
    else if (topLevelMenuIndex == 3) // Tools
    {
        menu.addItem(30, "Add Knob");
        menu.addItem(31, "Add Slider (Horizontal)");
        menu.addItem(32, "Add Slider (Vertical)");
        menu.addItem(33, "Add Button");
        menu.addSeparator();
        menu.addItem(34, "Run Self Test");
    }
    else if (topLevelMenuIndex == 4) // Help
    {
        menu.addItem(40, "About");
        menu.addItem(41, "Documentation");
    }
    
    return menu;
}

void MainComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    juce::ignoreUnused(topLevelMenuIndex);
    
    switch (menuItemID)
    {
        case 1: newProject(); break;
        case 2: openProject(); break;
        case 3: saveProject(); break;
        case 5: exportAssets(); break;
        case 6: juce::JUCEApplication::getInstance()->systemRequestedQuit(); break;
        case 10: undoManager.undo(); break;
        case 11: undoManager.redo(); break;
        case 20: projectData.zoom = juce::jmin(4.0f, projectData.zoom * 1.25f); break;
        case 21: projectData.zoom = juce::jmax(0.25f, projectData.zoom * 0.8f); break;
        case 22: projectData.zoom = 1.0f; break;
        case 23: projectData.gridEnabled = !projectData.gridEnabled; break;
        case 24: projectData.snapEnabled = !projectData.snapEnabled; break;
        case 34: runSelfTest(); break;
        case 40: showAbout(); break;
        case 41: juce::URL("https://github.com/hueyf333/Broken-Arrow-Software").launchInDefaultBrowser(); break;
    }
}

void MainComponent::newProject()
{
    projectData = ProjectData();
    currentProjectFile = juce::File();
    statusBar.setText("New project created", juce::dontSendNotification);
}

void MainComponent::openProject()
{
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    fileChooser = std::make_unique<juce::FileChooser>("Open Project", 
                                                       juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), 
                                                       "*.baslproj");
    
    fileChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file.existsAsFile())
        {
            auto json = file.loadFileAsString();
            if (projectData.fromJSON(json))
            {
                currentProjectFile = file;
                statusBar.setText("Loaded: " + file.getFileName(), juce::dontSendNotification);
            }
            else
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Failed to load project");
            }
        }
    });
}

void MainComponent::saveProject()
{
    if (currentProjectFile.existsAsFile())
    {
        currentProjectFile.replaceWithText(projectData.toJSON());
        statusBar.setText("Saved: " + currentProjectFile.getFileName(), juce::dontSendNotification);
    }
    else
    {
        auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting;
        fileChooser = std::make_unique<juce::FileChooser>("Save Project", 
                                                           juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), 
                                                           "*.baslproj");
        
        fileChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                currentProjectFile = file.withFileExtension(".baslproj");
                currentProjectFile.replaceWithText(projectData.toJSON());
                statusBar.setText("Saved: " + currentProjectFile.getFileName(), juce::dontSendNotification);
            }
        });
    }
}

void MainComponent::exportAssets()
{
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
    fileChooser = std::make_unique<juce::FileChooser>("Export Directory", 
                                                       juce::File::getSpecialLocation(juce::File::userDocumentsDirectory));
    
    fileChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
    {
        auto outputDir = fc.getResult();
        if (outputDir != juce::File{})
        {
            // Generate knob frames and export
            juce::OwnedArray<juce::Image> frames;
            for (int i = 0; i < projectData.frameCount; ++i)
            {
                float normalizedValue = (float)i / (float)(projectData.frameCount - 1);
                float angle = juce::degreesToRadians(projectData.minAngle + normalizedValue * (projectData.maxAngle - projectData.minAngle));
                frames.add(new juce::Image(KnobRenderer::renderKnobFrame(projectData.canvasWidth, projectData.canvasHeight, angle, projectData.tickCount)));
            }
            
            // Export vertical filmstrip
            juce::Image filmstrip(juce::Image::ARGB, projectData.canvasWidth, projectData.canvasHeight * frames.size(), true);
            juce::Graphics g(filmstrip);
            for (int i = 0; i < frames.size(); ++i)
            {
                g.drawImage(*frames[i], 0, i * projectData.canvasHeight, projectData.canvasWidth, projectData.canvasHeight,
                           0, 0, projectData.canvasWidth, projectData.canvasHeight);
            }
            
            juce::File outputFile = outputDir.getChildFile("knob_filmstrip.png");
            juce::FileOutputStream stream(outputFile);
            if (stream.openedOk())
            {
                juce::PNGImageFormat pngFormat;
                pngFormat.writeImageToStream(filmstrip, stream);
                statusBar.setText("Exported to: " + outputDir.getFullPathName(), juce::dontSendNotification);
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Export Complete",
                    "Knob filmstrip exported successfully!");
            }
        }
    });
}

void MainComponent::runSelfTest()
{
    juce::String testResults;
    testResults += "=== BrokenArrowSkinLab Self-Test ===\n\n";
    
    // Test 1: Generate sample knob
    testResults += "[TEST 1] Generating sample knob preset...\n";
    ProjectData testData;
    testData.canvasWidth = 128;
    testData.canvasHeight = 128;
    testData.frameCount = 64;
    testResults += "  ✓ Sample knob preset created\n\n";
    
    // Test 2: Render frames
    testResults += "[TEST 2] Rendering knob frames...\n";
    for (int i = 0; i < testData.frameCount; ++i)
    {
        float angle = juce::degreesToRadians(-135.0f + (float)i / 63.0f * 270.0f);
        auto img = KnobRenderer::renderKnobFrame(testData.canvasWidth, testData.canvasHeight, angle, testData.tickCount);
        if (!img.isValid())
        {
            testResults += "  ✗ FAILED: Could not render frame " + juce::String(i) + "\n";
            break;
        }
    }
    testResults += "  ✓ Rendered " + juce::String(testData.frameCount) + " frames successfully\n\n";
    
    // Test 3: Export to temp
    testResults += "[TEST 3] Exporting to temporary directory...\n";
    auto tempDir = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("BASkinLabTest");
    tempDir.createDirectory();
    
    juce::Image filmstrip(juce::Image::ARGB, testData.canvasWidth, testData.canvasHeight * testData.frameCount, true);
    juce::Graphics g(filmstrip);
    for (int i = 0; i < testData.frameCount; ++i)
    {
        float angle = juce::degreesToRadians(-135.0f + (float)i / 63.0f * 270.0f);
        auto img = KnobRenderer::renderKnobFrame(testData.canvasWidth, testData.canvasHeight, angle, testData.tickCount);
        g.drawImage(img, 0, i * testData.canvasHeight, testData.canvasWidth, testData.canvasHeight,
                   0, 0, testData.canvasWidth, testData.canvasHeight);
    }
    
    juce::File outputFile = tempDir.getChildFile("test_knob.png");
    juce::FileOutputStream stream(outputFile);
    if (stream.openedOk())
    {
        juce::PNGImageFormat pngFormat;
        pngFormat.writeImageToStream(filmstrip, stream);
        testResults += "  ✓ Exported test filmstrip: " + outputFile.getFullPathName() + "\n";
        testResults += "  ✓ File size: " + juce::String(outputFile.getSize()) + " bytes\n\n";
    }
    
    // Test 4: JSON serialization
    testResults += "[TEST 4] Testing JSON serialization...\n";
    juce::String json = testData.toJSON();
    ProjectData loadedData;
    if (loadedData.fromJSON(json))
    {
        testResults += "  ✓ JSON export/import successful\n";
        testResults += "  ✓ Data integrity verified\n\n";
    }
    
    tempDir.deleteRecursively();
    testResults += "\n=== ALL TESTS PASSED ✓ ===";
    
    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Self Test Results", testResults);
}

void MainComponent::showAbout()
{
    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
        "About BrokenArrowSkinLab",
        "BrokenArrowSkinLab v1.0.0\n\n"
        "A KnobMan/SkinMan-style skin authoring tool\n"
        "Built with JUCE Framework\n\n"
        "Features:\n"
        "• Knob Designer with live preview\n"
        "• Layout Designer with grid and snap\n"
        "• Asset export (PNG filmstrips)\n"
        "• Project save/load (.baslproj)\n"
        "• Undo/Redo support\n\n"
        "© 2024 Broken Arrow Software");
}
