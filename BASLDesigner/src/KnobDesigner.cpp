#include "KnobDesigner.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QMouseEvent>

KnobDesigner::PreviewLabel::PreviewLabel(QWidget *parent)
    : QLabel(parent) {
    setMinimumSize(300, 300);
    setAlignment(Qt::AlignCenter);
    setFrameStyle(QFrame::Box);
}

void KnobDesigner::PreviewLabel::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
}

void KnobDesigner::PreviewLabel::mouseMoveEvent(QMouseEvent *event) {
    QPoint delta = event->pos() - m_lastPos;
    m_lastPos = event->pos();
    emit dragged(delta);
}

KnobDesigner::KnobDesigner(QWidget *parent)
    : QWidget(parent)
    , m_renderer(std::make_unique<KnobRenderer>())
{
    createUI();
    updatePreview();
}

void KnobDesigner::createUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    
    // Left side - parameters
    QWidget *paramsWidget = new QWidget;
    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWidget);
    
    // Canvas group
    QGroupBox *canvasGroup = new QGroupBox("Canvas");
    QFormLayout *canvasLayout = new QFormLayout(canvasGroup);
    
    m_canvasWidthSpin = new QSpinBox;
    m_canvasWidthSpin->setRange(64, 2048);
    m_canvasWidthSpin->setValue(m_params.canvasWidth);
    canvasLayout->addRow("Width:", m_canvasWidthSpin);
    
    m_canvasHeightSpin = new QSpinBox;
    m_canvasHeightSpin->setRange(64, 2048);
    m_canvasHeightSpin->setValue(m_params.canvasHeight);
    canvasLayout->addRow("Height:", m_canvasHeightSpin);
    
    m_frameCountSpin = new QSpinBox;
    m_frameCountSpin->setRange(1, 256);
    m_frameCountSpin->setValue(m_params.frameCount);
    canvasLayout->addRow("Frames:", m_frameCountSpin);
    
    paramsLayout->addWidget(canvasGroup);
    
    // Angles group
    QGroupBox *anglesGroup = new QGroupBox("Angles");
    QFormLayout *anglesLayout = new QFormLayout(anglesGroup);
    
    m_startAngleSpin = new QSpinBox;
    m_startAngleSpin->setRange(-360, 360);
    m_startAngleSpin->setValue(m_params.startAngle);
    anglesLayout->addRow("Start:", m_startAngleSpin);
    
    m_endAngleSpin = new QSpinBox;
    m_endAngleSpin->setRange(-360, 360);
    m_endAngleSpin->setValue(m_params.endAngle);
    anglesLayout->addRow("End:", m_endAngleSpin);
    
    paramsLayout->addWidget(anglesGroup);
    
    // Geometry group
    QGroupBox *geomGroup = new QGroupBox("Geometry");
    QFormLayout *geomLayout = new QFormLayout(geomGroup);
    
    m_innerRadiusSpin = new QSpinBox;
    m_innerRadiusSpin->setRange(0, 500);
    m_innerRadiusSpin->setValue(m_params.innerRadius);
    geomLayout->addRow("Inner Radius:", m_innerRadiusSpin);
    
    m_outerRadiusSpin = new QSpinBox;
    m_outerRadiusSpin->setRange(10, 500);
    m_outerRadiusSpin->setValue(m_params.outerRadius);
    geomLayout->addRow("Outer Radius:", m_outerRadiusSpin);
    
    paramsLayout->addWidget(geomGroup);
    
    // Ticks group
    QGroupBox *ticksGroup = new QGroupBox("Ticks");
    QFormLayout *ticksLayout = new QFormLayout(ticksGroup);
    
    m_tickCountSpin = new QSpinBox;
    m_tickCountSpin->setRange(0, 50);
    m_tickCountSpin->setValue(m_params.tickCount);
    ticksLayout->addRow("Count:", m_tickCountSpin);
    
    m_tickStyleCombo = new QComboBox;
    m_tickStyleCombo->addItems({"None", "Line", "Dot"});
    m_tickStyleCombo->setCurrentText(m_params.tickStyle);
    ticksLayout->addRow("Style:", m_tickStyleCombo);
    
    paramsLayout->addWidget(ticksGroup);
    
    // Pointer group
    QGroupBox *pointerGroup = new QGroupBox("Pointer");
    QFormLayout *pointerLayout = new QFormLayout(pointerGroup);
    
    m_pointerStyleCombo = new QComboBox;
    m_pointerStyleCombo->addItems({"Line", "Arrow", "Triangle"});
    m_pointerStyleCombo->setCurrentText(m_params.pointerStyle);
    pointerLayout->addRow("Style:", m_pointerStyleCombo);
    
    paramsLayout->addWidget(pointerGroup);
    
    // Shadow group
    QGroupBox *shadowGroup = new QGroupBox("Shadow");
    QFormLayout *shadowLayout = new QFormLayout(shadowGroup);
    
    m_shadowCheckBox = new QCheckBox("Enable");
    m_shadowCheckBox->setChecked(m_params.enableShadow);
    shadowLayout->addRow(m_shadowCheckBox);
    
    m_shadowBlurSpin = new QSpinBox;
    m_shadowBlurSpin->setRange(0, 50);
    m_shadowBlurSpin->setValue(m_params.shadowBlur);
    shadowLayout->addRow("Blur:", m_shadowBlurSpin);
    
    m_shadowOpacitySlider = new QSlider(Qt::Horizontal);
    m_shadowOpacitySlider->setRange(0, 100);
    m_shadowOpacitySlider->setValue(m_params.shadowOpacity * 100);
    shadowLayout->addRow("Opacity:", m_shadowOpacitySlider);
    
    paramsLayout->addWidget(shadowGroup);
    
    // Layer manager
    m_layerManager = new LayerManager;
    m_layerManager->setLayers(m_params.layers);
    paramsLayout->addWidget(m_layerManager);
    
    paramsLayout->addStretch();
    
    // Right side - preview
    QVBoxLayout *previewLayout = new QVBoxLayout;
    
    m_previewLabel = new PreviewLabel;
    m_frameInfoLabel = new QLabel("Frame: 0 / 0");
    
    previewLayout->addWidget(m_previewLabel, 1);
    previewLayout->addWidget(m_frameInfoLabel);
    
    mainLayout->addWidget(paramsWidget, 0);
    mainLayout->addLayout(previewLayout, 1);
    
    // Connect signals
    connect(m_canvasWidthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_canvasHeightSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_frameCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_startAngleSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_endAngleSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_innerRadiusSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_outerRadiusSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_tickCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_tickStyleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_pointerStyleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_shadowCheckBox, &QCheckBox::toggled,
            this, &KnobDesigner::onParameterChanged);
    connect(m_shadowBlurSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &KnobDesigner::onParameterChanged);
    connect(m_shadowOpacitySlider, &QSlider::valueChanged,
            this, &KnobDesigner::onParameterChanged);
    
    connect(m_layerManager, &LayerManager::layersChanged,
            this, &KnobDesigner::onLayersChanged);
    connect(m_previewLabel, &PreviewLabel::dragged,
            this, &KnobDesigner::onPreviewDragged);
}

void KnobDesigner::setParameters(const KnobParameters& params) {
    m_params = params;
    updateControls();
    m_layerManager->setLayers(m_params.layers);
    updatePreview();
}

void KnobDesigner::updateControls() {
    m_canvasWidthSpin->setValue(m_params.canvasWidth);
    m_canvasHeightSpin->setValue(m_params.canvasHeight);
    m_frameCountSpin->setValue(m_params.frameCount);
    m_startAngleSpin->setValue(m_params.startAngle);
    m_endAngleSpin->setValue(m_params.endAngle);
    m_innerRadiusSpin->setValue(m_params.innerRadius);
    m_outerRadiusSpin->setValue(m_params.outerRadius);
    m_tickCountSpin->setValue(m_params.tickCount);
    m_tickStyleCombo->setCurrentText(m_params.tickStyle);
    m_pointerStyleCombo->setCurrentText(m_params.pointerStyle);
    m_shadowCheckBox->setChecked(m_params.enableShadow);
    m_shadowBlurSpin->setValue(m_params.shadowBlur);
    m_shadowOpacitySlider->setValue(m_params.shadowOpacity * 100);
}

void KnobDesigner::onParameterChanged() {
    m_params.canvasWidth = m_canvasWidthSpin->value();
    m_params.canvasHeight = m_canvasHeightSpin->value();
    m_params.frameCount = m_frameCountSpin->value();
    m_params.startAngle = m_startAngleSpin->value();
    m_params.endAngle = m_endAngleSpin->value();
    m_params.innerRadius = m_innerRadiusSpin->value();
    m_params.outerRadius = m_outerRadiusSpin->value();
    m_params.tickCount = m_tickCountSpin->value();
    m_params.tickStyle = m_tickStyleCombo->currentText();
    m_params.pointerStyle = m_pointerStyleCombo->currentText();
    m_params.enableShadow = m_shadowCheckBox->isChecked();
    m_params.shadowBlur = m_shadowBlurSpin->value();
    m_params.shadowOpacity = m_shadowOpacitySlider->value() / 100.0;
    
    updatePreview();
    emit parametersChanged();
}

void KnobDesigner::onLayersChanged() {
    m_params.layers = m_layerManager->getLayers();
    updatePreview();
    emit parametersChanged();
}

void KnobDesigner::onPreviewDragged(QPoint delta) {
    int sensitivity = 2;
    int frameDelta = delta.x() / sensitivity;
    m_currentFrame = qBound(0, m_currentFrame + frameDelta, m_params.frameCount - 1);
    updatePreview();
}

void KnobDesigner::updatePreview() {
    QImage frame = m_renderer->renderFrame(m_params, m_currentFrame);
    m_previewLabel->setPixmap(QPixmap::fromImage(frame).scaled(
        m_previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    double value = m_params.frameCount > 1 ? 
        static_cast<double>(m_currentFrame) / (m_params.frameCount - 1) : 0.0;
    m_frameInfoLabel->setText(QString("Frame: %1 / %2 (Value: %3)")
        .arg(m_currentFrame)
        .arg(m_params.frameCount - 1)
        .arg(value, 0, 'f', 3));
}

QImage KnobDesigner::getCurrentFrame() const {
    return m_renderer->renderFrame(m_params, m_currentFrame);
}
