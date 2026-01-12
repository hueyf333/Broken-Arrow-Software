"""Transform gizmos for 3D viewport"""

import numpy as np
from typing import Optional, Tuple


class Gizmo:
    """Base class for transform gizmos"""
    
    def __init__(self):
        self.position = np.array([0, 0, 0], dtype=np.float32)
        self.active_axis = None  # 'X', 'Y', 'Z', or None
    
    def hit_test(self, ray_origin: np.ndarray, ray_direction: np.ndarray) -> Optional[str]:
        """Test if ray hits gizmo, return axis or None"""
        # Simplified hit test
        return None
    
    def get_geometry(self) -> dict:
        """Get gizmo geometry for rendering"""
        return {}


class TranslateGizmo(Gizmo):
    """Translation gizmo with 3 arrows"""
    
    def __init__(self):
        super().__init__()
        self.arrow_length = 2.0
        self.arrow_radius = 0.1
    
    def get_geometry(self) -> dict:
        """Get arrow geometry"""
        # Returns simplified arrow geometry for rendering
        return {
            'X': {'color': [1, 0, 0], 'direction': [1, 0, 0]},
            'Y': {'color': [0, 1, 0], 'direction': [0, 1, 0]},
            'Z': {'color': [0, 0, 1], 'direction': [0, 0, 1]},
        }


class RotateGizmo(Gizmo):
    """Rotation gizmo with 3 rings"""
    
    def __init__(self):
        super().__init__()
        self.ring_radius = 2.0
    
    def get_geometry(self) -> dict:
        """Get ring geometry"""
        return {
            'X': {'color': [1, 0, 0], 'axis': [1, 0, 0]},
            'Y': {'color': [0, 1, 0], 'axis': [0, 1, 0]},
            'Z': {'color': [0, 0, 1], 'axis': [0, 0, 1]},
        }


class ScaleGizmo(Gizmo):
    """Scale gizmo with 3 boxes"""
    
    def __init__(self):
        super().__init__()
        self.handle_size = 0.2
    
    def get_geometry(self) -> dict:
        """Get box geometry"""
        return {
            'X': {'color': [1, 0, 0], 'direction': [1, 0, 0]},
            'Y': {'color': [0, 1, 0], 'direction': [0, 1, 0]},
            'Z': {'color': [0, 0, 1], 'direction': [0, 0, 1]},
        }
