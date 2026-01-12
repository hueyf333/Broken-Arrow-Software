"""Camera controller for 3D viewport"""

import numpy as np
from typing import Tuple
from ..core.utils import create_transform_matrix


class Camera:
    """Orbital camera for 3D viewport"""
    
    def __init__(self):
        # Orbital parameters
        self.target = np.array([0.0, 0.0, 0.0], dtype=np.float32)
        self.distance = 10.0
        self.azimuth = 45.0  # degrees
        self.elevation = 30.0  # degrees
        
        # Projection
        self.fov = 45.0  # degrees
        self.near_clip = 0.1
        self.far_clip = 1000.0
        self.aspect_ratio = 16.0 / 9.0
        
        # Pan offset
        self.pan_offset = np.array([0.0, 0.0], dtype=np.float32)
    
    def get_position(self) -> np.ndarray:
        """Get camera world position"""
        azimuth_rad = np.radians(self.azimuth)
        elevation_rad = np.radians(self.elevation)
        
        x = self.distance * np.cos(elevation_rad) * np.cos(azimuth_rad)
        y = self.distance * np.cos(elevation_rad) * np.sin(azimuth_rad)
        z = self.distance * np.sin(elevation_rad)
        
        position = self.target + np.array([x, y, z], dtype=np.float32)
        return position
    
    def get_view_matrix(self) -> np.ndarray:
        """Get view matrix"""
        position = self.get_position()
        
        # Look at target
        forward = self.target - position
        forward = forward / (np.linalg.norm(forward) + 1e-8)
        
        # Right vector
        world_up = np.array([0.0, 0.0, 1.0], dtype=np.float32)
        right = np.cross(forward, world_up)
        right = right / (np.linalg.norm(right) + 1e-8)
        
        # Up vector
        up = np.cross(right, forward)
        up = up / (np.linalg.norm(up) + 1e-8)
        
        # View matrix
        view = np.eye(4, dtype=np.float32)
        view[0, :3] = right
        view[1, :3] = up
        view[2, :3] = -forward
        view[:3, 3] = -np.array([
            np.dot(right, position),
            np.dot(up, position),
            np.dot(-forward, position)
        ])
        
        # Apply pan offset
        view[3, 0] += self.pan_offset[0]
        view[3, 1] += self.pan_offset[1]
        
        return view
    
    def get_projection_matrix(self) -> np.ndarray:
        """Get projection matrix"""
        fov_rad = np.radians(self.fov)
        f = 1.0 / np.tan(fov_rad / 2.0)
        
        proj = np.zeros((4, 4), dtype=np.float32)
        proj[0, 0] = f / self.aspect_ratio
        proj[1, 1] = f
        proj[2, 2] = (self.far_clip + self.near_clip) / (self.near_clip - self.far_clip)
        proj[2, 3] = (2.0 * self.far_clip * self.near_clip) / (self.near_clip - self.far_clip)
        proj[3, 2] = -1.0
        
        return proj
    
    def orbit(self, delta_azimuth: float, delta_elevation: float):
        """Orbit camera around target"""
        self.azimuth += delta_azimuth
        self.elevation += delta_elevation
        
        # Clamp elevation
        self.elevation = np.clip(self.elevation, -89.0, 89.0)
        
        # Wrap azimuth
        self.azimuth = self.azimuth % 360.0
    
    def pan(self, delta_x: float, delta_y: float):
        """Pan camera"""
        self.pan_offset[0] += delta_x
        self.pan_offset[1] += delta_y
    
    def zoom(self, delta: float):
        """Zoom camera (change distance)"""
        self.distance *= (1.0 + delta)
        self.distance = np.clip(self.distance, 0.1, 10000.0)
    
    def frame_bounds(self, bounds_min: np.ndarray, bounds_max: np.ndarray):
        """Frame camera to view bounds"""
        center = (bounds_min + bounds_max) * 0.5
        size = np.linalg.norm(bounds_max - bounds_min)
        
        self.target = center
        self.distance = size * 2.0
        self.pan_offset = np.array([0.0, 0.0], dtype=np.float32)
    
    def reset(self):
        """Reset camera to default"""
        self.target = np.array([0.0, 0.0, 0.0], dtype=np.float32)
        self.distance = 10.0
        self.azimuth = 45.0
        self.elevation = 30.0
        self.pan_offset = np.array([0.0, 0.0], dtype=np.float32)
