"""Guide curves for hair system"""

import numpy as np
from typing import List, Optional, Tuple
from .curves import Curve
from ..core.utils import (
    sample_triangle_uniform, triangle_area, triangle_normal,
    normalize_vector
)


class Guide(Curve):
    """Guide curve - user-editable curve that controls strand generation"""
    
    def __init__(self, points: np.ndarray, root_position: np.ndarray, root_normal: np.ndarray):
        super().__init__(points, root_position, root_normal)
        self.selected = False
        self.influence_radius = 10.0  # Radius for strand interpolation
    
    def clone(self) -> 'Guide':
        """Create a copy of this guide"""
        new_guide = Guide(self.points.copy(), self.root_position.copy(), self.root_normal.copy())
        new_guide.width_root = self.width_root
        new_guide.width_tip = self.width_tip
        new_guide.clump_id = self.clump_id
        new_guide.seed = self.seed
        new_guide.influence_radius = self.influence_radius
        return new_guide


class GuideGenerator:
    """Generates guide curves on a scalp mesh"""
    
    @staticmethod
    def generate_uniform(
        vertices: np.ndarray,
        triangles: np.ndarray,
        normals: np.ndarray,
        count: int,
        length: float,
        length_randomness: float,
        num_points: int,
        seed: int
    ) -> List[Guide]:
        """
        Generate guides uniformly distributed on mesh surface
        
        Args:
            vertices: Mesh vertices (V x 3)
            triangles: Mesh triangles (T x 3)
            normals: Vertex normals (V x 3)
            count: Number of guides to generate
            length: Base hair length
            length_randomness: Random variation in length (0-1)
            num_points: Points per guide curve
            seed: Random seed for determinism
        
        Returns:
            List of Guide objects
        """
        rng = np.random.default_rng(seed)
        
        # Calculate triangle areas for weighted sampling
        triangle_areas = np.array([
            triangle_area(
                vertices[tri[0]],
                vertices[tri[1]],
                vertices[tri[2]]
            )
            for tri in triangles
        ])
        
        total_area = np.sum(triangle_areas)
        if total_area < 1e-8:
            return []
        
        triangle_probs = triangle_areas / total_area
        
        guides = []
        for i in range(count):
            # Select random triangle weighted by area
            tri_idx = rng.choice(len(triangles), p=triangle_probs)
            tri = triangles[tri_idx]
            
            # Sample random point in triangle
            root_pos = sample_triangle_uniform(
                vertices[tri[0]],
                vertices[tri[1]],
                vertices[tri[2]],
                rng
            )
            
            # Interpolate normal
            u, v, w = 1.0/3.0, 1.0/3.0, 1.0/3.0  # Simple average for now
            root_normal = normalize_vector(
                u * normals[tri[0]] +
                v * normals[tri[1]] +
                w * normals[tri[2]]
            )
            
            # Random length variation
            local_length = length * (1.0 + rng.uniform(-length_randomness, length_randomness))
            
            # Create curve points along normal
            points = np.zeros((num_points, 3), dtype=np.float32)
            for j in range(num_points):
                t = j / max(1, num_points - 1)
                points[j] = root_pos + root_normal * local_length * t
            
            guide = Guide(points, root_pos, root_normal)
            guide.seed = seed + i
            guides.append(guide)
        
        return guides
    
    @staticmethod
    def generate_from_density_map(
        vertices: np.ndarray,
        triangles: np.ndarray,
        normals: np.ndarray,
        uvs: np.ndarray,
        density_map: np.ndarray,
        target_count: int,
        length: float,
        length_randomness: float,
        num_points: int,
        seed: int
    ) -> List[Guide]:
        """
        Generate guides based on a density texture map
        
        Args:
            vertices: Mesh vertices
            triangles: Mesh triangles
            normals: Vertex normals
            uvs: Vertex UV coordinates (V x 2)
            density_map: Grayscale density image (H x W)
            target_count: Target number of guides
            length: Base hair length
            length_randomness: Length variation
            num_points: Points per curve
            seed: Random seed
        
        Returns:
            List of guides
        """
        rng = np.random.default_rng(seed)
        
        guides = []
        attempts = target_count * 3  # Try multiple times to hit target
        
        for i in range(attempts):
            if len(guides) >= target_count:
                break
            
            # Select random triangle
            tri_idx = rng.integers(0, len(triangles))
            tri = triangles[tri_idx]
            
            # Sample point in triangle
            root_pos = sample_triangle_uniform(
                vertices[tri[0]],
                vertices[tri[1]],
                vertices[tri[2]],
                rng
            )
            
            # Sample UV at this point (simple barycentric)
            uv = (uvs[tri[0]] + uvs[tri[1]] + uvs[tri[2]]) / 3.0
            
            # Sample density map
            u, v = uv[0], uv[1]
            u = np.clip(u, 0.0, 1.0)
            v = np.clip(v, 0.0, 1.0)
            
            h, w = density_map.shape[:2]
            x = int(u * (w - 1))
            y = int((1.0 - v) * (h - 1))  # Flip V
            
            if len(density_map.shape) == 3:
                density = density_map[y, x, 0] / 255.0
            else:
                density = density_map[y, x] / 255.0
            
            # Accept based on density
            if rng.random() > density:
                continue
            
            # Create guide
            root_normal = normalize_vector(
                (normals[tri[0]] + normals[tri[1]] + normals[tri[2]]) / 3.0
            )
            
            local_length = length * (1.0 + rng.uniform(-length_randomness, length_randomness))
            
            points = np.zeros((num_points, 3), dtype=np.float32)
            for j in range(num_points):
                t = j / max(1, num_points - 1)
                points[j] = root_pos + root_normal * local_length * t
            
            guide = Guide(points, root_pos, root_normal)
            guide.seed = seed + i
            guides.append(guide)
        
        return guides
