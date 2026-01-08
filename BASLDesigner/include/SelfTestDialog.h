#ifndef SELFTESTDIALOG_H
#define SELFTESTDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QList>

class SelfTestDialog : public QDialog {
    Q_OBJECT

public:
    explicit SelfTestDialog(QWidget *parent = nullptr);
    
public slots:
    void runTests();

private slots:
    void onTestCompleted();

private:
    void createUI();
    void logMessage(const QString& message, bool success = true);
    bool testKnobGeneration();
    bool testLayoutCreation();
    bool testExport();
    bool testFileVerification();
    
    QTextEdit *m_logText;
    QProgressBar *m_progressBar;
    QPushButton *m_runButton;
    QPushButton *m_closeButton;
    
    int m_testsCompleted = 0;
    int m_totalTests = 4;
    bool m_allTestsPassed = true;
};

#endif // SELFTESTDIALOG_H
