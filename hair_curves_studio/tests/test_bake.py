"""Tests for baking system"""

import numpy as np
import pytest
from hair_curves_studio.app.hair.curves import Curve
from hair_curves_studio.app.hair.bake_cards import bake_to_cards
from hair_curves_studio.app.hair.bake_tubes import bake_to_tubes


def test_bake_to_cards():
    """Test hair cards baking"""
    # Create test curves
    curves = []
    for i in range(4):
        points = np.array([
            [i, 0, 0],
            [i, 0, 5],
            [i, 0, 10]
        ], dtype=np.float32)
        root = points[0]
        normal = np.array([0, 0, 1], dtype=np.float32)
        curve = Curve(points, root, normal)
        curves.append(curve)
    
    # Bake to cards
    vertices, normals, uvs, triangles = bake_to_cards(
        curves=curves,
        card_width=1.0,
        segments_per_card=4,
        strands_per_card=2
    )
    
    # Verify output
    assert len(vertices) > 0
    assert len(normals) > 0
    assert len(uvs) > 0
    assert len(triangles) > 0
    
    # Check triangle indices are valid
    assert np.all(triangles >= 0)
    assert np.all(triangles < len(vertices))


def test_bake_to_tubes():
    """Test strand tubes baking"""
    # Create test curves
    curves = []
    for i in range(3):
        points = np.array([
            [i, 0, 0],
            [i, 0, 5],
            [i, 0, 10]
        ], dtype=np.float32)
        root = points[0]
        normal = np.array([0, 0, 1], dtype=np.float32)
        curve = Curve(points, root, normal)
        curves.append(curve)
    
    # Bake to tubes
    vertices, normals, uvs, triangles = bake_to_tubes(
        curves=curves,
        tube_sides=6,
        segments_per_curve=8
    )
    
    # Verify output
    assert len(vertices) > 0
    assert len(normals) > 0
    assert len(uvs) > 0
    assert len(triangles) > 0
    
    # Check triangle indices are valid
    assert np.all(triangles >= 0)
    assert np.all(triangles < len(vertices))


def test_bake_empty_curves():
    """Test baking with empty curve list"""
    vertices, normals, uvs, triangles = bake_to_cards([], 1.0, 4, 2)
    
    assert len(vertices) == 0
    assert len(normals) == 0
    assert len(uvs) == 0
    assert len(triangles) == 0


def test_bake_output_consistency():
    """Test that baked output has consistent vertex/triangle counts"""
    curves = []
    for i in range(8):
        points = np.array([[i, 0, j] for j in range(0, 11, 2)], dtype=np.float32)
        root = points[0]
        normal = np.array([0, 0, 1], dtype=np.float32)
        curve = Curve(points, root, normal)
        curves.append(curve)
    
    vertices, normals, uvs, triangles = bake_to_cards(curves, 1.0, 4, 4)
    
    # All vertex attributes should have same length
    assert len(vertices) == len(normals)
    assert len(vertices) == len(uvs)
    
    # Triangles should have correct shape
    assert triangles.shape[1] == 3


if __name__ == "__main__":
    pytest.main([__file__])
