#include "LayoutDesigner.h"
#include <QVBoxLayout>
#include <QGraphicsRectItem>

LayoutDesigner::LayoutDesigner(QWidget *parent)
    : QWidget(parent)
{
    createUI();
}

void LayoutDesigner::createUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, m_params.width, m_params.height);
    
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    
    layout->addWidget(m_view);
    
    connect(m_scene, &QGraphicsScene::selectionChanged,
            this, &LayoutDesigner::onSceneSelectionChanged);
    
    drawBackground();
}

void LayoutDesigner::drawBackground() {
    if (!m_gridVisible) return;
    
    QPen gridPen(QColor(200, 200, 200), 0.5);
    for (int x = 0; x < m_params.width; x += m_params.gridSize) {
        m_scene->addLine(x, 0, x, m_params.height, gridPen);
    }
    for (int y = 0; y < m_params.height; y += m_params.gridSize) {
        m_scene->addLine(0, y, m_params.width, y, gridPen);
    }
}

void LayoutDesigner::setParameters(const LayoutParameters& params) {
    m_params = params;
    m_scene->setSceneRect(0, 0, params.width, params.height);
    
    // Clear and recreate controls
    m_scene->clear();
    m_controlItems.clear();
    drawBackground();
    
    for (const auto& control : params.controls) {
        addControl(control);
    }
}

LayoutParameters LayoutDesigner::getParameters() const {
    LayoutParameters params = m_params;
    params.controls.clear();
    
    for (auto* item : m_controlItems) {
        params.controls.append(item->getDefinition());
    }
    
    return params;
}

void LayoutDesigner::addControl(const ControlDefinition& control) {
    ControlItem *item = new ControlItem(control);
    m_scene->addItem(item);
    m_controlItems.append(item);
}

void LayoutDesigner::deleteSelected() {
    auto selectedItems = m_scene->selectedItems();
    for (auto* item : selectedItems) {
        ControlItem *controlItem = dynamic_cast<ControlItem*>(item);
        if (controlItem) {
            m_controlItems.removeOne(controlItem);
            delete controlItem;
        }
    }
    emit parametersChanged();
}

void LayoutDesigner::duplicateSelected() {
    auto selectedItems = m_scene->selectedItems();
    for (auto* item : selectedItems) {
        ControlItem *controlItem = dynamic_cast<ControlItem*>(item);
        if (controlItem) {
            ControlDefinition def = controlItem->getDefinition();
            def.bounds.translate(10, 10);
            addControl(def);
        }
    }
    emit parametersChanged();
}

void LayoutDesigner::selectAll() {
    for (auto* item : m_controlItems) {
        item->setSelected(true);
    }
}

QList<ControlDefinition> LayoutDesigner::getSelectedControls() const {
    QList<ControlDefinition> controls;
    auto selectedItems = m_scene->selectedItems();
    for (auto* item : selectedItems) {
        ControlItem *controlItem = dynamic_cast<ControlItem*>(item);
        if (controlItem) {
            controls.append(controlItem->getDefinition());
        }
    }
    return controls;
}

void LayoutDesigner::setZoom(double factor) {
    m_zoomFactor = qBound(0.1, factor, 5.0);
    m_view->resetTransform();
    m_view->scale(m_zoomFactor, m_zoomFactor);
}

void LayoutDesigner::setGridVisible(bool visible) {
    m_gridVisible = visible;
    m_scene->clear();
    m_controlItems.clear();
    drawBackground();
    
    for (const auto& control : m_params.controls) {
        addControl(control);
    }
}

void LayoutDesigner::setSnapToGrid(bool snap) {
    m_snapToGrid = snap;
}

void LayoutDesigner::setGuidesVisible(bool visible) {
    m_guidesVisible = visible;
}

void LayoutDesigner::onSceneSelectionChanged() {
    emit selectionChanged();
}
