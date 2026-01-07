#include "CanvasComponent.h"
#include "LayerRenderer.h"

namespace KnobSmith
{

CanvasComponent::CanvasComponent(Project& proj) : project(proj)
{
    setOpaque(true);
}

void CanvasComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(juce::Colour(0xff2d2d2d));
    
    // Calculate canvas position
    auto canvasWidth = static_cast<int>(project.getCanvasWidth() * zoom);
    auto canvasHeight = static_cast<int>(project.getCanvasHeight() * zoom);
    
    juce::Rectangle<int> canvasBounds(
        static_cast<int>(getWidth() / 2 + offset.x - canvasWidth / 2),
        static_cast<int>(getHeight() / 2 + offset.y - canvasHeight / 2),
        canvasWidth,
        canvasHeight
    );
    
    // Draw checkerboard background
    g.setColour(juce::Colours::white);
    g.fillRect(canvasBounds);
    g.setColour(juce::Colour(0xffcccccc));
    
    int checkSize = 8;
    for (int y = 0; y < canvasBounds.getHeight(); y += checkSize)
    {
        for (int x = 0; x < canvasBounds.getWidth(); x += checkSize)
        {
            if ((x / checkSize + y / checkSize) % 2 == 0)
            {
                g.fillRect(canvasBounds.getX() + x, canvasBounds.getY() + y, checkSize, checkSize);
            }
        }
    }
    
    // Render all layers
    auto layers = project.getLayers();
    auto composited = LayerRenderer::renderLayers(layers, project.getCanvasWidth(), 
                                                  project.getCanvasHeight(), 
                                                  project.getBackgroundColour());
    
    if (composited.isValid())
    {
        g.drawImage(composited, canvasBounds.toFloat());
    }
    
    // Draw grid overlay
    if (grid.isGridVisible())
    {
        juce::Graphics::ScopedSaveState state(g);
        g.reduceClipRegion(canvasBounds);
        g.addTransform(juce::AffineTransform::translation(static_cast<float>(canvasBounds.getX()), 
                                                         static_cast<float>(canvasBounds.getY()))
                      .scaled(zoom));
        grid.paint(g, juce::Rectangle<int>(0, 0, project.getCanvasWidth(), project.getCanvasHeight()));
    }
    
    // Draw border around canvas
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    g.drawRect(canvasBounds, 1);
}

void CanvasComponent::resized()
{
    repaint();
}

juce::Point<int> CanvasComponent::screenToCanvas(juce::Point<int> point) const
{
    auto canvasWidth = static_cast<int>(project.getCanvasWidth() * zoom);
    auto canvasHeight = static_cast<int>(project.getCanvasHeight() * zoom);
    
    int canvasX = static_cast<int>(getWidth() / 2 + offset.x - canvasWidth / 2);
    int canvasY = static_cast<int>(getHeight() / 2 + offset.y - canvasHeight / 2);
    
    int x = static_cast<int>((point.x - canvasX) / zoom);
    int y = static_cast<int>((point.y - canvasY) / zoom);
    
    return {x, y};
}

juce::Point<int> CanvasComponent::canvasToScreen(juce::Point<int> point) const
{
    auto canvasWidth = static_cast<int>(project.getCanvasWidth() * zoom);
    auto canvasHeight = static_cast<int>(project.getCanvasHeight() * zoom);
    
    int canvasX = static_cast<int>(getWidth() / 2 + offset.x - canvasWidth / 2);
    int canvasY = static_cast<int>(getHeight() / 2 + offset.y - canvasHeight / 2);
    
    int x = static_cast<int>(point.x * zoom) + canvasX;
    int y = static_cast<int>(point.y * zoom) + canvasY;
    
    return {x, y};
}

juce::MouseEvent CanvasComponent::transformMouseEvent(const juce::MouseEvent& e, juce::Point<int> canvasPos) const
{
    return juce::MouseEvent(e.source, e.position, e.mods, e.pressure,
                           e.orientation, e.rotation, e.tiltX, e.tiltY,
                           e.eventComponent, e.originalComponent, e.eventTime,
                           juce::Point<float>(canvasPos.x, canvasPos.y),
                           e.eventTime, 1, false);
}

void CanvasComponent::mouseDown(const juce::MouseEvent& e)
{
    if (e.mods.isMiddleButtonDown() || (e.mods.isLeftButtonDown() && e.mods.isCommandDown()))
    {
        isPanning = true;
        panStart = e.getPosition();
        offsetStart = offset;
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        return;
    }
    
    if (activeTool && activeLayer && e.mods.isLeftButtonDown())
    {
        auto canvasPos = screenToCanvas(e.getPosition());
        auto transformedEvent = transformMouseEvent(e, canvasPos);
        activeTool->mouseDown(transformedEvent, activeLayer->getImage());
        repaint();
    }
}

void CanvasComponent::mouseDrag(const juce::MouseEvent& e)
{
    if (isPanning)
    {
        auto delta = e.getPosition() - panStart;
        offset = offsetStart + juce::Point<float>(delta.x, delta.y);
        repaint();
        return;
    }
    
    if (activeTool && activeLayer && e.mods.isLeftButtonDown())
    {
        auto canvasPos = screenToCanvas(e.getPosition());
        auto transformedEvent = transformMouseEvent(e, canvasPos);
        activeTool->mouseDrag(transformedEvent, activeLayer->getImage());
        repaint();
    }
    
    if (onStatusUpdate)
    {
        auto canvasPos = screenToCanvas(e.getPosition());
        onStatusUpdate(canvasPos, "Cursor: " + juce::String(canvasPos.x) + ", " + juce::String(canvasPos.y));
    }
}

void CanvasComponent::mouseUp(const juce::MouseEvent& e)
{
    if (isPanning)
    {
        isPanning = false;
        setMouseCursor(juce::MouseCursor::NormalCursor);
        return;
    }
    
    if (activeTool && activeLayer)
    {
        auto canvasPos = screenToCanvas(e.getPosition());
        auto transformedEvent = transformMouseEvent(e, canvasPos);
        activeTool->mouseUp(transformedEvent, activeLayer->getImage());
        repaint();
    }
}

void CanvasComponent::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
    if (e.mods.isCommandDown() || e.mods.isCtrlDown())
    {
        float factor = wheel.deltaY > 0 ? 1.1f : 0.9f;
        setZoom(zoom * factor);
    }
}

void CanvasComponent::setZoom(float newZoom)
{
    zoom = juce::jlimit(0.1f, 10.0f, newZoom);
    repaint();
}

void CanvasComponent::fitToScreen()
{
    float scaleX = getWidth() * 0.9f / project.getCanvasWidth();
    float scaleY = getHeight() * 0.9f / project.getCanvasHeight();
    zoom = juce::jmin(scaleX, scaleY);
    offset = {0.0f, 0.0f};
    repaint();
}

} // namespace KnobSmith
