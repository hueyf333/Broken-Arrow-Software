#include "ExportDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>

ExportDialog::ExportDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Export");
    createUI();
}

void ExportDialog::createUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout;
    
    m_formatCombo = new QComboBox;
    m_formatCombo->addItems({
        "Filmstrip (Horizontal)",
        "Filmstrip (Vertical)",
        "Frame Sequence",
        "Atlas + JSON"
    });
    formLayout->addRow("Format:", m_formatCombo);
    
    m_scaleSpin = new QSpinBox;
    m_scaleSpin->setRange(1, 4);
    m_scaleSpin->setValue(1);
    m_scaleSpin->setSuffix("x");
    formLayout->addRow("Scale:", m_scaleSpin);
    
    QHBoxLayout *pathLayout = new QHBoxLayout;
    m_pathEdit = new QLineEdit;
    m_browseButton = new QPushButton("Browse...");
    pathLayout->addWidget(m_pathEdit);
    pathLayout->addWidget(m_browseButton);
    formLayout->addRow("Output:", pathLayout);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);
    
    connect(m_browseButton, &QPushButton::clicked, this, &ExportDialog::onBrowse);
    connect(m_formatCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ExportDialog::onFormatChanged);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ExportDialog::ExportFormat ExportDialog::getFormat() const {
    return static_cast<ExportFormat>(m_formatCombo->currentIndex());
}

int ExportDialog::getScale() const {
    return m_scaleSpin->value();
}

QString ExportDialog::getOutputPath() const {
    return m_pathEdit->text();
}

void ExportDialog::onBrowse() {
    QString filter;
    if (m_formatCombo->currentIndex() == 2) {
        filter = "Directory";
        QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory");
        if (!dir.isEmpty()) {
            m_pathEdit->setText(dir);
        }
    } else {
        filter = "PNG Files (*.png)";
        QString path = QFileDialog::getSaveFileName(this, "Save Export", "", filter);
        if (!path.isEmpty()) {
            m_pathEdit->setText(path);
        }
    }
}

void ExportDialog::onFormatChanged() {
    // Update UI based on format
}
