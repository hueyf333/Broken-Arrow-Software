#include "AssetBrowser.h"
#include <QVBoxLayout>
#include <QDir>

AssetBrowser::AssetBrowser(QWidget *parent)
    : QWidget(parent)
    , m_watcher(new QFileSystemWatcher(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_listWidget = new QListWidget;
    layout->addWidget(m_listWidget);
    
    connect(m_listWidget, &QListWidget::itemClicked,
            this, &AssetBrowser::onItemClicked);
    connect(m_listWidget, &QListWidget::itemDoubleClicked,
            this, &AssetBrowser::onItemDoubleClicked);
}

void AssetBrowser::setProjectPath(const QString& path) {
    m_projectPath = path;
    loadAssets();
    
    if (!path.isEmpty()) {
        m_watcher->addPath(path);
    }
}

void AssetBrowser::refresh() {
    loadAssets();
}

void AssetBrowser::loadAssets() {
    m_listWidget->clear();
    
    if (m_projectPath.isEmpty()) return;
    
    QDir dir(m_projectPath);
    QStringList filters = {"*.png", "*.jpg", "*.jpeg", "*.bmp"};
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    
    for (const QFileInfo& fileInfo : files) {
        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
        item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
        m_listWidget->addItem(item);
    }
}

void AssetBrowser::onItemClicked(QListWidgetItem *item) {
    QString path = item->data(Qt::UserRole).toString();
    emit assetSelected(path);
}

void AssetBrowser::onItemDoubleClicked(QListWidgetItem *item) {
    QString path = item->data(Qt::UserRole).toString();
    emit assetDoubleClicked(path);
}
