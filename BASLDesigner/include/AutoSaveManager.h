#ifndef AUTOSAVEMANAGER_H
#define AUTOSAVEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <functional>

class AutoSaveManager : public QObject {
    Q_OBJECT

public:
    explicit AutoSaveManager(QObject *parent = nullptr);
    
    void setAutoSaveInterval(int minutes);
    void setSaveCallback(std::function<bool(const QString&)> callback);
    void enable(bool enabled);
    
    bool hasRecoveryFile() const;
    QString getRecoveryFilePath() const;
    void removeRecoveryFile();

private slots:
    void onAutoSave();

private:
    QString getAutoSaveFilePath() const;
    
    QTimer *m_timer;
    std::function<bool(const QString&)> m_saveCallback;
    int m_intervalMinutes = 5;
    bool m_enabled = true;
};

#endif // AUTOSAVEMANAGER_H
