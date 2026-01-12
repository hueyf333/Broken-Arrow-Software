"""Outliner panel for scene hierarchy"""

from PySide6.QtWidgets import QWidget, QVBoxLayout, QTreeWidget, QTreeWidgetItem, QPushButton
from PySide6.QtCore import Qt


class OutlinerPanel(QWidget):
    """Panel showing scene hierarchy"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        layout = QVBoxLayout(self)
        
        # Tree widget
        self.tree = QTreeWidget()
        self.tree.setHeaderLabel("Scene Objects")
        layout.addWidget(self.tree)
        
        # Add sample items
        self._populate_tree()
    
    def _populate_tree(self):
        """Populate tree with sample data"""
        # Scene root
        scene_item = QTreeWidgetItem(self.tree, ["Scene"])
        
        # Scalp meshes
        scalp_folder = QTreeWidgetItem(scene_item, ["Scalp Meshes"])
        
        # Hair descriptions
        hair_folder = QTreeWidgetItem(scene_item, ["Hair Descriptions"])
        
        # Bake outputs
        bake_folder = QTreeWidgetItem(scene_item, ["Bake Outputs"])
        
        self.tree.expandAll()
