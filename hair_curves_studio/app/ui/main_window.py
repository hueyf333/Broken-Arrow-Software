"""Main application window"""

from PySide6.QtWidgets import (
    QMainWindow, QDockWidget, QMenuBar, QMenu, QToolBar,
    QStatusBar, QFileDialog, QMessageBox, QLabel
)
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QAction, QKeySequence
from pathlib import Path

from .panels_outliner import OutlinerPanel
from .panels_properties import PropertiesPanel
from .panels_modifiers import ModifierStackPanel
from .panels_groom import GroomPanel
from .panels_bake_export import BakeExportPanel
from ..viewport.viewport_widget import ViewportWidget
from ..core.logger import logger
from ..core.commands import CommandHistory
from ..io.project import Project


class MainWindow(QMainWindow):
    """Main application window with dockable panels"""
    
    def __init__(self):
        super().__init__()
        
        self.setWindowTitle("Broken Arrow Hair Curves Studio")
        self.resize(1600, 900)
        
        # Application state
        self.project = Project()
        self.command_history = CommandHistory()
        
        # Create UI
        self._create_viewport()
        self._create_panels()
        self._create_menus()
        self._create_toolbar()
        self._create_statusbar()
        
        # Autosave timer
        self.autosave_timer = QTimer()
        self.autosave_timer.timeout.connect(self._autosave)
        self.autosave_timer.start(60000)  # 1 minute
        
        logger.info("Main window initialized")
    
    def _create_viewport(self):
        """Create central 3D viewport"""
        self.viewport = ViewportWidget(self)
        self.setCentralWidget(self.viewport)
    
    def _create_panels(self):
        """Create dockable panels"""
        # Outliner (left)
        self.outliner_panel = OutlinerPanel(self)
        self.outliner_dock = QDockWidget("Outliner", self)
        self.outliner_dock.setWidget(self.outliner_panel)
        self.addDockWidget(Qt.LeftDockWidgetArea, self.outliner_dock)
        
        # Properties (right)
        self.properties_panel = PropertiesPanel(self)
        self.properties_dock = QDockWidget("Properties", self)
        self.properties_dock.setWidget(self.properties_panel)
        self.addDockWidget(Qt.RightDockWidgetArea, self.properties_dock)
        
        # Modifier Stack (right, below properties)
        self.modifier_panel = ModifierStackPanel(self)
        self.modifier_dock = QDockWidget("Modifier Stack", self)
        self.modifier_dock.setWidget(self.modifier_panel)
        self.addDockWidget(Qt.RightDockWidgetArea, self.modifier_dock)
        
        # Groom Tools (right, below modifiers)
        self.groom_panel = GroomPanel(self)
        self.groom_dock = QDockWidget("Groom Tools", self)
        self.groom_dock.setWidget(self.groom_panel)
        self.addDockWidget(Qt.RightDockWidgetArea, self.groom_dock)
        
        # Bake/Export (right, below groom)
        self.bake_export_panel = BakeExportPanel(self)
        self.bake_export_dock = QDockWidget("Bake/Export", self)
        self.bake_export_dock.setWidget(self.bake_export_panel)
        self.addDockWidget(Qt.RightDockWidgetArea, self.bake_export_dock)
    
    def _create_menus(self):
        """Create menu bar"""
        menubar = self.menuBar()
        
        # File menu
        file_menu = menubar.addMenu("&File")
        
        new_action = QAction("&New", self)
        new_action.setShortcut(QKeySequence.New)
        new_action.triggered.connect(self.new_project)
        file_menu.addAction(new_action)
        
        open_action = QAction("&Open...", self)
        open_action.setShortcut(QKeySequence.Open)
        open_action.triggered.connect(self.open_project)
        file_menu.addAction(open_action)
        
        save_action = QAction("&Save", self)
        save_action.setShortcut(QKeySequence.Save)
        save_action.triggered.connect(self.save_project)
        file_menu.addAction(save_action)
        
        save_as_action = QAction("Save &As...", self)
        save_as_action.setShortcut(QKeySequence.SaveAs)
        save_as_action.triggered.connect(self.save_project_as)
        file_menu.addAction(save_as_action)
        
        file_menu.addSeparator()
        
        import_action = QAction("&Import Scalp...", self)
        import_action.triggered.connect(self.import_mesh)
        file_menu.addAction(import_action)
        
        file_menu.addSeparator()
        
        exit_action = QAction("E&xit", self)
        exit_action.setShortcut(QKeySequence.Quit)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)
        
        # Edit menu
        edit_menu = menubar.addMenu("&Edit")
        
        undo_action = QAction("&Undo", self)
        undo_action.setShortcut(QKeySequence.Undo)
        undo_action.triggered.connect(self.undo)
        edit_menu.addAction(undo_action)
        
        redo_action = QAction("&Redo", self)
        redo_action.setShortcut(QKeySequence.Redo)
        redo_action.triggered.connect(self.redo)
        edit_menu.addAction(redo_action)
        
        # View menu
        view_menu = menubar.addMenu("&View")
        
        wireframe_action = QAction("Toggle Wireframe", self)
        wireframe_action.setCheckable(True)
        wireframe_action.triggered.connect(self.toggle_wireframe)
        view_menu.addAction(wireframe_action)
        
        frame_all_action = QAction("Frame All", self)
        frame_all_action.setShortcut("F")
        frame_all_action.triggered.connect(self.frame_all)
        view_menu.addAction(frame_all_action)
        
        # Help menu
        help_menu = menubar.addMenu("&Help")
        
        demo_action = QAction("Quick &Demo", self)
        demo_action.triggered.connect(self.run_quick_demo)
        help_menu.addAction(demo_action)
        
        help_menu.addSeparator()
        
        about_action = QAction("&About", self)
        about_action.triggered.connect(self.show_about)
        help_menu.addAction(about_action)
    
    def _create_toolbar(self):
        """Create toolbar"""
        toolbar = QToolBar("Main Toolbar")
        self.addToolBar(toolbar)
        
        # Add common actions
        # (Simplified for now)
    
    def _create_statusbar(self):
        """Create status bar"""
        self.statusbar = QStatusBar()
        self.setStatusBar(self.statusbar)
        
        self.fps_label = QLabel("FPS: 0")
        self.statusbar.addPermanentWidget(self.fps_label)
        
        self.strand_count_label = QLabel("Strands: 0")
        self.statusbar.addPermanentWidget(self.strand_count_label)
    
    # File operations
    def new_project(self):
        """Create new project"""
        self.project = Project()
        self.command_history.clear()
        self.viewport.clear_meshes()
        self.statusbar.showMessage("New project created", 3000)
    
    def open_project(self):
        """Open project file"""
        filepath, _ = QFileDialog.getOpenFileName(
            self, "Open Project", "", "Hair Curves Project (*.hcp)"
        )
        if filepath:
            if self.project.load(Path(filepath)):
                self.statusbar.showMessage(f"Opened: {filepath}", 3000)
            else:
                QMessageBox.warning(self, "Error", "Failed to open project")
    
    def save_project(self):
        """Save project"""
        if self.project.filepath:
            self.project.save()
            self.statusbar.showMessage(f"Saved: {self.project.filepath}", 3000)
        else:
            self.save_project_as()
    
    def save_project_as(self):
        """Save project as"""
        filepath, _ = QFileDialog.getSaveFileName(
            self, "Save Project As", "", "Hair Curves Project (*.hcp)"
        )
        if filepath:
            if self.project.save(Path(filepath)):
                self.statusbar.showMessage(f"Saved: {filepath}", 3000)
            else:
                QMessageBox.warning(self, "Error", "Failed to save project")
    
    def import_mesh(self):
        """Import scalp mesh"""
        from ..io.import_mesh import import_mesh
        
        filepath, _ = QFileDialog.getOpenFileName(
            self, "Import Scalp Mesh", "", "Mesh Files (*.obj *.gltf *.glb)"
        )
        if filepath:
            mesh_data = import_mesh(Path(filepath))
            if mesh_data:
                self.viewport.add_mesh(
                    mesh_data['name'],
                    mesh_data['vertices'],
                    mesh_data['triangles'],
                    mesh_data['normals'],
                    mesh_data['uvs']
                )
                self.viewport.frame_all()
                self.statusbar.showMessage(f"Imported: {filepath}", 3000)
            else:
                QMessageBox.warning(self, "Error", "Failed to import mesh")
    
    # Edit operations
    def undo(self):
        """Undo last command"""
        if self.command_history.undo():
            self.statusbar.showMessage("Undo", 2000)
            self.viewport.update()
    
    def redo(self):
        """Redo last undone command"""
        if self.command_history.redo():
            self.statusbar.showMessage("Redo", 2000)
            self.viewport.update()
    
    # View operations
    def toggle_wireframe(self, checked):
        """Toggle wireframe mode"""
        self.viewport.show_wireframe = checked
        self.viewport.update()
    
    def frame_all(self):
        """Frame all objects in viewport"""
        self.viewport.frame_all()
    
    # Demo
    def run_quick_demo(self):
        """Run quick demonstration"""
        QMessageBox.information(
            self,
            "Quick Demo",
            "Quick Demo feature: This would load a sample scalp, generate hair, "
            "apply modifiers, bake, and export. Implementation in progress."
        )
    
    def show_about(self):
        """Show about dialog"""
        QMessageBox.about(
            self,
            "About Broken Arrow Hair Curves Studio",
            "Broken Arrow Hair Curves Studio v1.0.0\n\n"
            "A complete desktop application for creating and customizing hairstyles\n"
            "using a Hair Curves system modeled after Blender's workflow.\n\n"
            "© 2024 Broken Arrow Software\n"
            "Licensed under GPL-3.0"
        )
    
    def _autosave(self):
        """Autosave project"""
        if self.project.filepath and self.project.is_modified():
            self.project.create_autosave()
    
    def closeEvent(self, event):
        """Handle window close"""
        if self.project.is_modified():
            reply = QMessageBox.question(
                self,
                "Unsaved Changes",
                "You have unsaved changes. Do you want to save before closing?",
                QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel
            )
            
            if reply == QMessageBox.Save:
                self.save_project()
                event.accept()
            elif reply == QMessageBox.Discard:
                event.accept()
            else:
                event.ignore()
        else:
            event.accept()
