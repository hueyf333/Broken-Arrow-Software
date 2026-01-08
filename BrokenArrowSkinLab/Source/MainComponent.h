#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>

//==============================================================================
class MainComponent : public juce::Component,
                      private juce::MenuBarModel,
                      private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // MenuBarModel implementation
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
    
    void timerCallback() override;
    
    // Menu actions
    void newProject();
    void openProject();
    void saveProject();
    void exportAssets();
    void runSelfTest();
    void showAbout();
    
    // UI Components
    juce::MenuBarComponent menuBar;
    juce::TabbedComponent centerTabs;
    juce::TreeView projectTree;
    juce::Label statusBar;
    juce::TextEditor propertyPanel;
    
    // Data
    struct ProjectData
    {
        int canvasWidth = 100;
        int canvasHeight = 100;
        int frameCount = 64;
        float minAngle = -135.0f;
        float maxAngle = 135.0f;
        int tickCount = 11;
        juce::String pointerStyle = "Line";
        bool gridEnabled = true;
        int gridSize = 10;
        bool snapEnabled = true;
        float zoom = 1.0f;
        
        juce::String toJSON() const
        {
            juce::DynamicObject::Ptr root = new juce::DynamicObject();
            root->setProperty("canvasWidth", canvasWidth);
            root->setProperty("canvasHeight", canvasHeight);
            root->setProperty("frameCount", frameCount);
            root->setProperty("minAngle", minAngle);
            root->setProperty("maxAngle", maxAngle);
            root->setProperty("tickCount", tickCount);
            root->setProperty("pointerStyle", pointerStyle);
            root->setProperty("gridEnabled", gridEnabled);
            root->setProperty("gridSize", gridSize);
            root->setProperty("snapEnabled", snapEnabled);
            root->setProperty("zoom", zoom);
            return juce::JSON::toString(juce::var(root.get()), true);
        }
        
        bool fromJSON(const juce::String& json)
        {
            auto result = juce::JSON::parse(json);
            if (!result.isObject()) return false;
            auto* obj = result.getDynamicObject();
            if (!obj) return false;
            
            canvasWidth = obj->getProperty("canvasWidth");
            canvasHeight = obj->getProperty("canvasHeight");
            frameCount = obj->getProperty("frameCount");
            minAngle = obj->getProperty("minAngle");
            maxAngle = obj->getProperty("maxAngle");
            tickCount = obj->getProperty("tickCount");
            pointerStyle = obj->getProperty("pointerStyle").toString();
            gridEnabled = obj->getProperty("gridEnabled");
            gridSize = obj->getProperty("gridSize");
            snapEnabled = obj->getProperty("snapEnabled");
            zoom = obj->getProperty("zoom");
            return true;
        }
    } projectData;
    
    juce::File currentProjectFile;
    juce::UndoManager undoManager;
    std::unique_ptr<juce::FileChooser> fileChooser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
