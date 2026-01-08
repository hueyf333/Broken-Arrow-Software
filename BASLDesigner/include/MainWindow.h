#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QUndoStack>
#include <QDockWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <memory>

// Forward declarations
class KnobDesigner;
class LayoutDesigner;
class PreviewWidget;
class PropertyInspector;
class AssetBrowser;
class ProjectModel;
class RecentFilesManager;
class AutoSaveManager;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File menu
    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onSaveProjectAs();
    void onImport();
    void onExport();
    void onOpenRecentFile();
    void onExit();
    
    // Edit menu
    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();
    void onDelete();
    void onDuplicate();
    void onSelectAll();
    
    // View menu
    void onZoomIn();
    void onZoomOut();
    void onZoomReset();
    void onToggleGrid();
    void onToggleSnap();
    void onToggleGuides();
    
    // Tools menu
    void onRunSelfTest();
    void onPreferences();
    
    // Help menu
    void onAbout();
    void onHelp();
    
    // Other
    void onProjectModified();
    void onTabChanged(int index);
    void updateWindowTitle();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWidgets();
    void createCentralWidget();
    void updateRecentFiles();
    bool maybeSave();
    void loadProject(const QString& filePath);
    void saveProject(const QString& filePath);
    
    // UI Components
    QTabWidget *m_tabWidget;
    KnobDesigner *m_knobDesigner;
    LayoutDesigner *m_layoutDesigner;
    PreviewWidget *m_previewWidget;
    
    // Dock widgets
    QDockWidget *m_propertyDock;
    PropertyInspector *m_propertyInspector;
    QDockWidget *m_assetDock;
    AssetBrowser *m_assetBrowser;
    
    // Menus
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_toolsMenu;
    QMenu *m_helpMenu;
    QMenu *m_recentFilesMenu;
    
    // Toolbars
    QToolBar *m_mainToolBar;
    QToolBar *m_editToolBar;
    
    // Status bar
    QLabel *m_statusLabel;
    
    // Actions
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_importAction;
    QAction *m_exportAction;
    QAction *m_exitAction;
    
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    QAction *m_deleteAction;
    QAction *m_duplicateAction;
    QAction *m_selectAllAction;
    
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_zoomResetAction;
    QAction *m_toggleGridAction;
    QAction *m_toggleSnapAction;
    QAction *m_toggleGuidesAction;
    
    QAction *m_selfTestAction;
    QAction *m_preferencesAction;
    
    QAction *m_aboutAction;
    QAction *m_helpAction;
    
    // Data model
    std::unique_ptr<ProjectModel> m_project;
    QUndoStack *m_undoStack;
    QString m_currentFilePath;
    
    // Managers
    std::unique_ptr<RecentFilesManager> m_recentFilesManager;
    std::unique_ptr<AutoSaveManager> m_autoSaveManager;
};

#endif // MAINWINDOW_H
