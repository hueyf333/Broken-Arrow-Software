#include "ControlItem.h"
#include <QGraphicsSceneMouseEvent>

ControlItem::ControlItem(const ControlDefinition& def, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , m_definition(def)
{
    updateFromDefinition();
    setFlags(QGraphicsItem::ItemIsSelectable | 
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsGeometryChanges);
}

void ControlItem::setDefinition(const ControlDefinition& def) {
    m_definition = def;
    updateFromDefinition();
}

void ControlItem::updateFromDefinition() {
    setRect(m_definition.bounds);
    
    // Set color based on control type
    QColor color;
    switch (m_definition.type) {
        case ControlType::Knob:
            color = QColor(100, 150, 200);
            break;
        case ControlType::SliderHorizontal:
        case ControlType::SliderVertical:
            color = QColor(150, 100, 200);
            break;
        case ControlType::ButtonToggle:
        case ControlType::ButtonMomentary:
            color = QColor(200, 150, 100);
            break;
        case ControlType::SwitchMultiState:
            color = QColor(100, 200, 150);
            break;
        case ControlType::MeterSegmented:
            color = QColor(200, 100, 150);
            break;
    }
    
    setBrush(color);
    setPen(QPen(Qt::black, 2));
}

void ControlItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsRectItem::paint(painter, option, widget);
    
    // Draw label
    painter->setPen(Qt::white);
    painter->drawText(rect(), Qt::AlignCenter, m_definition.name);
}

QVariant ControlItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        m_definition.bounds.moveTo(pos());
    }
    return QGraphicsRectItem::itemChange(change, value);
}

void ControlItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_dragging = true;
    m_dragStart = event->pos();
    QGraphicsRectItem::mousePressEvent(event);
}

void ControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsRectItem::mouseMoveEvent(event);
}

void ControlItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_dragging = false;
    QGraphicsRectItem::mouseReleaseEvent(event);
}
