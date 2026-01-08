#include "RecentFilesManager.h"
#include <QFileInfo>

RecentFilesManager::RecentFilesManager()
    : m_settings("BrokenArrowSoftware", "BASLDesigner")
{
    loadRecentFiles();
}

void RecentFilesManager::addRecentFile(const QString& filePath) {
    m_recentFiles.removeAll(filePath);
    m_recentFiles.prepend(filePath);
    
    while (m_recentFiles.size() > MaxRecentFiles) {
        m_recentFiles.removeLast();
    }
    
    saveRecentFiles();
}

QStringList RecentFilesManager::getRecentFiles() const {
    QStringList validFiles;
    for (const QString& file : m_recentFiles) {
        if (QFileInfo::exists(file)) {
            validFiles.append(file);
        }
    }
    return validFiles;
}

void RecentFilesManager::clearRecentFiles() {
    m_recentFiles.clear();
    saveRecentFiles();
}

void RecentFilesManager::saveRecentFiles() {
    m_settings.setValue("recentFiles", m_recentFiles);
}

void RecentFilesManager::loadRecentFiles() {
    m_recentFiles = m_settings.value("recentFiles").toStringList();
}
