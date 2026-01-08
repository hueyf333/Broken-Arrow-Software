#ifndef ASSETBROWSER_H
#define ASSETBROWSER_H

#include <QWidget>
#include <QListWidget>
#include <QFileSystemWatcher>
#include <QString>

class AssetBrowser : public QWidget {
    Q_OBJECT

public:
    explicit AssetBrowser(QWidget *parent = nullptr);
    
    void setProjectPath(const QString& path);
    void refresh();
    
signals:
    void assetSelected(const QString& path);
    void assetDoubleClicked(const QString& path);

private slots:
    void onItemClicked(QListWidgetItem *item);
    void onItemDoubleClicked(QListWidgetItem *item);

private:
    void loadAssets();
    
    QListWidget *m_listWidget;
    QString m_projectPath;
    QFileSystemWatcher *m_watcher;
};

#endif // ASSETBROWSER_H
