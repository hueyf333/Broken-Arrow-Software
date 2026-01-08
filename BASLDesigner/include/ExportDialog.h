#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

class ExportDialog : public QDialog {
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    
    enum ExportFormat {
        FilmstripHorizontal,
        FilmstripVertical,
        FrameSequence,
        AtlasJSON
    };
    
    ExportFormat getFormat() const;
    int getScale() const;
    QString getOutputPath() const;

private slots:
    void onBrowse();
    void onFormatChanged();

private:
    void createUI();
    
    QComboBox *m_formatCombo;
    QSpinBox *m_scaleSpin;
    QLineEdit *m_pathEdit;
    QPushButton *m_browseButton;
    
    QRadioButton *m_horizontalRadio;
    QRadioButton *m_verticalRadio;
};

#endif // EXPORTDIALOG_H
