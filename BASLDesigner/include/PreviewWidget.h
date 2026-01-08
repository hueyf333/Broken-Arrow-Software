#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QPixmap>
#include <QList>
#include "ProjectModel.h"

class PreviewWidget : public QWidget {
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    
    void setFrames(const QList<QImage>& frames);
    void setLayoutImage(const QImage& image);
    
public slots:
    void setPreviewMode(int mode); // 0 = Knob, 1 = Layout
    void setCurrentFrame(int frame);
    void playAnimation();
    void stopAnimation();

signals:
    void frameChanged(int frame);

private:
    void createUI();
    void updateDisplay();
    
    QLabel *m_displayLabel;
    QSlider *m_frameSlider;
    QComboBox *m_modeCombo;
    QPushButton *m_playButton;
    QPushButton *m_stopButton;
    
    QList<QImage> m_frames;
    QImage m_layoutImage;
    int m_currentFrame = 0;
    int m_previewMode = 0;
    
    QTimer *m_animationTimer;
};

#endif // PREVIEWWIDGET_H
