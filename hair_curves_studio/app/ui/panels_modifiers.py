"""Modifier stack panel"""

from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QListWidget, QPushButton, QMenu,
    QHBoxLayout, QListWidgetItem
)
from PySide6.QtCore import Qt


class ModifierStackPanel(QWidget):
    """Panel for managing modifier stack"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        layout = QVBoxLayout(self)
        
        # Modifier list
        self.modifier_list = QListWidget()
        self.modifier_list.setDragDropMode(QListWidget.InternalMove)
        layout.addWidget(self.modifier_list)
        
        # Buttons
        button_layout = QHBoxLayout()
        
        self.add_button = QPushButton("Add Modifier")
        self.add_button.clicked.connect(self.show_add_menu)
        button_layout.addWidget(self.add_button)
        
        self.remove_button = QPushButton("Remove")
        self.remove_button.clicked.connect(self.remove_modifier)
        button_layout.addWidget(self.remove_button)
        
        layout.addLayout(button_layout)
    
    def show_add_menu(self):
        """Show menu to add modifiers"""
        menu = QMenu(self)
        
        modifiers = [
            "Length", "Width", "Clump", "Noise",
            "Curl", "Bend", "Parting", "Smooth", "Mirror"
        ]
        
        for mod in modifiers:
            action = menu.addAction(mod)
            action.triggered.connect(lambda checked, m=mod: self.add_modifier(m))
        
        menu.exec_(self.add_button.mapToGlobal(self.add_button.rect().bottomLeft()))
    
    def add_modifier(self, modifier_type: str):
        """Add a modifier to the stack"""
        item = QListWidgetItem(modifier_type)
        self.modifier_list.addItem(item)
    
    def remove_modifier(self):
        """Remove selected modifier"""
        current = self.modifier_list.currentRow()
        if current >= 0:
            self.modifier_list.takeItem(current)
