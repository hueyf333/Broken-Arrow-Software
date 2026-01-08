#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QImage>

class ImportDialog : public QDialog {
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = nullptr);
    
    QString getFilePath() const;
    int getFrameCount() const;
    bool isHorizontal() const;

private slots:
    void onBrowse();
    void onParametersChanged();
    void updatePreview();

private:
    void createUI();
    
    QLineEdit *m_pathEdit;
    QPushButton *m_browseButton;
    QSpinBox *m_frameCountSpin;
    QComboBox *m_orientationCombo;
    QLabel *m_previewLabel;
    
    QImage m_sourceImage;
};

#endif // IMPORTDIALOG_H
