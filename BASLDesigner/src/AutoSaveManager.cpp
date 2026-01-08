#include "AutoSaveManager.h"
#include <QDir>
#include <QStandardPaths>
#include <QFile>

AutoSaveManager::AutoSaveManager(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &AutoSaveManager::onAutoSave);
    setAutoSaveInterval(5); // Default 5 minutes
}

void AutoSaveManager::setAutoSaveInterval(int minutes) {
    m_intervalMinutes = minutes;
    if (m_enabled) {
        m_timer->start(minutes * 60 * 1000);
    }
}

void AutoSaveManager::setSaveCallback(std::function<bool(const QString&)> callback) {
    m_saveCallback = callback;
}

void AutoSaveManager::enable(bool enabled) {
    m_enabled = enabled;
    if (enabled) {
        m_timer->start(m_intervalMinutes * 60 * 1000);
    } else {
        m_timer->stop();
    }
}

bool AutoSaveManager::hasRecoveryFile() const {
    return QFile::exists(getRecoveryFilePath());
}

QString AutoSaveManager::getRecoveryFilePath() const {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    return dataPath + "/autosave_recovery.baslproj";
}

void AutoSaveManager::removeRecoveryFile() {
    QFile::remove(getRecoveryFilePath());
}

void AutoSaveManager::onAutoSave() {
    if (m_saveCallback) {
        m_saveCallback(getAutoSaveFilePath());
    }
}

QString AutoSaveManager::getAutoSaveFilePath() const {
    return getRecoveryFilePath();
}
