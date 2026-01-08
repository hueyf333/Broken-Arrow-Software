#ifndef CONTROLITEM_H
#define CONTROLITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "ProjectModel.h"

class ControlItem : public QGraphicsRectItem {
public:
    explicit ControlItem(const ControlDefinition& def, QGraphicsItem *parent = nullptr);
    
    void setDefinition(const ControlDefinition& def);
    ControlDefinition getDefinition() const { return m_definition; }
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void updateFromDefinition();
    
    ControlDefinition m_definition;
    bool m_dragging = false;
    QPointF m_dragStart;
};

#endif // CONTROLITEM_H
