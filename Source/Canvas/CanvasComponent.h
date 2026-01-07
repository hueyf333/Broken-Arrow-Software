#pragma once

#include <JuceHeader.h>
#include "../Model/Project.h"
#include "../Tools/Tool.h"
#include "GridOverlay.h"

namespace KnobSmith
{

class CanvasComponent : public juce::Component
{
public:
    CanvasComponent(Project& project);
    ~CanvasComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

    void setActiveTool(Tool* tool) { activeTool = tool; }
    Tool* getActiveTool() const { return activeTool; }

    void setActiveLayer(Layer* layer) { activeLayer = layer; }
    Layer* getActiveLayer() const { return activeLayer; }

    void setZoom(float newZoom);
    float getZoom() const { return zoom; }
    
    void zoomIn() { setZoom(zoom * 1.2f); }
    void zoomOut() { setZoom(zoom / 1.2f); }
    void fitToScreen();

    GridOverlay& getGrid() { return grid; }

    std::function<void(juce::Point<int>, juce::String)> onStatusUpdate;

private:
    Project& project;
    Tool* activeTool{nullptr};
    Layer* activeLayer{nullptr};
    
    float zoom{1.0f};
    juce::Point<float> offset{0.0f, 0.0f};
    
    bool isPanning{false};
    juce::Point<int> panStart;
    juce::Point<float> offsetStart;
    
    GridOverlay grid;
    juce::UndoManager undoManager;

    juce::Point<int> canvasToScreen(juce::Point<int> point) const;
    juce::Point<int> screenToCanvas(juce::Point<int> point) const;
    
    juce::MouseEvent transformMouseEvent(const juce::MouseEvent& e, juce::Point<int> canvasPos) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CanvasComponent)
};

} // namespace KnobSmith
