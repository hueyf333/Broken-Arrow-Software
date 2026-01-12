"""Import mesh files (OBJ, GLTF)"""

import numpy as np
from pathlib import Path
from typing import Tuple, Optional, Dict
import trimesh
from ..core.logger import logger


def import_obj(filepath: Path) -> Optional[Dict]:
    """
    Import OBJ file
    
    Returns:
        Dictionary with vertices, triangles, normals, uvs
    """
    try:
        mesh = trimesh.load(filepath, process=False)
        
        if isinstance(mesh, trimesh.Scene):
            # Take first mesh from scene
            meshes = list(mesh.geometry.values())
            if meshes:
                mesh = meshes[0]
            else:
                logger.error("No meshes found in OBJ scene")
                return None
        
        vertices = np.array(mesh.vertices, dtype=np.float32)
        triangles = np.array(mesh.faces, dtype=np.int32)
        
        # Normals
        if hasattr(mesh, 'vertex_normals') and mesh.vertex_normals is not None:
            normals = np.array(mesh.vertex_normals, dtype=np.float32)
        else:
            mesh.vertex_normals
            normals = np.array(mesh.vertex_normals, dtype=np.float32)
        
        # UVs
        uvs = None
        if hasattr(mesh.visual, 'uv') and mesh.visual.uv is not None:
            uvs = np.array(mesh.visual.uv, dtype=np.float32)
        else:
            # Generate default UVs if missing
            uvs = np.zeros((len(vertices), 2), dtype=np.float32)
        
        logger.info(f"Imported OBJ: {filepath} ({len(vertices)} vertices, {len(triangles)} triangles)")
        
        return {
            "vertices": vertices,
            "triangles": triangles,
            "normals": normals,
            "uvs": uvs,
            "name": filepath.stem
        }
    
    except Exception as e:
        logger.error(f"Failed to import OBJ {filepath}: {e}")
        logger.exception(e)
        return None


def import_gltf(filepath: Path) -> Optional[Dict]:
    """Import GLTF/GLB file"""
    try:
        mesh = trimesh.load(filepath, process=False)
        
        if isinstance(mesh, trimesh.Scene):
            meshes = list(mesh.geometry.values())
            if meshes:
                mesh = meshes[0]
            else:
                logger.error("No meshes found in GLTF scene")
                return None
        
        vertices = np.array(mesh.vertices, dtype=np.float32)
        triangles = np.array(mesh.faces, dtype=np.int32)
        
        # Normals
        if hasattr(mesh, 'vertex_normals'):
            normals = np.array(mesh.vertex_normals, dtype=np.float32)
        else:
            mesh.vertex_normals
            normals = np.array(mesh.vertex_normals, dtype=np.float32)
        
        # UVs
        uvs = None
        if hasattr(mesh.visual, 'uv') and mesh.visual.uv is not None:
            uvs = np.array(mesh.visual.uv, dtype=np.float32)
        else:
            uvs = np.zeros((len(vertices), 2), dtype=np.float32)
        
        logger.info(f"Imported GLTF: {filepath} ({len(vertices)} vertices, {len(triangles)} triangles)")
        
        return {
            "vertices": vertices,
            "triangles": triangles,
            "normals": normals,
            "uvs": uvs,
            "name": filepath.stem
        }
    
    except Exception as e:
        logger.error(f"Failed to import GLTF {filepath}: {e}")
        logger.exception(e)
        return None


def import_mesh(filepath: Path) -> Optional[Dict]:
    """Import mesh file (auto-detect format)"""
    filepath = Path(filepath)
    suffix = filepath.suffix.lower()
    
    if suffix == '.obj':
        return import_obj(filepath)
    elif suffix in ['.gltf', '.glb']:
        return import_gltf(filepath)
    else:
        logger.error(f"Unsupported mesh format: {suffix}")
        return None
