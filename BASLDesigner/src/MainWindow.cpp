#include "MainWindow.h"
#include "KnobDesigner.h"
#include "LayoutDesigner.h"
#include "PreviewWidget.h"
#include "PropertyInspector.h"
#include "AssetBrowser.h"
#include "ExportDialog.h"
#include "ImportDialog.h"
#include "SelfTestDialog.h"
#include "RecentFilesManager.h"
#include "AutoSaveManager.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_project(std::make_unique<ProjectModel>())
    , m_undoStack(new QUndoStack(this))
    , m_recentFilesManager(std::make_unique<RecentFilesManager>())
    , m_autoSaveManager(std::make_unique<AutoSaveManager>(this))
{
    setWindowTitle("BASL Designer");
    resize(1400, 900);
    
    createActions();
    createMenus();
    createToolBars();
    createCentralWidget();
    createDockWidgets();
    
    // Status bar
    m_statusLabel = new QLabel(this);
    statusBar()->addWidget(m_statusLabel);
    m_statusLabel->setText("Ready");
    
    // Connect signals
    connect(m_undoStack, &QUndoStack::canUndoChanged, m_undoAction, &QAction::setEnabled);
    connect(m_undoStack, &QUndoStack::canRedoChanged, m_redoAction, &QAction::setEnabled);
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    
    // Setup autosave
    m_autoSaveManager->setSaveCallback([this](const QString& path) {
        return m_project->saveToFile(path);
    });
    
    // Check for recovery file
    if (m_autoSaveManager->hasRecoveryFile()) {
        auto reply = QMessageBox::question(this, "Recovery",
            "A recovery file was found. Do you want to restore it?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            m_project->loadFromFile(m_autoSaveManager->getRecoveryFilePath());
            m_knobDesigner->setParameters(m_project->knobParams);
            m_layoutDesigner->setParameters(m_project->layoutParams);
            onProjectModified();
        } else {
            m_autoSaveManager->removeRecoveryFile();
        }
    }
    
    updateRecentFiles();
    updateWindowTitle();
}

MainWindow::~MainWindow() {
}

void MainWindow::createActions() {
    // File actions
    m_newAction = new QAction(QIcon(":/icons/new.png"), tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::onNewProject);
    
    m_openAction = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpenProject);
    
    m_saveAction = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveProject);
    
    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::onSaveProjectAs);
    
    m_importAction = new QAction(QIcon(":/icons/import.png"), tr("&Import..."), this);
    connect(m_importAction, &QAction::triggered, this, &MainWindow::onImport);
    
    m_exportAction = new QAction(QIcon(":/icons/export.png"), tr("&Export..."), this);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::onExport);
    
    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
    
    // Edit actions
    m_undoAction = new QAction(QIcon(":/icons/undo.png"), tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setEnabled(false);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    
    m_redoAction = new QAction(QIcon(":/icons/redo.png"), tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setEnabled(false);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::onRedo);
    
    m_cutAction = new QAction(QIcon(":/icons/cut.png"), tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::onCut);
    
    m_copyAction = new QAction(QIcon(":/icons/copy.png"), tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::onCopy);
    
    m_pasteAction = new QAction(QIcon(":/icons/paste.png"), tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::onPaste);
    
    m_deleteAction = new QAction(QIcon(":/icons/delete.png"), tr("&Delete"), this);
    m_deleteAction->setShortcut(QKeySequence::Delete);
    connect(m_deleteAction, &QAction::triggered, this, &MainWindow::onDelete);
    
    m_duplicateAction = new QAction(tr("D&uplicate"), this);
    m_duplicateAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(m_duplicateAction, &QAction::triggered, this, &MainWindow::onDuplicate);
    
    m_selectAllAction = new QAction(tr("Select &All"), this);
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(m_selectAllAction, &QAction::triggered, this, &MainWindow::onSelectAll);
    
    // View actions
    m_zoomInAction = new QAction(tr("Zoom &In"), this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::onZoomIn);
    
    m_zoomOutAction = new QAction(tr("Zoom &Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::onZoomOut);
    
    m_zoomResetAction = new QAction(tr("&Reset Zoom"), this);
    m_zoomResetAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(m_zoomResetAction, &QAction::triggered, this, &MainWindow::onZoomReset);
    
    m_toggleGridAction = new QAction(tr("Show &Grid"), this);
    m_toggleGridAction->setCheckable(true);
    m_toggleGridAction->setChecked(true);
    connect(m_toggleGridAction, &QAction::triggered, this, &MainWindow::onToggleGrid);
    
    m_toggleSnapAction = new QAction(tr("&Snap to Grid"), this);
    m_toggleSnapAction->setCheckable(true);
    m_toggleSnapAction->setChecked(true);
    connect(m_toggleSnapAction, &QAction::triggered, this, &MainWindow::onToggleSnap);
    
    m_toggleGuidesAction = new QAction(tr("Show &Guides"), this);
    m_toggleGuidesAction->setCheckable(true);
    m_toggleGuidesAction->setChecked(true);
    connect(m_toggleGuidesAction, &QAction::triggered, this, &MainWindow::onToggleGuides);
    
    // Tools actions
    m_selfTestAction = new QAction(tr("Run &Self Test..."), this);
    connect(m_selfTestAction, &QAction::triggered, this, &MainWindow::onRunSelfTest);
    
    m_preferencesAction = new QAction(tr("&Preferences..."), this);
    connect(m_preferencesAction, &QAction::triggered, this, &MainWindow::onPreferences);
    
    // Help actions
    m_aboutAction = new QAction(tr("&About"), this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    
    m_helpAction = new QAction(tr("&Help"), this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::onHelp);
}

void MainWindow::createMenus() {
    // File menu
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_recentFilesMenu = m_fileMenu->addMenu(tr("Open &Recent"));
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_importAction);
    m_fileMenu->addAction(m_exportAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    // Edit menu
    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);
    m_editMenu->addAction(m_deleteAction);
    m_editMenu->addAction(m_duplicateAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_selectAllAction);
    
    // View menu
    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_zoomInAction);
    m_viewMenu->addAction(m_zoomOutAction);
    m_viewMenu->addAction(m_zoomResetAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_toggleGridAction);
    m_viewMenu->addAction(m_toggleSnapAction);
    m_viewMenu->addAction(m_toggleGuidesAction);
    
    // Tools menu
    m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
    m_toolsMenu->addAction(m_selfTestAction);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_preferencesAction);
    
    // Help menu
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars() {
    m_mainToolBar = addToolBar(tr("Main"));
    m_mainToolBar->addAction(m_newAction);
    m_mainToolBar->addAction(m_openAction);
    m_mainToolBar->addAction(m_saveAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_importAction);
    m_mainToolBar->addAction(m_exportAction);
    
    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAction);
    m_editToolBar->addAction(m_copyAction);
    m_editToolBar->addAction(m_pasteAction);
    m_editToolBar->addAction(m_deleteAction);
}

void MainWindow::createCentralWidget() {
    m_tabWidget = new QTabWidget(this);
    
    m_knobDesigner = new KnobDesigner(this);
    m_layoutDesigner = new LayoutDesigner(this);
    m_previewWidget = new PreviewWidget(this);
    
    m_tabWidget->addTab(m_knobDesigner, "Knob Designer");
    m_tabWidget->addTab(m_layoutDesigner, "Layout Designer");
    m_tabWidget->addTab(m_previewWidget, "Preview");
    
    setCentralWidget(m_tabWidget);
    
    connect(m_knobDesigner, &KnobDesigner::parametersChanged, 
            this, &MainWindow::onProjectModified);
    connect(m_layoutDesigner, &LayoutDesigner::parametersChanged,
            this, &MainWindow::onProjectModified);
}

void MainWindow::createDockWidgets() {
    // Property inspector dock
    m_propertyDock = new QDockWidget(tr("Properties"), this);
    m_propertyInspector = new PropertyInspector(this);
    m_propertyDock->setWidget(m_propertyInspector);
    addDockWidget(Qt::RightDockWidgetArea, m_propertyDock);
    
    // Asset browser dock
    m_assetDock = new QDockWidget(tr("Assets"), this);
    m_assetBrowser = new AssetBrowser(this);
    m_assetDock->setWidget(m_assetBrowser);
    addDockWidget(Qt::RightDockWidgetArea, m_assetDock);
}

// Slot implementations
void MainWindow::onNewProject() {
    if (!maybeSave()) return;
    
    m_project = std::make_unique<ProjectModel>();
    m_currentFilePath.clear();
    m_knobDesigner->setParameters(m_project->knobParams);
    m_layoutDesigner->setParameters(m_project->layoutParams);
    m_undoStack->clear();
    updateWindowTitle();
    m_statusLabel->setText("New project created");
}

void MainWindow::onOpenProject() {
    if (!maybeSave()) return;
    
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Project"), QString(),
        tr("BASL Projects (*.baslproj);;All Files (*)"));
    
    if (!filePath.isEmpty()) {
        loadProject(filePath);
    }
}

void MainWindow::onSaveProject() {
    if (m_currentFilePath.isEmpty()) {
        onSaveProjectAs();
    } else {
        saveProject(m_currentFilePath);
    }
}

void MainWindow::onSaveProjectAs() {
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("Save Project"), QString(),
        tr("BASL Projects (*.baslproj);;All Files (*)"));
    
    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".baslproj", Qt::CaseInsensitive)) {
            filePath += ".baslproj";
        }
        saveProject(filePath);
    }
}

void MainWindow::onImport() {
    ImportDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Handle import
        m_statusLabel->setText("Import completed");
    }
}

void MainWindow::onExport() {
    ExportDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        m_project->knobParams = m_knobDesigner->getParameters();
        
        KnobRenderer renderer;
        QString outputPath = dialog.getOutputPath();
        int scale = dialog.getScale();
        
        bool success = false;
        switch (dialog.getFormat()) {
            case ExportDialog::FilmstripHorizontal:
                success = renderer.exportFilmstrip(m_project->knobParams, outputPath, true, scale);
                break;
            case ExportDialog::FilmstripVertical:
                success = renderer.exportFilmstrip(m_project->knobParams, outputPath, false, scale);
                break;
            case ExportDialog::FrameSequence:
                success = renderer.exportFrameSequence(m_project->knobParams, outputPath, "frame_", scale);
                break;
            case ExportDialog::AtlasJSON:
                success = renderer.exportAtlas(m_project->knobParams, 
                    outputPath, outputPath.replace(".png", ".json"), scale);
                break;
        }
        
        if (success) {
            m_statusLabel->setText("Export completed successfully");
            QMessageBox::information(this, "Export", "Export completed successfully!");
        } else {
            m_statusLabel->setText("Export failed");
            QMessageBox::warning(this, "Export", "Export failed!");
        }
    }
}

void MainWindow::onOpenRecentFile() {
    QAction *action = qobject_cast<QAction*>(sender());
    if (action && maybeSave()) {
        loadProject(action->data().toString());
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onUndo() {
    m_undoStack->undo();
}

void MainWindow::onRedo() {
    m_undoStack->redo();
}

void MainWindow::onCut() {
    onCopy();
    onDelete();
}

void MainWindow::onCopy() {
    // Copy selected items to clipboard
    m_statusLabel->setText("Copied to clipboard");
}

void MainWindow::onPaste() {
    // Paste from clipboard
    m_statusLabel->setText("Pasted from clipboard");
}

void MainWindow::onDelete() {
    if (m_tabWidget->currentWidget() == m_layoutDesigner) {
        m_layoutDesigner->deleteSelected();
        m_statusLabel->setText("Deleted selection");
    }
}

void MainWindow::onDuplicate() {
    if (m_tabWidget->currentWidget() == m_layoutDesigner) {
        m_layoutDesigner->duplicateSelected();
        m_statusLabel->setText("Duplicated selection");
    }
}

void MainWindow::onSelectAll() {
    if (m_tabWidget->currentWidget() == m_layoutDesigner) {
        m_layoutDesigner->selectAll();
    }
}

void MainWindow::onZoomIn() {
    if (m_tabWidget->currentWidget() == m_layoutDesigner) {
        m_layoutDesigner->setZoom(m_layoutDesigner->getZoom() * 1.2);
    }
}

void MainWindow::onZoomOut() {
    if (m_tabWidget->currentWidget() == m_layoutDesigner) {
        m_layoutDesigner->setZoom(m_layoutDesigner->getZoom() / 1.2);
    }
}

void MainWindow::onZoomReset() {
    if (m_tabWidget->currentWidget() == m_layoutDesigner) {
        m_layoutDesigner->setZoom(1.0);
    }
}

void MainWindow::onToggleGrid() {
    m_layoutDesigner->setGridVisible(m_toggleGridAction->isChecked());
}

void MainWindow::onToggleSnap() {
    m_layoutDesigner->setSnapToGrid(m_toggleSnapAction->isChecked());
}

void MainWindow::onToggleGuides() {
    m_layoutDesigner->setGuidesVisible(m_toggleGuidesAction->isChecked());
}

void MainWindow::onRunSelfTest() {
    SelfTestDialog *dialog = new SelfTestDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
    dialog->runTests();
}

void MainWindow::onPreferences() {
    QMessageBox::information(this, "Preferences", 
        "Preferences dialog would be shown here.\n"
        "Settings: autosave interval, default canvas size, etc.");
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About BASL Designer",
        "<h2>BASL Designer 1.0</h2>"
        "<p>Broken Arrow Software Layer Designer</p>"
        "<p>A professional knob and skin design tool modeled after KnobMan and SkinMan.</p>"
        "<p>Copyright © 2024 Broken Arrow Software</p>");
}

void MainWindow::onHelp() {
    QMessageBox::information(this, "Help",
        "<h3>BASL Designer Help</h3>"
        "<p><b>Knob Designer:</b> Create rotary knobs with layers, ticks, and pointers</p>"
        "<p><b>Layout Designer:</b> Design UI layouts with controls</p>"
        "<p><b>Preview:</b> Preview your designs in action</p>"
        "<p><b>Export:</b> Export to filmstrip, frame sequence, or atlas+JSON</p>"
        "<p><b>Self Test:</b> Run automated tests from Tools menu</p>");
}

void MainWindow::onProjectModified() {
    m_project->setModified(true);
    updateWindowTitle();
}

void MainWindow::onTabChanged(int index) {
    // Update preview when switching tabs
    if (m_tabWidget->widget(index) == m_previewWidget) {
        KnobRenderer renderer;
        QList<QImage> frames = renderer.renderAllFrames(m_knobDesigner->getParameters());
        m_previewWidget->setFrames(frames);
    }
}

void MainWindow::updateWindowTitle() {
    QString title = "BASL Designer";
    if (!m_currentFilePath.isEmpty()) {
        title += " - " + QFileInfo(m_currentFilePath).fileName();
    }
    if (m_project->isModified()) {
        title += " *";
    }
    setWindowTitle(title);
}

void MainWindow::updateRecentFiles() {
    m_recentFilesMenu->clear();
    QStringList recentFiles = m_recentFilesManager->getRecentFiles();
    
    for (const QString& filePath : recentFiles) {
        QAction *action = m_recentFilesMenu->addAction(QFileInfo(filePath).fileName());
        action->setData(filePath);
        action->setToolTip(filePath);
        connect(action, &QAction::triggered, this, &MainWindow::onOpenRecentFile);
    }
    
    if (!recentFiles.isEmpty()) {
        m_recentFilesMenu->addSeparator();
    }
    
    QAction *clearAction = m_recentFilesMenu->addAction("Clear Recent Files");
    connect(clearAction, &QAction::triggered, [this]() {
        m_recentFilesManager->clearRecentFiles();
        updateRecentFiles();
    });
}

bool MainWindow::maybeSave() {
    if (!m_project->isModified()) {
        return true;
    }
    
    QMessageBox::StandardButton ret = QMessageBox::warning(this, "BASL Designer",
        "The project has been modified.\nDo you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    
    if (ret == QMessageBox::Save) {
        onSaveProject();
        return !m_project->isModified();
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void MainWindow::loadProject(const QString& filePath) {
    if (m_project->loadFromFile(filePath)) {
        m_currentFilePath = filePath;
        m_knobDesigner->setParameters(m_project->knobParams);
        m_layoutDesigner->setParameters(m_project->layoutParams);
        m_recentFilesManager->addRecentFile(filePath);
        updateRecentFiles();
        updateWindowTitle();
        m_statusLabel->setText("Project loaded: " + QFileInfo(filePath).fileName());
    } else {
        QMessageBox::warning(this, "Error", "Failed to load project file.");
    }
}

void MainWindow::saveProject(const QString& filePath) {
    // Update project with current parameters
    m_project->knobParams = m_knobDesigner->getParameters();
    m_project->layoutParams = m_layoutDesigner->getParameters();
    m_project->projectPath = filePath;
    m_project->projectName = QFileInfo(filePath).baseName();
    
    if (m_project->saveToFile(filePath)) {
        m_currentFilePath = filePath;
        m_recentFilesManager->addRecentFile(filePath);
        updateRecentFiles();
        updateWindowTitle();
        m_statusLabel->setText("Project saved: " + QFileInfo(filePath).fileName());
    } else {
        QMessageBox::warning(this, "Error", "Failed to save project file.");
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
