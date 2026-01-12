"""Bake hair to card meshes for game engines"""

import numpy as np
from typing import List, Tuple, Dict
from .curves import Curve


class HairCard:
    """Represents a hair card mesh strip"""
    
    def __init__(self):
        self.vertices: np.ndarray = np.array([], dtype=np.float32)
        self.normals: np.ndarray = np.array([], dtype=np.float32)
        self.uvs: np.ndarray = np.array([], dtype=np.float32)
        self.triangles: np.ndarray = np.array([], dtype=np.int32)


def bake_to_cards(
    curves: List[Curve],
    card_width: float = 1.0,
    segments_per_card: int = 8,
    strands_per_card: int = 4
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """
    Bake hair curves to card meshes
    
    Args:
        curves: List of hair curves
        card_width: Width of each card
        segments_per_card: Number of segments along card length
        strands_per_card: Number of strands to cluster per card
    
    Returns:
        Tuple of (vertices, normals, uvs, triangles)
    """
    if not curves:
        return (np.array([]), np.array([]), np.array([]), np.array([]))
    
    all_vertices = []
    all_normals = []
    all_uvs = []
    all_triangles = []
    vertex_offset = 0
    
    # Group curves into cards
    num_cards = max(1, len(curves) // strands_per_card)
    
    for card_idx in range(num_cards):
        start_idx = card_idx * strands_per_card
        end_idx = min(start_idx + strands_per_card, len(curves))
        card_curves = curves[start_idx:end_idx]
        
        if not card_curves:
            continue
        
        # Use first curve as centerline
        center_curve = card_curves[0]
        
        if len(center_curve.points) < 2:
            continue
        
        # Resample to segments
        num_points = segments_per_card + 1
        if len(center_curve.points) != num_points:
            center_curve.resample(num_points)
        
        # Generate card vertices
        card_verts = []
        card_norms = []
        card_uvs = []
        
        for i in range(num_points):
            t = i / max(1, num_points - 1)
            center = center_curve.points[i]
            tangent = center_curve.get_tangent_at(t)
            
            # Find perpendicular direction
            if abs(tangent[2]) < 0.9:
                right = np.cross(tangent, [0, 0, 1])
            else:
                right = np.cross(tangent, [1, 0, 0])
            
            right = right / (np.linalg.norm(right) + 1e-8)
            
            # Width taper
            width = card_width * (center_curve.width_root * (1 - t) + center_curve.width_tip * t)
            
            # Left and right vertices
            left_vert = center - right * width * 0.5
            right_vert = center + right * width * 0.5
            
            card_verts.append(left_vert)
            card_verts.append(right_vert)
            
            # Normals (facing camera, perpendicular to tangent and right)
            normal = np.cross(tangent, right)
            normal = normal / (np.linalg.norm(normal) + 1e-8)
            
            card_norms.append(normal)
            card_norms.append(normal)
            
            # UVs
            card_uvs.append([0.0, t])
            card_uvs.append([1.0, t])
        
        # Generate triangles
        for i in range(segments_per_card):
            base = vertex_offset + i * 2
            
            # Triangle 1
            all_triangles.append([base, base + 2, base + 1])
            # Triangle 2
            all_triangles.append([base + 1, base + 2, base + 3])
        
        all_vertices.extend(card_verts)
        all_normals.extend(card_norms)
        all_uvs.extend(card_uvs)
        
        vertex_offset += len(card_verts)
    
    vertices = np.array(all_vertices, dtype=np.float32)
    normals = np.array(all_normals, dtype=np.float32)
    uvs = np.array(all_uvs, dtype=np.float32)
    triangles = np.array(all_triangles, dtype=np.int32)
    
    return vertices, normals, uvs, triangles
