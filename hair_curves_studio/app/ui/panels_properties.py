"""Properties panel for selected objects"""

from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QLabel, QLineEdit, QDoubleSpinBox,
    QSpinBox, QFormLayout, QGroupBox
)


class PropertiesPanel(QWidget):
    """Panel showing properties of selected object"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        layout = QVBoxLayout(self)
        
        # Title
        self.title_label = QLabel("Properties")
        self.title_label.setStyleSheet("font-weight: bold; font-size: 14px;")
        layout.addWidget(self.title_label)
        
        # Hair Description properties
        hair_group = QGroupBox("Hair Description")
        hair_layout = QFormLayout()
        
        self.name_edit = QLineEdit()
        hair_layout.addRow("Name:", self.name_edit)
        
        self.density_spin = QSpinBox()
        self.density_spin.setRange(100, 100000)
        self.density_spin.setValue(10000)
        hair_layout.addRow("Density:", self.density_spin)
        
        self.length_spin = QDoubleSpinBox()
        self.length_spin.setRange(0.1, 100.0)
        self.length_spin.setValue(10.0)
        hair_layout.addRow("Length:", self.length_spin)
        
        self.width_root_spin = QDoubleSpinBox()
        self.width_root_spin.setRange(0.01, 10.0)
        self.width_root_spin.setValue(1.0)
        hair_layout.addRow("Width (Root):", self.width_root_spin)
        
        self.width_tip_spin = QDoubleSpinBox()
        self.width_tip_spin.setRange(0.01, 10.0)
        self.width_tip_spin.setValue(0.1)
        hair_layout.addRow("Width (Tip):", self.width_tip_spin)
        
        hair_group.setLayout(hair_layout)
        layout.addWidget(hair_group)
        
        layout.addStretch()
