#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>
#include <QList>
#include "ProjectModel.h"

class LayerManager : public QWidget {
    Q_OBJECT

public:
    explicit LayerManager(QWidget *parent = nullptr);
    
    void setLayers(const QList<Layer>& layers);
    QList<Layer> getLayers() const { return m_layers; }
    
signals:
    void layersChanged();

private slots:
    void onAddLayer();
    void onRemoveLayer();
    void onMoveLayerUp();
    void onMoveLayerDown();
    void onLayerDoubleClicked(QListWidgetItem *item);
    void onEditLayer();

private:
    void createUI();
    void updateList();
    
    QListWidget *m_listWidget;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QToolButton *m_upButton;
    QToolButton *m_downButton;
    QPushButton *m_editButton;
    
    QList<Layer> m_layers;
};

#endif // LAYERMANAGER_H
