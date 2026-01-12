"""Strand generation and interpolation from guides"""

import numpy as np
from typing import List, Optional
from .curves import Curve
from .guides import Guide
from ..core.utils import normalize_vector, lerp_array, distance_3d


class Strand(Curve):
    """Render strand interpolated from guides"""
    
    def __init__(self, points: np.ndarray, root_position: np.ndarray, root_normal: np.ndarray):
        super().__init__(points, root_position, root_normal)
        self.guide_indices: List[int] = []  # Indices of influencing guides
        self.guide_weights: List[float] = []  # Weights for each guide


class StrandGenerator:
    """Generates render strands from guide curves"""
    
    @staticmethod
    def generate_strands(
        guides: List[Guide],
        vertices: np.ndarray,
        triangles: np.ndarray,
        normals: np.ndarray,
        strands_per_guide: int,
        num_points: int,
        interpolation_radius: float,
        seed: int
    ) -> List[Strand]:
        """
        Generate strands interpolated from guides
        
        Args:
            guides: List of guide curves
            vertices: Scalp mesh vertices
            triangles: Scalp mesh triangles
            normals: Scalp normals
            strands_per_guide: Number of strands per guide
            num_points: Points per strand
            interpolation_radius: Max distance for guide influence
            seed: Random seed
        
        Returns:
            List of strand curves
        """
        if not guides:
            return []
        
        rng = np.random.default_rng(seed)
        
        # Build guide position lookup
        guide_roots = np.array([g.root_position for g in guides])
        
        strands = []
        
        for guide_idx, guide in enumerate(guides):
            # Generate strands around this guide
            for i in range(strands_per_guide):
                # Random offset from guide root
                offset_dist = rng.uniform(0, interpolation_radius * 0.3)
                offset_angle = rng.uniform(0, 2 * np.pi)
                
                # Find local tangent plane
                tangent = guide.root_normal
                if abs(tangent[2]) < 0.9:
                    bitangent = normalize_vector(np.cross(tangent, [0, 0, 1]))
                else:
                    bitangent = normalize_vector(np.cross(tangent, [1, 0, 0]))
                
                cotangent = normalize_vector(np.cross(tangent, bitangent))
                
                # Offset position
                root_pos = guide.root_position + \
                          (np.cos(offset_angle) * bitangent + \
                           np.sin(offset_angle) * cotangent) * offset_dist
                
                # Find nearby guides for interpolation
                distances = np.linalg.norm(guide_roots - root_pos, axis=1)
                nearby_indices = np.where(distances < interpolation_radius)[0]
                
                if len(nearby_indices) == 0:
                    nearby_indices = [guide_idx]
                
                # Calculate weights (inverse distance)
                weights = 1.0 / (distances[nearby_indices] + 1e-6)
                weights /= np.sum(weights)
                
                # Interpolate strand from guides
                points = np.zeros((num_points, 3), dtype=np.float32)
                
                for j in range(num_points):
                    t = j / max(1, num_points - 1)
                    
                    # Weighted average of guide points
                    point = np.zeros(3, dtype=np.float32)
                    for k, gi in enumerate(nearby_indices):
                        guide_point = guides[gi].get_point_at(t)
                        point += weights[k] * guide_point
                    
                    points[j] = point
                
                # Create strand
                strand = Strand(points, root_pos, guide.root_normal)
                strand.guide_indices = nearby_indices.tolist()
                strand.guide_weights = weights.tolist()
                strand.width_root = guide.width_root
                strand.width_tip = guide.width_tip
                strand.clump_id = guide.clump_id
                strand.seed = seed + guide_idx * strands_per_guide + i
                
                strands.append(strand)
        
        return strands
    
    @staticmethod
    def interpolate_from_nearest_guides(
        root_position: np.ndarray,
        guides: List[Guide],
        num_neighbors: int,
        num_points: int
    ) -> Optional[Strand]:
        """
        Create a single strand interpolated from nearest guides
        
        Args:
            root_position: Root position for new strand
            guides: Available guides
            num_neighbors: Number of nearest guides to use
            num_points: Points for new strand
        
        Returns:
            Interpolated strand or None
        """
        if not guides:
            return None
        
        # Find nearest guides
        guide_roots = np.array([g.root_position for g in guides])
        distances = np.linalg.norm(guide_roots - root_position, axis=1)
        
        nearest_indices = np.argsort(distances)[:num_neighbors]
        
        # Calculate weights
        nearest_distances = distances[nearest_indices]
        weights = 1.0 / (nearest_distances + 1e-6)
        weights /= np.sum(weights)
        
        # Interpolate points
        points = np.zeros((num_points, 3), dtype=np.float32)
        root_normal = np.zeros(3, dtype=np.float32)
        
        for i in range(num_points):
            t = i / max(1, num_points - 1)
            
            point = np.zeros(3, dtype=np.float32)
            for j, gi in enumerate(nearest_indices):
                guide = guides[gi]
                guide_point = guide.get_point_at(t)
                point += weights[j] * guide_point
                
                if i == 0:
                    root_normal += weights[j] * guide.root_normal
            
            points[i] = point
        
        root_normal = normalize_vector(root_normal)
        
        strand = Strand(points, root_position, root_normal)
        strand.guide_indices = nearest_indices.tolist()
        strand.guide_weights = weights.tolist()
        
        return strand
