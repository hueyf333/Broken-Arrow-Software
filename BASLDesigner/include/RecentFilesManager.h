#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QString>
#include <QStringList>
#include <QSettings>

class RecentFilesManager {
public:
    RecentFilesManager();
    
    void addRecentFile(const QString& filePath);
    QStringList getRecentFiles() const;
    void clearRecentFiles();
    
    static constexpr int MaxRecentFiles = 10;

private:
    void saveRecentFiles();
    void loadRecentFiles();
    
    QStringList m_recentFiles;
    QSettings m_settings;
};

#endif // RECENTFILESMANAGER_H
