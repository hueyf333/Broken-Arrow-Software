"""Tests for sampling and guide generation"""

import numpy as np
import pytest
from hair_curves_studio.app.hair.guides import GuideGenerator
from hair_curves_studio.app.hair.strands import StrandGenerator


def test_uniform_guide_generation():
    """Test uniform guide generation on mesh"""
    # Create simple triangle mesh (single triangle)
    vertices = np.array([
        [0, 0, 0],
        [1, 0, 0],
        [0, 1, 0]
    ], dtype=np.float32)
    
    triangles = np.array([[0, 1, 2]], dtype=np.int32)
    
    normals = np.array([
        [0, 0, 1],
        [0, 0, 1],
        [0, 0, 1]
    ], dtype=np.float32)
    
    # Generate guides
    guides = GuideGenerator.generate_uniform(
        vertices=vertices,
        triangles=triangles,
        normals=normals,
        count=10,
        length=5.0,
        length_randomness=0.1,
        num_points=5,
        seed=42
    )
    
    assert len(guides) == 10
    for guide in guides:
        assert len(guide.points) == 5


def test_strand_generation():
    """Test strand generation from guides"""
    # Create simple triangle mesh
    vertices = np.array([[0, 0, 0], [5, 0, 0], [2.5, 5, 0]], dtype=np.float32)
    triangles = np.array([[0, 1, 2]], dtype=np.int32)
    normals = np.array([[0, 0, 1], [0, 0, 1], [0, 0, 1]], dtype=np.float32)
    
    guides = GuideGenerator.generate_uniform(
        vertices=vertices,
        triangles=triangles,
        normals=normals,
        count=2,
        length=10.0,
        length_randomness=0.0,
        num_points=5,
        seed=42
    )
    
    # Generate strands
    strands = StrandGenerator.generate_strands(
        guides=guides,
        vertices=vertices,
        triangles=triangles,
        normals=normals,
        strands_per_guide=3,
        num_points=5,
        interpolation_radius=10.0,
        seed=42
    )
    
    assert len(strands) > 0
    for strand in strands:
        assert len(strand.points) == 5


def test_strand_interpolation():
    """Test single strand interpolation from guides"""
    vertices = np.array([[0, 0, 0], [5, 0, 0], [2.5, 5, 0]], dtype=np.float32)
    triangles = np.array([[0, 1, 2]], dtype=np.int32)
    normals = np.array([[0, 0, 1], [0, 0, 1], [0, 0, 1]], dtype=np.float32)
    
    guides = GuideGenerator.generate_uniform(
        vertices=vertices,
        triangles=triangles,
        normals=normals,
        count=3,
        length=10.0,
        length_randomness=0.0,
        num_points=5,
        seed=42
    )
    
    root_pos = np.array([0, 0, 0], dtype=np.float32)
    
    strand = StrandGenerator.interpolate_from_nearest_guides(
        root_position=root_pos,
        guides=guides,
        num_neighbors=2,
        num_points=5
    )
    
    assert strand is not None
    assert len(strand.points) == 5


if __name__ == "__main__":
    pytest.main([__file__])
