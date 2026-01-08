#include "LayerManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QInputDialog>

LayerManager::LayerManager(QWidget *parent)
    : QWidget(parent)
{
    createUI();
}

void LayerManager::createUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_listWidget = new QListWidget;
    layout->addWidget(m_listWidget);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    
    m_addButton = new QPushButton("+");
    m_removeButton = new QPushButton("-");
    m_upButton = new QToolButton;
    m_upButton->setText("↑");
    m_downButton = new QToolButton;
    m_downButton->setText("↓");
    m_editButton = new QPushButton("Edit");
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addWidget(m_upButton);
    buttonLayout->addWidget(m_downButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addStretch();
    
    layout->addLayout(buttonLayout);
    
    connect(m_addButton, &QPushButton::clicked, this, &LayerManager::onAddLayer);
    connect(m_removeButton, &QPushButton::clicked, this, &LayerManager::onRemoveLayer);
    connect(m_upButton, &QToolButton::clicked, this, &LayerManager::onMoveLayerUp);
    connect(m_downButton, &QToolButton::clicked, this, &LayerManager::onMoveLayerDown);
    connect(m_editButton, &QPushButton::clicked, this, &LayerManager::onEditLayer);
    connect(m_listWidget, &QListWidget::itemDoubleClicked,
            this, &LayerManager::onLayerDoubleClicked);
}

void LayerManager::setLayers(const QList<Layer>& layers) {
    m_layers = layers;
    updateList();
}

void LayerManager::updateList() {
    m_listWidget->clear();
    for (const auto& layer : m_layers) {
        QString text = QString("%1 %2").arg(layer.visible ? "👁" : "🚫", layer.name);
        m_listWidget->addItem(text);
    }
}

void LayerManager::onAddLayer() {
    Layer layer;
    layer.name = QString("Layer %1").arg(m_layers.size() + 1);
    layer.color = QColor(100, 150, 200);
    layer.order = m_layers.size();
    layer.visible = true;
    
    m_layers.append(layer);
    updateList();
    emit layersChanged();
}

void LayerManager::onRemoveLayer() {
    int row = m_listWidget->currentRow();
    if (row >= 0 && row < m_layers.size()) {
        m_layers.removeAt(row);
        updateList();
        emit layersChanged();
    }
}

void LayerManager::onMoveLayerUp() {
    int row = m_listWidget->currentRow();
    if (row > 0) {
        m_layers.swapItemsAt(row, row - 1);
        m_layers[row].order = row;
        m_layers[row - 1].order = row - 1;
        updateList();
        m_listWidget->setCurrentRow(row - 1);
        emit layersChanged();
    }
}

void LayerManager::onMoveLayerDown() {
    int row = m_listWidget->currentRow();
    if (row >= 0 && row < m_layers.size() - 1) {
        m_layers.swapItemsAt(row, row + 1);
        m_layers[row].order = row;
        m_layers[row + 1].order = row + 1;
        updateList();
        m_listWidget->setCurrentRow(row + 1);
        emit layersChanged();
    }
}

void LayerManager::onLayerDoubleClicked(QListWidgetItem *item) {
    onEditLayer();
}

void LayerManager::onEditLayer() {
    int row = m_listWidget->currentRow();
    if (row < 0 || row >= m_layers.size()) return;
    
    Layer& layer = m_layers[row];
    
    // Simple dialog to edit layer color
    QColor newColor = QColorDialog::getColor(layer.color, this, "Select Layer Color");
    if (newColor.isValid()) {
        layer.color = newColor;
        emit layersChanged();
    }
}
