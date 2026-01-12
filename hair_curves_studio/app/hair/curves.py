"""Hair curves and spline utilities"""

import numpy as np
from typing import List, Tuple, Optional
from ..core.utils import (
    normalize_vector, compute_curve_length, resample_curve_uniform,
    lerp_array
)


class Curve:
    """Represents a single hair curve (guide or strand)"""
    
    def __init__(self, points: np.ndarray, root_position: np.ndarray, root_normal: np.ndarray):
        """
        Initialize a curve
        
        Args:
            points: Curve control points (N x 3)
            root_position: Position on scalp surface
            root_normal: Normal at root position
        """
        self.points = np.array(points, dtype=np.float32)
        self.root_position = np.array(root_position, dtype=np.float32)
        self.root_normal = normalize_vector(np.array(root_normal, dtype=np.float32))
        
        # Attributes
        self.width_root = 1.0
        self.width_tip = 0.1
        self.clump_id = 0
        self.seed = 0
    
    def get_point_at(self, t: float) -> np.ndarray:
        """Get point along curve at parameter t (0 to 1)"""
        if len(self.points) == 0:
            return self.root_position
        
        if len(self.points) == 1:
            return self.points[0]
        
        # Find segment
        n = len(self.points) - 1
        segment_idx = min(int(t * n), n - 1)
        local_t = (t * n) - segment_idx
        
        return lerp_array(self.points[segment_idx], self.points[segment_idx + 1], local_t)
    
    def get_tangent_at(self, t: float) -> np.ndarray:
        """Get tangent along curve at parameter t"""
        if len(self.points) < 2:
            return self.root_normal
        
        # Find segment
        n = len(self.points) - 1
        segment_idx = min(int(t * n), n - 1)
        
        tangent = self.points[segment_idx + 1] - self.points[segment_idx]
        return normalize_vector(tangent)
    
    def get_length(self) -> float:
        """Get total curve length"""
        return compute_curve_length(self.points)
    
    def resample(self, num_points: int):
        """Resample curve with given number of points"""
        if num_points < 2:
            num_points = 2
        self.points = resample_curve_uniform(self.points, num_points)
    
    def offset(self, offset: np.ndarray):
        """Offset all points by a vector"""
        self.points += offset
        self.root_position += offset
    
    def scale_length(self, factor: float):
        """Scale curve length from root"""
        if len(self.points) == 0:
            return
        
        direction = self.points - self.root_position
        self.points = self.root_position + direction * factor
    
    def apply_noise(self, amplitude: float, frequency: float, seed: int):
        """Apply noise to curve"""
        rng = np.random.default_rng(seed + self.seed)
        
        for i in range(len(self.points)):
            t = i / max(1, len(self.points) - 1)
            noise_factor = np.sin(t * frequency * np.pi) * amplitude
            
            # Random direction perpendicular to curve
            tangent = self.get_tangent_at(t)
            perp1 = np.cross(tangent, [0, 0, 1])
            if np.linalg.norm(perp1) < 0.1:
                perp1 = np.cross(tangent, [0, 1, 0])
            perp1 = normalize_vector(perp1)
            
            angle = rng.random() * 2 * np.pi
            perp = np.cos(angle) * perp1 + np.sin(angle) * np.cross(tangent, perp1)
            
            self.points[i] += perp * noise_factor * t
    
    def apply_gravity(self, strength: float, direction: np.ndarray = np.array([0, 0, -1])):
        """Apply gravity/bend to curve"""
        direction = normalize_vector(direction)
        
        for i in range(len(self.points)):
            t = i / max(1, len(self.points) - 1)
            # Quadratic falloff
            factor = t * t * strength
            self.points[i] += direction * factor
    
    def smooth(self, iterations: int = 1, strength: float = 0.5):
        """Smooth curve by averaging neighbor points"""
        for _ in range(iterations):
            if len(self.points) < 3:
                return
            
            new_points = self.points.copy()
            for i in range(1, len(self.points) - 1):
                avg = (self.points[i - 1] + self.points[i + 1]) * 0.5
                new_points[i] = lerp_array(self.points[i], avg, strength)
            
            self.points = new_points
    
    def to_dict(self) -> dict:
        """Convert to dictionary for serialization"""
        return {
            "points": self.points.tolist(),
            "root_position": self.root_position.tolist(),
            "root_normal": self.root_normal.tolist(),
            "width_root": float(self.width_root),
            "width_tip": float(self.width_tip),
            "clump_id": int(self.clump_id),
            "seed": int(self.seed)
        }
    
    @staticmethod
    def from_dict(data: dict) -> 'Curve':
        """Create from dictionary"""
        curve = Curve(
            np.array(data["points"], dtype=np.float32),
            np.array(data["root_position"], dtype=np.float32),
            np.array(data["root_normal"], dtype=np.float32)
        )
        curve.width_root = data.get("width_root", 1.0)
        curve.width_tip = data.get("width_tip", 0.1)
        curve.clump_id = data.get("clump_id", 0)
        curve.seed = data.get("seed", 0)
        return curve


def catmull_rom_spline(p0: np.ndarray, p1: np.ndarray, p2: np.ndarray, p3: np.ndarray, t: float) -> np.ndarray:
    """Evaluate Catmull-Rom spline at parameter t"""
    t2 = t * t
    t3 = t2 * t
    
    return 0.5 * (
        (2 * p1) +
        (-p0 + p2) * t +
        (2 * p0 - 5 * p1 + 4 * p2 - p3) * t2 +
        (-p0 + 3 * p1 - 3 * p2 + p3) * t3
    )


def bezier_cubic(p0: np.ndarray, p1: np.ndarray, p2: np.ndarray, p3: np.ndarray, t: float) -> np.ndarray:
    """Evaluate cubic Bezier curve at parameter t"""
    u = 1.0 - t
    u2 = u * u
    u3 = u2 * u
    t2 = t * t
    t3 = t2 * t
    
    return u3 * p0 + 3 * u2 * t * p1 + 3 * u * t2 * p2 + t3 * p3
