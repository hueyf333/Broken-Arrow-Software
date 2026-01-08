#include "PreviewWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget(parent)
    , m_animationTimer(new QTimer(this))
{
    createUI();
    
    connect(m_animationTimer, &QTimer::timeout, [this]() {
        if (m_currentFrame < m_frames.size() - 1) {
            setCurrentFrame(m_currentFrame + 1);
        } else {
            setCurrentFrame(0);
        }
    });
}

void PreviewWidget::createUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_displayLabel = new QLabel;
    m_displayLabel->setMinimumSize(400, 400);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setFrameStyle(QFrame::Box);
    
    m_modeCombo = new QComboBox;
    m_modeCombo->addItems({"Knob Frames", "Layout"});
    
    m_frameSlider = new QSlider(Qt::Horizontal);
    m_frameSlider->setRange(0, 0);
    
    m_playButton = new QPushButton("Play");
    m_stopButton = new QPushButton("Stop");
    m_stopButton->setEnabled(false);
    
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_modeCombo);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_stopButton);
    
    layout->addWidget(m_displayLabel, 1);
    layout->addWidget(m_frameSlider);
    layout->addLayout(controlLayout);
    
    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PreviewWidget::setPreviewMode);
    connect(m_frameSlider, &QSlider::valueChanged,
            this, &PreviewWidget::setCurrentFrame);
    connect(m_playButton, &QPushButton::clicked, this, &PreviewWidget::playAnimation);
    connect(m_stopButton, &QPushButton::clicked, this, &PreviewWidget::stopAnimation);
}

void PreviewWidget::setFrames(const QList<QImage>& frames) {
    m_frames = frames;
    m_frameSlider->setRange(0, qMax(0, frames.size() - 1));
    m_currentFrame = 0;
    updateDisplay();
}

void PreviewWidget::setLayoutImage(const QImage& image) {
    m_layoutImage = image;
    if (m_previewMode == 1) {
        updateDisplay();
    }
}

void PreviewWidget::setPreviewMode(int mode) {
    m_previewMode = mode;
    updateDisplay();
}

void PreviewWidget::setCurrentFrame(int frame) {
    m_currentFrame = qBound(0, frame, m_frames.size() - 1);
    m_frameSlider->setValue(m_currentFrame);
    updateDisplay();
    emit frameChanged(m_currentFrame);
}

void PreviewWidget::playAnimation() {
    m_animationTimer->start(33); // ~30 FPS
    m_playButton->setEnabled(false);
    m_stopButton->setEnabled(true);
}

void PreviewWidget::stopAnimation() {
    m_animationTimer->stop();
    m_playButton->setEnabled(true);
    m_stopButton->setEnabled(false);
}

void PreviewWidget::updateDisplay() {
    QImage displayImage;
    
    if (m_previewMode == 0 && !m_frames.isEmpty() && m_currentFrame < m_frames.size()) {
        displayImage = m_frames[m_currentFrame];
    } else if (m_previewMode == 1 && !m_layoutImage.isNull()) {
        displayImage = m_layoutImage;
    }
    
    if (!displayImage.isNull()) {
        m_displayLabel->setPixmap(QPixmap::fromImage(displayImage).scaled(
            m_displayLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
