"""Groom and brush tools panel"""

from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QComboBox, QLabel, QDoubleSpinBox,
    QFormLayout, QGroupBox, QPushButton
)


class GroomPanel(QWidget):
    """Panel for groom and brush tools"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        layout = QVBoxLayout(self)
        
        # Brush selection
        brush_group = QGroupBox("Brush Tool")
        brush_layout = QFormLayout()
        
        self.brush_type = QComboBox()
        self.brush_type.addItems([
            "Comb", "Smooth", "Cut/Grow", "Clump Paint",
            "Density Paint", "Curl Paint"
        ])
        brush_layout.addRow("Type:", self.brush_type)
        
        self.brush_radius = QDoubleSpinBox()
        self.brush_radius.setRange(0.1, 50.0)
        self.brush_radius.setValue(5.0)
        brush_layout.addRow("Radius:", self.brush_radius)
        
        self.brush_strength = QDoubleSpinBox()
        self.brush_strength.setRange(0.0, 1.0)
        self.brush_strength.setSingleStep(0.1)
        self.brush_strength.setValue(0.5)
        brush_layout.addRow("Strength:", self.brush_strength)
        
        brush_group.setLayout(brush_layout)
        layout.addWidget(brush_group)
        
        # Guide tools
        guide_group = QGroupBox("Guide Tools")
        guide_layout = QVBoxLayout()
        
        self.create_guides_btn = QPushButton("Generate Guides")
        guide_layout.addWidget(self.create_guides_btn)
        
        self.draw_guide_btn = QPushButton("Draw Guide")
        guide_layout.addWidget(self.draw_guide_btn)
        
        guide_group.setLayout(guide_layout)
        layout.addWidget(guide_group)
        
        layout.addStretch()
