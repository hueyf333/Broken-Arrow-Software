#ifndef PROPERTYINSPECTOR_H
#define PROPERTYINSPECTOR_H

#include <QWidget>
#include <QTableWidget>
#include <QVariant>
#include <QMap>

class PropertyInspector : public QWidget {
    Q_OBJECT

public:
    explicit PropertyInspector(QWidget *parent = nullptr);
    
    void setProperties(const QMap<QString, QVariant>& properties);
    void clear();
    
signals:
    void propertyChanged(const QString& name, const QVariant& value);

private slots:
    void onItemChanged(QTableWidgetItem *item);

private:
    QTableWidget *m_table;
    bool m_updating = false;
};

#endif // PROPERTYINSPECTOR_H
