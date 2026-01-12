"""Tests for spline evaluation and curve utilities"""

import numpy as np
import pytest
from hair_curves_studio.app.hair.curves import Curve, catmull_rom_spline, bezier_cubic
from hair_curves_studio.app.core.utils import compute_curve_length, resample_curve_uniform


def test_curve_creation():
    """Test basic curve creation"""
    points = np.array([[0, 0, 0], [0, 0, 5], [0, 0, 10]], dtype=np.float32)
    root = np.array([0, 0, 0], dtype=np.float32)
    normal = np.array([0, 0, 1], dtype=np.float32)
    
    curve = Curve(points, root, normal)
    
    assert len(curve.points) == 3
    assert np.allclose(curve.root_position, root)
    assert np.allclose(curve.root_normal, normal)


def test_curve_length():
    """Test curve length computation"""
    points = np.array([[0, 0, 0], [0, 0, 5], [0, 0, 10]], dtype=np.float32)
    root = np.array([0, 0, 0], dtype=np.float32)
    normal = np.array([0, 0, 1], dtype=np.float32)
    
    curve = Curve(points, root, normal)
    length = curve.get_length()
    
    assert np.isclose(length, 10.0)


def test_curve_resample():
    """Test curve resampling"""
    points = np.array([[0, 0, 0], [0, 0, 10]], dtype=np.float32)
    root = np.array([0, 0, 0], dtype=np.float32)
    normal = np.array([0, 0, 1], dtype=np.float32)
    
    curve = Curve(points, root, normal)
    curve.resample(5)
    
    assert len(curve.points) == 5
    assert np.allclose(curve.points[0], [0, 0, 0])
    assert np.allclose(curve.points[-1], [0, 0, 10])


def test_catmull_rom_spline():
    """Test Catmull-Rom spline evaluation"""
    p0 = np.array([0, 0, 0], dtype=np.float32)
    p1 = np.array([0, 0, 5], dtype=np.float32)
    p2 = np.array([0, 0, 10], dtype=np.float32)
    p3 = np.array([0, 0, 15], dtype=np.float32)
    
    # At t=0, should be at p1
    result = catmull_rom_spline(p0, p1, p2, p3, 0.0)
    assert np.allclose(result, p1)
    
    # At t=1, should be at p2
    result = catmull_rom_spline(p0, p1, p2, p3, 1.0)
    assert np.allclose(result, p2)


def test_bezier_cubic():
    """Test cubic Bezier curve evaluation"""
    p0 = np.array([0, 0, 0], dtype=np.float32)
    p1 = np.array([0, 0, 3], dtype=np.float32)
    p2 = np.array([0, 0, 7], dtype=np.float32)
    p3 = np.array([0, 0, 10], dtype=np.float32)
    
    # At t=0, should be at p0
    result = bezier_cubic(p0, p1, p2, p3, 0.0)
    assert np.allclose(result, p0)
    
    # At t=1, should be at p3
    result = bezier_cubic(p0, p1, p2, p3, 1.0)
    assert np.allclose(result, p3)


def test_curve_serialization():
    """Test curve to_dict and from_dict"""
    points = np.array([[0, 0, 0], [0, 0, 5], [0, 0, 10]], dtype=np.float32)
    root = np.array([0, 0, 0], dtype=np.float32)
    normal = np.array([0, 0, 1], dtype=np.float32)
    
    curve = Curve(points, root, normal)
    curve.width_root = 2.0
    curve.width_tip = 0.5
    
    # Serialize
    data = curve.to_dict()
    
    # Deserialize
    curve2 = Curve.from_dict(data)
    
    assert np.allclose(curve.points, curve2.points)
    assert np.allclose(curve.root_position, curve2.root_position)
    assert np.isclose(curve.width_root, curve2.width_root)
    assert np.isclose(curve.width_tip, curve2.width_tip)


if __name__ == "__main__":
    pytest.main([__file__])
