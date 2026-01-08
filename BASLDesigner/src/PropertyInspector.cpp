#include "PropertyInspector.h"
#include <QVBoxLayout>
#include <QHeaderView>

PropertyInspector::PropertyInspector(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_table = new QTableWidget;
    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels({"Property", "Value"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    
    layout->addWidget(m_table);
    
    connect(m_table, &QTableWidget::itemChanged, 
            this, &PropertyInspector::onItemChanged);
}

void PropertyInspector::setProperties(const QMap<QString, QVariant>& properties) {
    m_updating = true;
    m_table->setRowCount(0);
    
    int row = 0;
    for (auto it = properties.begin(); it != properties.end(); ++it) {
        m_table->insertRow(row);
        
        QTableWidgetItem *nameItem = new QTableWidgetItem(it.key());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        m_table->setItem(row, 0, nameItem);
        
        QTableWidgetItem *valueItem = new QTableWidgetItem(it.value().toString());
        m_table->setItem(row, 1, valueItem);
        
        ++row;
    }
    
    m_updating = false;
}

void PropertyInspector::clear() {
    m_table->setRowCount(0);
}

void PropertyInspector::onItemChanged(QTableWidgetItem *item) {
    if (m_updating || item->column() != 1) return;
    
    QString name = m_table->item(item->row(), 0)->text();
    QVariant value = item->text();
    
    emit propertyChanged(name, value);
}
