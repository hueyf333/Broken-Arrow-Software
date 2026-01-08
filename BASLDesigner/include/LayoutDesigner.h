#ifndef LAYOUTDESIGNER_H
#define LAYOUTDESIGNER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include "ProjectModel.h"
#include "ControlItem.h"

class LayoutDesigner : public QWidget {
    Q_OBJECT

public:
    explicit LayoutDesigner(QWidget *parent = nullptr);
    
    void setParameters(const LayoutParameters& params);
    LayoutParameters getParameters() const;
    
    void addControl(const ControlDefinition& control);
    void deleteSelected();
    void duplicateSelected();
    void selectAll();
    QList<ControlDefinition> getSelectedControls() const;
    
    void setZoom(double factor);
    double getZoom() const { return m_zoomFactor; }
    
    void setGridVisible(bool visible);
    void setSnapToGrid(bool snap);
    void setGuidesVisible(bool visible);

signals:
    void parametersChanged();
    void selectionChanged();

private slots:
    void onSceneSelectionChanged();

private:
    void createUI();
    void drawBackground();
    
    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
    LayoutParameters m_params;
    double m_zoomFactor = 1.0;
    bool m_gridVisible = true;
    bool m_snapToGrid = true;
    bool m_guidesVisible = true;
    
    QList<ControlItem*> m_controlItems;
};

#endif // LAYOUTDESIGNER_H
