"""Export meshes to OBJ format"""

import numpy as np
from pathlib import Path
from typing import Optional
from ..core.logger import logger


def export_obj(
    filepath: Path,
    vertices: np.ndarray,
    triangles: np.ndarray,
    normals: Optional[np.ndarray] = None,
    uvs: Optional[np.ndarray] = None,
    name: str = "mesh"
) -> bool:
    """
    Export mesh to OBJ format
    
    Args:
        filepath: Output file path
        vertices: Vertex positions (N x 3)
        triangles: Triangle indices (T x 3)
        normals: Vertex normals (N x 3)
        uvs: Vertex UVs (N x 2)
        name: Object name
    
    Returns:
        True if successful
    """
    try:
        filepath = Path(filepath)
        filepath.parent.mkdir(parents=True, exist_ok=True)
        
        with open(filepath, 'w') as f:
            f.write(f"# Exported from Broken Arrow Hair Curves Studio\n")
            f.write(f"o {name}\n\n")
            
            # Write vertices
            for v in vertices:
                f.write(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}\n")
            f.write("\n")
            
            # Write UVs
            if uvs is not None and len(uvs) > 0:
                for uv in uvs:
                    f.write(f"vt {uv[0]:.6f} {uv[1]:.6f}\n")
                f.write("\n")
            
            # Write normals
            if normals is not None and len(normals) > 0:
                for n in normals:
                    f.write(f"vn {n[0]:.6f} {n[1]:.6f} {n[2]:.6f}\n")
                f.write("\n")
            
            # Write faces
            has_uvs = uvs is not None and len(uvs) > 0
            has_normals = normals is not None and len(normals) > 0
            
            for tri in triangles:
                if has_uvs and has_normals:
                    f.write(f"f {tri[0]+1}/{tri[0]+1}/{tri[0]+1} "
                           f"{tri[1]+1}/{tri[1]+1}/{tri[1]+1} "
                           f"{tri[2]+1}/{tri[2]+1}/{tri[2]+1}\n")
                elif has_uvs:
                    f.write(f"f {tri[0]+1}/{tri[0]+1} "
                           f"{tri[1]+1}/{tri[1]+1} "
                           f"{tri[2]+1}/{tri[2]+1}\n")
                elif has_normals:
                    f.write(f"f {tri[0]+1}//{tri[0]+1} "
                           f"{tri[1]+1}//{tri[1]+1} "
                           f"{tri[2]+1}//{tri[2]+1}\n")
                else:
                    f.write(f"f {tri[0]+1} {tri[1]+1} {tri[2]+1}\n")
        
        logger.info(f"Exported OBJ: {filepath} ({len(vertices)} vertices, {len(triangles)} triangles)")
        return True
    
    except Exception as e:
        logger.error(f"Failed to export OBJ {filepath}: {e}")
        logger.exception(e)
        return False
