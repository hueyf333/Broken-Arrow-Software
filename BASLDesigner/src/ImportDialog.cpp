#include "ImportDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFileDialog>

ImportDialog::ImportDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Import Filmstrip");
    createUI();
}

void ImportDialog::createUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout;
    
    QHBoxLayout *pathLayout = new QHBoxLayout;
    m_pathEdit = new QLineEdit;
    m_browseButton = new QPushButton("Browse...");
    pathLayout->addWidget(m_pathEdit);
    pathLayout->addWidget(m_browseButton);
    formLayout->addRow("File:", pathLayout);
    
    m_frameCountSpin = new QSpinBox;
    m_frameCountSpin->setRange(1, 256);
    m_frameCountSpin->setValue(64);
    formLayout->addRow("Frame Count:", m_frameCountSpin);
    
    m_orientationCombo = new QComboBox;
    m_orientationCombo->addItems({"Horizontal", "Vertical"});
    formLayout->addRow("Orientation:", m_orientationCombo);
    
    layout->addLayout(formLayout);
    
    m_previewLabel = new QLabel;
    m_previewLabel->setMinimumSize(200, 200);
    m_previewLabel->setFrameStyle(QFrame::Box);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_previewLabel);
    
    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);
    
    connect(m_browseButton, &QPushButton::clicked, this, &ImportDialog::onBrowse);
    connect(m_frameCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ImportDialog::onParametersChanged);
    connect(m_orientationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImportDialog::onParametersChanged);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString ImportDialog::getFilePath() const {
    return m_pathEdit->text();
}

int ImportDialog::getFrameCount() const {
    return m_frameCountSpin->value();
}

bool ImportDialog::isHorizontal() const {
    return m_orientationCombo->currentIndex() == 0;
}

void ImportDialog::onBrowse() {
    QString filter = "Images (*.png *.jpg *.jpeg *.bmp)";
    QString path = QFileDialog::getOpenFileName(this, "Select Image", "", filter);
    if (!path.isEmpty()) {
        m_pathEdit->setText(path);
        m_sourceImage = QImage(path);
        updatePreview();
    }
}

void ImportDialog::onParametersChanged() {
    updatePreview();
}

void ImportDialog::updatePreview() {
    if (m_sourceImage.isNull()) return;
    
    // Show preview of first frame
    int frameWidth, frameHeight;
    if (isHorizontal()) {
        frameWidth = m_sourceImage.width() / m_frameCountSpin->value();
        frameHeight = m_sourceImage.height();
    } else {
        frameWidth = m_sourceImage.width();
        frameHeight = m_sourceImage.height() / m_frameCountSpin->value();
    }
    
    QImage preview = m_sourceImage.copy(0, 0, frameWidth, frameHeight);
    m_previewLabel->setPixmap(QPixmap::fromImage(preview).scaled(
        m_previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
