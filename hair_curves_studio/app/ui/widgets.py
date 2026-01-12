"""Custom widgets for the UI"""

from PySide6.QtWidgets import QWidget, QSlider, QLabel, QHBoxLayout
from PySide6.QtCore import Qt, Signal


class LabeledSlider(QWidget):
    """Slider with label and value display"""
    
    valueChanged = Signal(int)
    
    def __init__(self, label: str, min_val: int = 0, max_val: int = 100, default: int = 50, parent=None):
        super().__init__(parent)
        
        layout = QHBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        
        self.label = QLabel(label)
        layout.addWidget(self.label)
        
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setRange(min_val, max_val)
        self.slider.setValue(default)
        self.slider.valueChanged.connect(self._on_value_changed)
        layout.addWidget(self.slider)
        
        self.value_label = QLabel(str(default))
        self.value_label.setMinimumWidth(40)
        layout.addWidget(self.value_label)
    
    def _on_value_changed(self, value):
        """Handle value change"""
        self.value_label.setText(str(value))
        self.valueChanged.emit(value)
    
    def value(self) -> int:
        """Get current value"""
        return self.slider.value()
    
    def setValue(self, value: int):
        """Set value"""
        self.slider.setValue(value)
