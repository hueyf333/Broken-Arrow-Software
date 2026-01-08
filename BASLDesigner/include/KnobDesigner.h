#ifndef KNOBDESIGNER_H
#define KNOBDESIGNER_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <memory>
#include "ProjectModel.h"
#include "KnobRenderer.h"
#include "LayerManager.h"

class KnobDesigner : public QWidget {
    Q_OBJECT

public:
    explicit KnobDesigner(QWidget *parent = nullptr);
    
    void setParameters(const KnobParameters& params);
    KnobParameters getParameters() const { return m_params; }
    
    QImage getCurrentFrame() const;
    
signals:
    void parametersChanged();
    void frameChanged(int frameIndex);

private slots:
    void onParameterChanged();
    void onPreviewDragged(QPoint delta);
    void onLayersChanged();
    void updatePreview();

private:
    void createUI();
    void updateControls();
    
    // Parameters
    KnobParameters m_params;
    
    // Renderer
    std::unique_ptr<KnobRenderer> m_renderer;
    
    // UI Components
    QSpinBox *m_canvasWidthSpin;
    QSpinBox *m_canvasHeightSpin;
    QSpinBox *m_frameCountSpin;
    QSpinBox *m_startAngleSpin;
    QSpinBox *m_endAngleSpin;
    QSpinBox *m_innerRadiusSpin;
    QSpinBox *m_outerRadiusSpin;
    QSpinBox *m_tickCountSpin;
    QComboBox *m_tickStyleCombo;
    QComboBox *m_pointerStyleCombo;
    QCheckBox *m_shadowCheckBox;
    QSpinBox *m_shadowBlurSpin;
    QSlider *m_shadowOpacitySlider;
    
    // Layer management
    LayerManager *m_layerManager;
    
    // Preview
    class PreviewLabel : public QLabel {
    public:
        PreviewLabel(QWidget *parent = nullptr);
        int currentFrame() const { return m_frame; }
        void setCurrentFrame(int frame) { m_frame = frame; }
        
    signals:
        void dragged(QPoint delta);
        
    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        
    private:
        QPoint m_lastPos;
        int m_frame = 0;
    };
    
    PreviewLabel *m_previewLabel;
    QLabel *m_frameInfoLabel;
    int m_currentFrame = 0;
};

#endif // KNOBDESIGNER_H
