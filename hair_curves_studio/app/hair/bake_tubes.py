"""Bake hair to tube/ribbon meshes"""

import numpy as np
from typing import List, Tuple
from .curves import Curve


def bake_to_tubes(
    curves: List[Curve],
    tube_sides: int = 4,
    segments_per_curve: int = 16
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """
    Bake hair curves to tube meshes
    
    Args:
        curves: List of hair curves
        tube_sides: Number of sides for tube (3=triangular, 4=quad, 8=octagonal, etc.)
        segments_per_curve: Number of segments along each curve
    
    Returns:
        Tuple of (vertices, normals, uvs, triangles)
    """
    if not curves or tube_sides < 3:
        return (np.array([]), np.array([]), np.array([]), np.array([]))
    
    all_vertices = []
    all_normals = []
    all_uvs = []
    all_triangles = []
    vertex_offset = 0
    
    for curve in curves:
        if len(curve.points) < 2:
            continue
        
        # Resample curve
        if len(curve.points) != segments_per_curve:
            curve.resample(segments_per_curve)
        
        # Generate tube vertices
        for i in range(len(curve.points)):
            t = i / max(1, len(curve.points) - 1)
            center = curve.points[i]
            tangent = curve.get_tangent_at(t)
            
            # Find perpendicular vectors
            if abs(tangent[2]) < 0.9:
                perp1 = np.cross(tangent, [0, 0, 1])
            else:
                perp1 = np.cross(tangent, [1, 0, 0])
            
            perp1 = perp1 / (np.linalg.norm(perp1) + 1e-8)
            perp2 = np.cross(tangent, perp1)
            perp2 = perp2 / (np.linalg.norm(perp2) + 1e-8)
            
            # Width taper
            radius = (curve.width_root * (1 - t) + curve.width_tip * t) * 0.5
            
            # Create ring of vertices
            for j in range(tube_sides):
                angle = (j / tube_sides) * 2 * np.pi
                offset = (np.cos(angle) * perp1 + np.sin(angle) * perp2) * radius
                
                vert = center + offset
                normal = offset / (np.linalg.norm(offset) + 1e-8)
                
                all_vertices.append(vert)
                all_normals.append(normal)
                all_uvs.append([j / tube_sides, t])
        
        # Generate triangles
        for i in range(len(curve.points) - 1):
            for j in range(tube_sides):
                j_next = (j + 1) % tube_sides
                
                v0 = vertex_offset + i * tube_sides + j
                v1 = vertex_offset + i * tube_sides + j_next
                v2 = vertex_offset + (i + 1) * tube_sides + j
                v3 = vertex_offset + (i + 1) * tube_sides + j_next
                
                # Triangle 1
                all_triangles.append([v0, v2, v1])
                # Triangle 2
                all_triangles.append([v1, v2, v3])
        
        vertex_offset += len(curve.points) * tube_sides
    
    vertices = np.array(all_vertices, dtype=np.float32)
    normals = np.array(all_normals, dtype=np.float32)
    uvs = np.array(all_uvs, dtype=np.float32)
    triangles = np.array(all_triangles, dtype=np.int32)
    
    return vertices, normals, uvs, triangles
