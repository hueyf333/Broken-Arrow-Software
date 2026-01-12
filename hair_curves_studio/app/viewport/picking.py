"""3D picking and selection system"""

import numpy as np
from typing import Optional, Tuple, List


def screen_to_ray(
    screen_x: int,
    screen_y: int,
    screen_width: int,
    screen_height: int,
    view_matrix: np.ndarray,
    projection_matrix: np.ndarray
) -> Tuple[np.ndarray, np.ndarray]:
    """
    Convert screen coordinates to world ray
    
    Returns:
        (ray_origin, ray_direction) in world space
    """
    # Normalize screen coordinates to NDC
    ndc_x = (2.0 * screen_x) / screen_width - 1.0
    ndc_y = 1.0 - (2.0 * screen_y) / screen_height
    
    # Clip space
    clip = np.array([ndc_x, ndc_y, -1.0, 1.0], dtype=np.float32)
    
    # Eye space
    proj_inv = np.linalg.inv(projection_matrix)
    eye = proj_inv @ clip
    eye[2] = -1.0
    eye[3] = 0.0
    
    # World space
    view_inv = np.linalg.inv(view_matrix)
    world = view_inv @ eye
    
    ray_direction = world[:3]
    ray_direction = ray_direction / (np.linalg.norm(ray_direction) + 1e-8)
    
    ray_origin = view_inv[:3, 3]
    
    return ray_origin, ray_direction


def ray_triangle_intersection(
    ray_origin: np.ndarray,
    ray_direction: np.ndarray,
    v0: np.ndarray,
    v1: np.ndarray,
    v2: np.ndarray
) -> Optional[float]:
    """
    Ray-triangle intersection (Möller–Trumbore algorithm)
    
    Returns:
        Distance along ray if hit, None if miss
    """
    epsilon = 1e-8
    
    edge1 = v1 - v0
    edge2 = v2 - v0
    h = np.cross(ray_direction, edge2)
    a = np.dot(edge1, h)
    
    if abs(a) < epsilon:
        return None
    
    f = 1.0 / a
    s = ray_origin - v0
    u = f * np.dot(s, h)
    
    if u < 0.0 or u > 1.0:
        return None
    
    q = np.cross(s, edge1)
    v = f * np.dot(ray_direction, q)
    
    if v < 0.0 or u + v > 1.0:
        return None
    
    t = f * np.dot(edge2, q)
    
    if t > epsilon:
        return t
    
    return None


def ray_mesh_intersection(
    ray_origin: np.ndarray,
    ray_direction: np.ndarray,
    vertices: np.ndarray,
    triangles: np.ndarray
) -> Optional[Tuple[int, float, np.ndarray]]:
    """
    Find closest triangle hit by ray
    
    Returns:
        (triangle_index, distance, hit_point) or None
    """
    closest_dist = float('inf')
    closest_tri = None
    closest_point = None
    
    for i, tri in enumerate(triangles):
        v0 = vertices[tri[0]]
        v1 = vertices[tri[1]]
        v2 = vertices[tri[2]]
        
        dist = ray_triangle_intersection(ray_origin, ray_direction, v0, v1, v2)
        
        if dist is not None and dist < closest_dist:
            closest_dist = dist
            closest_tri = i
            closest_point = ray_origin + ray_direction * dist
    
    if closest_tri is not None:
        return (closest_tri, closest_dist, closest_point)
    
    return None
