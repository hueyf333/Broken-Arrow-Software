"""Utility functions for the application"""

import numpy as np
from typing import Tuple, List, Optional
import hashlib
import json


def normalize_vector(v: np.ndarray) -> np.ndarray:
    """Normalize a vector"""
    length = np.linalg.norm(v)
    if length < 1e-8:
        return v
    return v / length


def distance_3d(p1: np.ndarray, p2: np.ndarray) -> float:
    """Calculate 3D distance between two points"""
    return np.linalg.norm(p2 - p1)


def lerp(a: float, b: float, t: float) -> float:
    """Linear interpolation"""
    return a + (b - a) * t


def clamp(value: float, min_val: float, max_val: float) -> float:
    """Clamp value between min and max"""
    return max(min_val, min(max_val, value))


def smooth_step(t: float) -> float:
    """Smooth step interpolation (0 to 1)"""
    t = clamp(t, 0.0, 1.0)
    return t * t * (3.0 - 2.0 * t)


def generate_seed_from_string(s: str) -> int:
    """Generate a deterministic seed from a string"""
    hash_obj = hashlib.md5(s.encode())
    return int(hash_obj.hexdigest(), 16) % (2**31)


def barycentric_coords(p: np.ndarray, a: np.ndarray, b: np.ndarray, c: np.ndarray) -> Tuple[float, float, float]:
    """Calculate barycentric coordinates of point p in triangle abc"""
    v0 = b - a
    v1 = c - a
    v2 = p - a
    
    d00 = np.dot(v0, v0)
    d01 = np.dot(v0, v1)
    d11 = np.dot(v1, v1)
    d20 = np.dot(v2, v0)
    d21 = np.dot(v2, v1)
    
    denom = d00 * d11 - d01 * d01
    if abs(denom) < 1e-8:
        return (1.0, 0.0, 0.0)
    
    v = (d11 * d20 - d01 * d21) / denom
    w = (d00 * d21 - d01 * d20) / denom
    u = 1.0 - v - w
    
    return (u, v, w)


def point_in_triangle_2d(p: np.ndarray, a: np.ndarray, b: np.ndarray, c: np.ndarray) -> bool:
    """Check if point p is inside triangle abc in 2D"""
    u, v, w = barycentric_coords(p, a, b, c)
    return u >= 0 and v >= 0 and w >= 0


def sample_triangle_uniform(a: np.ndarray, b: np.ndarray, c: np.ndarray, rng: np.random.Generator) -> np.ndarray:
    """Sample a random point uniformly in a triangle"""
    r1 = rng.random()
    r2 = rng.random()
    
    sqrt_r1 = np.sqrt(r1)
    u = 1.0 - sqrt_r1
    v = sqrt_r1 * (1.0 - r2)
    w = sqrt_r1 * r2
    
    return u * a + v * b + w * c


def triangle_area(a: np.ndarray, b: np.ndarray, c: np.ndarray) -> float:
    """Calculate area of triangle"""
    return 0.5 * np.linalg.norm(np.cross(b - a, c - a))


def triangle_normal(a: np.ndarray, b: np.ndarray, c: np.ndarray) -> np.ndarray:
    """Calculate normal of triangle"""
    return normalize_vector(np.cross(b - a, c - a))


def euler_to_matrix(angles: Tuple[float, float, float]) -> np.ndarray:
    """Convert Euler angles (XYZ) to rotation matrix"""
    rx, ry, rz = angles
    
    # Rotation around X
    cx, sx = np.cos(rx), np.sin(rx)
    mx = np.array([
        [1, 0, 0],
        [0, cx, -sx],
        [0, sx, cx]
    ])
    
    # Rotation around Y
    cy, sy = np.cos(ry), np.sin(ry)
    my = np.array([
        [cy, 0, sy],
        [0, 1, 0],
        [-sy, 0, cy]
    ])
    
    # Rotation around Z
    cz, sz = np.cos(rz), np.sin(rz)
    mz = np.array([
        [cz, -sz, 0],
        [sz, cz, 0],
        [0, 0, 1]
    ])
    
    return mz @ my @ mx


def matrix_to_euler(m: np.ndarray) -> Tuple[float, float, float]:
    """Convert rotation matrix to Euler angles (XYZ)"""
    sy = np.sqrt(m[0, 0]**2 + m[1, 0]**2)
    
    singular = sy < 1e-6
    
    if not singular:
        x = np.arctan2(m[2, 1], m[2, 2])
        y = np.arctan2(-m[2, 0], sy)
        z = np.arctan2(m[1, 0], m[0, 0])
    else:
        x = np.arctan2(-m[1, 2], m[1, 1])
        y = np.arctan2(-m[2, 0], sy)
        z = 0
    
    return (x, y, z)


def create_transform_matrix(translation: np.ndarray, rotation: Tuple[float, float, float], scale: np.ndarray) -> np.ndarray:
    """Create a 4x4 transformation matrix"""
    mat = np.eye(4)
    
    # Rotation
    rot_mat = euler_to_matrix(rotation)
    mat[:3, :3] = rot_mat
    
    # Scale
    mat[:3, :3] *= scale
    
    # Translation
    mat[:3, 3] = translation
    
    return mat


def safe_json_dump(obj) -> str:
    """Safely dump object to JSON, converting numpy types"""
    def convert(o):
        if isinstance(o, np.ndarray):
            return o.tolist()
        if isinstance(o, (np.int64, np.int32)):
            return int(o)
        if isinstance(o, (np.float64, np.float32)):
            return float(o)
        if isinstance(o, np.bool_):
            return bool(o)
        raise TypeError(f"Object of type {type(o)} is not JSON serializable")
    
    return json.dumps(obj, default=convert, indent=2)


def compute_curve_length(points: np.ndarray) -> float:
    """Compute total length of a curve defined by points"""
    if len(points) < 2:
        return 0.0
    
    segments = points[1:] - points[:-1]
    lengths = np.linalg.norm(segments, axis=1)
    return np.sum(lengths)


def resample_curve_uniform(points: np.ndarray, num_samples: int) -> np.ndarray:
    """Resample curve to have uniform spacing"""
    if len(points) < 2:
        return points
    
    if num_samples < 2:
        num_samples = 2
    
    # Compute cumulative arc lengths
    segments = points[1:] - points[:-1]
    segment_lengths = np.linalg.norm(segments, axis=1)
    cumulative_lengths = np.concatenate([[0], np.cumsum(segment_lengths)])
    total_length = cumulative_lengths[-1]
    
    if total_length < 1e-8:
        return np.tile(points[0], (num_samples, 1))
    
    # Sample at uniform intervals
    target_lengths = np.linspace(0, total_length, num_samples)
    new_points = np.zeros((num_samples, 3))
    
    for i, target_length in enumerate(target_lengths):
        # Find segment containing this arc length
        segment_idx = np.searchsorted(cumulative_lengths, target_length) - 1
        segment_idx = max(0, min(segment_idx, len(points) - 2))
        
        # Interpolate within segment
        segment_start_length = cumulative_lengths[segment_idx]
        segment_length = segment_lengths[segment_idx]
        
        if segment_length < 1e-8:
            t = 0.0
        else:
            t = (target_length - segment_start_length) / segment_length
        
        new_points[i] = lerp_array(points[segment_idx], points[segment_idx + 1], t)
    
    return new_points


def lerp_array(a: np.ndarray, b: np.ndarray, t: float) -> np.ndarray:
    """Linear interpolation for arrays"""
    return a + (b - a) * t
