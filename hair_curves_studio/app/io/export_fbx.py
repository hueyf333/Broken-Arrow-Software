"""Export to FBX format (placeholder - requires FBX SDK)"""

import numpy as np
from pathlib import Path
from typing import Optional
from ..core.logger import logger


def export_fbx(
    filepath: Path,
    vertices: np.ndarray,
    triangles: np.ndarray,
    normals: Optional[np.ndarray] = None,
    uvs: Optional[np.ndarray] = None,
    name: str = "mesh"
) -> bool:
    """
    Export mesh to FBX format
    
    NOTE: Full FBX export requires the FBX SDK which has licensing restrictions
    and complex integration. As a robust alternative, use OBJ or GLTF export.
    
    Both Unreal Engine and Daz Studio support OBJ and GLTF import.
    
    This function logs a message and returns False.
    """
    logger.warning("FBX export not implemented due to SDK licensing restrictions.")
    logger.info("Please use OBJ or GLTF export. Both formats are supported by Unreal Engine and Daz Studio.")
    logger.info("For best results: GLTF for Unreal, OBJ for Daz Studio")
    return False
