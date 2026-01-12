"""Export meshes to GLTF/GLB format"""

import numpy as np
from pathlib import Path
from typing import Optional
from pygltflib import GLTF2, Buffer, BufferView, Accessor, Mesh, Primitive, Asset, Scene, Node
from pygltflib import ARRAY_BUFFER, ELEMENT_ARRAY_BUFFER, FLOAT, UNSIGNED_INT
from ..core.logger import logger
import struct


def export_gltf(
    filepath: Path,
    vertices: np.ndarray,
    triangles: np.ndarray,
    normals: Optional[np.ndarray] = None,
    uvs: Optional[np.ndarray] = None,
    name: str = "mesh",
    binary: bool = False
) -> bool:
    """
    Export mesh to GLTF/GLB format
    
    Args:
        filepath: Output file path
        vertices: Vertex positions (N x 3)
        triangles: Triangle indices (T x 3)
        normals: Vertex normals (N x 3)
        uvs: Vertex UVs (N x 2)
        name: Mesh name
        binary: True for GLB, False for GLTF
    
    Returns:
        True if successful
    """
    try:
        filepath = Path(filepath)
        filepath.parent.mkdir(parents=True, exist_ok=True)
        
        # Create GLTF object
        gltf = GLTF2()
        gltf.asset = Asset(version="2.0", generator="Broken Arrow Hair Curves Studio")
        
        # Prepare binary data
        binary_data = bytearray()
        buffer_views = []
        accessors = []
        
        # Vertices
        vertices_bytes = vertices.astype(np.float32).tobytes()
        buffer_views.append(BufferView(
            buffer=0,
            byteOffset=len(binary_data),
            byteLength=len(vertices_bytes),
            target=ARRAY_BUFFER
        ))
        accessors.append(Accessor(
            bufferView=len(buffer_views) - 1,
            componentType=FLOAT,
            count=len(vertices),
            type="VEC3",
            min=vertices.min(axis=0).tolist(),
            max=vertices.max(axis=0).tolist()
        ))
        binary_data.extend(vertices_bytes)
        position_accessor = len(accessors) - 1
        
        # Normals
        normal_accessor = None
        if normals is not None:
            normals_bytes = normals.astype(np.float32).tobytes()
            buffer_views.append(BufferView(
                buffer=0,
                byteOffset=len(binary_data),
                byteLength=len(normals_bytes),
                target=ARRAY_BUFFER
            ))
            accessors.append(Accessor(
                bufferView=len(buffer_views) - 1,
                componentType=FLOAT,
                count=len(normals),
                type="VEC3"
            ))
            binary_data.extend(normals_bytes)
            normal_accessor = len(accessors) - 1
        
        # UVs
        uv_accessor = None
        if uvs is not None:
            uvs_bytes = uvs.astype(np.float32).tobytes()
            buffer_views.append(BufferView(
                buffer=0,
                byteOffset=len(binary_data),
                byteLength=len(uvs_bytes),
                target=ARRAY_BUFFER
            ))
            accessors.append(Accessor(
                bufferView=len(buffer_views) - 1,
                componentType=FLOAT,
                count=len(uvs),
                type="VEC2"
            ))
            binary_data.extend(uvs_bytes)
            uv_accessor = len(accessors) - 1
        
        # Indices
        indices_bytes = triangles.flatten().astype(np.uint32).tobytes()
        buffer_views.append(BufferView(
            buffer=0,
            byteOffset=len(binary_data),
            byteLength=len(indices_bytes),
            target=ELEMENT_ARRAY_BUFFER
        ))
        accessors.append(Accessor(
            bufferView=len(buffer_views) - 1,
            componentType=UNSIGNED_INT,
            count=len(triangles) * 3,
            type="SCALAR"
        ))
        binary_data.extend(indices_bytes)
        indices_accessor = len(accessors) - 1
        
        # Create primitive
        primitive = Primitive(
            attributes={"POSITION": position_accessor},
            indices=indices_accessor
        )
        
        if normal_accessor is not None:
            primitive.attributes["NORMAL"] = normal_accessor
        if uv_accessor is not None:
            primitive.attributes["TEXCOORD_0"] = uv_accessor
        
        # Create mesh
        mesh = Mesh(primitives=[primitive], name=name)
        
        # Set GLTF data
        gltf.buffers = [Buffer(byteLength=len(binary_data))]
        gltf.bufferViews = buffer_views
        gltf.accessors = accessors
        gltf.meshes = [mesh]
        gltf.nodes = [Node(mesh=0)]
        gltf.scenes = [Scene(nodes=[0])]
        gltf.scene = 0
        
        # Set binary data
        gltf.set_binary_blob(bytes(binary_data))
        
        # Save
        if binary:
            gltf.save(str(filepath))
        else:
            gltf.save(str(filepath))
        
        logger.info(f"Exported GLTF: {filepath} ({len(vertices)} vertices, {len(triangles)} triangles)")
        return True
    
    except Exception as e:
        logger.error(f"Failed to export GLTF {filepath}: {e}")
        logger.exception(e)
        return False
