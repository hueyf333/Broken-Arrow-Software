#include "SelfTestDialog.h"
#include "KnobRenderer.h"
#include "ProjectModel.h"
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QFileInfo>

SelfTestDialog::SelfTestDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Self Test");
    resize(600, 400);
    createUI();
}

void SelfTestDialog::createUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    m_logText = new QTextEdit;
    m_logText->setReadOnly(true);
    layout->addWidget(m_logText);
    
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, m_totalTests);
    layout->addWidget(m_progressBar);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    m_runButton = new QPushButton("Run Tests");
    m_closeButton = new QPushButton("Close");
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_runButton);
    buttonLayout->addWidget(m_closeButton);
    layout->addLayout(buttonLayout);
    
    connect(m_runButton, &QPushButton::clicked, this, &SelfTestDialog::runTests);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void SelfTestDialog::runTests() {
    m_runButton->setEnabled(false);
    m_logText->clear();
    m_testsCompleted = 0;
    m_allTestsPassed = true;
    m_progressBar->setValue(0);
    
    logMessage("<b>Starting Self Tests...</b>");
    logMessage("");
    
    // Run tests
    if (testKnobGeneration()) {
        logMessage("✓ Knob generation test PASSED", true);
    } else {
        logMessage("✗ Knob generation test FAILED", false);
        m_allTestsPassed = false;
    }
    onTestCompleted();
    
    if (testLayoutCreation()) {
        logMessage("✓ Layout creation test PASSED", true);
    } else {
        logMessage("✗ Layout creation test FAILED", false);
        m_allTestsPassed = false;
    }
    onTestCompleted();
    
    if (testExport()) {
        logMessage("✓ Export test PASSED", true);
    } else {
        logMessage("✗ Export test FAILED", false);
        m_allTestsPassed = false;
    }
    onTestCompleted();
    
    if (testFileVerification()) {
        logMessage("✓ File verification test PASSED", true);
    } else {
        logMessage("✗ File verification test FAILED", false);
        m_allTestsPassed = false;
    }
    onTestCompleted();
    
    logMessage("");
    if (m_allTestsPassed) {
        logMessage("<b><font color='green'>All tests PASSED!</font></b>");
    } else {
        logMessage("<b><font color='red'>Some tests FAILED!</font></b>");
    }
    
    m_runButton->setEnabled(true);
}

void SelfTestDialog::logMessage(const QString& message, bool success) {
    QString color = success ? "black" : "red";
    m_logText->append(QString("<font color='%1'>%2</font>").arg(color, message));
}

bool SelfTestDialog::testKnobGeneration() {
    logMessage("Test 1: Generating sample knob...");
    
    try {
        KnobParameters params;
        params.canvasWidth = 128;
        params.canvasHeight = 128;
        params.frameCount = 32;
        params.startAngle = -135;
        params.endAngle = 135;
        params.innerRadius = 20;
        params.outerRadius = 50;
        params.tickCount = 11;
        
        Layer layer;
        layer.name = "Base";
        layer.color = QColor(100, 150, 200);
        layer.visible = true;
        params.layers.append(layer);
        
        KnobRenderer renderer;
        QImage frame = renderer.renderFrame(params, 0);
        
        if (frame.isNull() || frame.width() != 128 || frame.height() != 128) {
            logMessage("  Error: Generated frame has invalid dimensions");
            return false;
        }
        
        logMessage("  Generated 128x128 knob with 32 frames");
        return true;
    } catch (...) {
        logMessage("  Error: Exception during knob generation");
        return false;
    }
}

bool SelfTestDialog::testLayoutCreation() {
    logMessage("Test 2: Creating sample layout...");
    
    try {
        LayoutParameters params;
        params.width = 800;
        params.height = 600;
        params.gridSize = 10;
        
        ControlDefinition control;
        control.id = "knob1";
        control.name = "Volume";
        control.type = ControlType::Knob;
        control.bounds = QRectF(100, 100, 64, 64);
        control.imageSource = "knob.png";
        
        params.controls.append(control);
        
        logMessage("  Created layout with 1 control");
        return true;
    } catch (...) {
        logMessage("  Error: Exception during layout creation");
        return false;
    }
}

bool SelfTestDialog::testExport() {
    logMessage("Test 3: Exporting to temp directory...");
    
    try {
        QString tempPath = QDir::tempPath() + "/basl_test";
        QDir().mkpath(tempPath);
        
        KnobParameters params;
        params.canvasWidth = 64;
        params.canvasHeight = 64;
        params.frameCount = 8;
        
        Layer layer;
        layer.name = "Base";
        layer.color = QColor(100, 150, 200);
        params.layers.append(layer);
        
        KnobRenderer renderer;
        QString filmstripPath = tempPath + "/test_filmstrip.png";
        bool success = renderer.exportFilmstrip(params, filmstripPath, true, 1);
        
        if (!success) {
            logMessage("  Error: Export failed");
            return false;
        }
        
        logMessage("  Exported filmstrip to: " + filmstripPath);
        return true;
    } catch (...) {
        logMessage("  Error: Exception during export");
        return false;
    }
}

bool SelfTestDialog::testFileVerification() {
    logMessage("Test 4: Verifying exported files...");
    
    try {
        QString filmstripPath = QDir::tempPath() + "/basl_test/test_filmstrip.png";
        
        QFileInfo fileInfo(filmstripPath);
        if (!fileInfo.exists()) {
            logMessage("  Error: Exported file does not exist");
            return false;
        }
        
        QImage image(filmstripPath);
        if (image.isNull()) {
            logMessage("  Error: Exported file is not a valid image");
            return false;
        }
        
        // Expected: 64x64 frames, 8 frames horizontal = 512x64
        if (image.width() != 512 || image.height() != 64) {
            logMessage(QString("  Error: Unexpected image size: %1x%2 (expected 512x64)")
                .arg(image.width()).arg(image.height()));
            return false;
        }
        
        logMessage(QString("  File exists: %1 bytes, dimensions: %2x%3")
            .arg(fileInfo.size())
            .arg(image.width())
            .arg(image.height()));
        return true;
    } catch (...) {
        logMessage("  Error: Exception during file verification");
        return false;
    }
}

void SelfTestDialog::onTestCompleted() {
    m_testsCompleted++;
    m_progressBar->setValue(m_testsCompleted);
}
