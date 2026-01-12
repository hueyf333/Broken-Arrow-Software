"""OpenGL viewport widget using ModernGL"""

import moderngl
import numpy as np
from pathlib import Path
from PySide6.QtWidgets import QOpenGLWidget
from PySide6.QtCore import Qt, QPoint
from PySide6.QtGui import QSurfaceFormat
from .camera import Camera
from ..core.logger import logger


class ViewportWidget(QOpenGLWidget):
    """3D OpenGL viewport for rendering hair and meshes"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        # Set OpenGL format
        fmt = QSurfaceFormat()
        fmt.setVersion(3, 3)
        fmt.setProfile(QSurfaceFormat.CoreProfile)
        fmt.setDepthBufferSize(24)
        fmt.setStencilBufferSize(8)
        fmt.setSamples(4)  # MSAA
        self.setFormat(fmt)
        
        # Camera
        self.camera = Camera()
        
        # Mouse interaction
        self.last_mouse_pos = QPoint()
        self.mouse_button = Qt.NoButton
        
        # ModernGL context
        self.ctx = None
        self.mesh_program = None
        self.hair_program = None
        
        # Render data
        self.mesh_vaos = {}
        self.hair_vaos = {}
        
        # Render settings
        self.show_wireframe = False
        self.show_guides = True
        self.show_strands = True
        self.background_color = (0.2, 0.2, 0.25, 1.0)
    
    def initializeGL(self):
        """Initialize OpenGL context"""
        try:
            self.ctx = moderngl.create_context()
            self.ctx.enable(moderngl.DEPTH_TEST)
            self.ctx.enable(moderngl.BLEND)
            self.ctx.blend_func = moderngl.SRC_ALPHA, moderngl.ONE_MINUS_SRC_ALPHA
            
            # Load shaders
            self._load_shaders()
            
            logger.info(f"OpenGL initialized: {self.ctx.info.get('GL_VERSION', 'unknown')}")
        
        except Exception as e:
            logger.error(f"Failed to initialize OpenGL: {e}")
            logger.exception(e)
    
    def _load_shaders(self):
        """Load shader programs"""
        shader_dir = Path(__file__).parent / "shaders"
        
        try:
            # Mesh shader
            with open(shader_dir / "mesh_vertex.glsl") as f:
                mesh_vs = f.read()
            with open(shader_dir / "mesh_fragment.glsl") as f:
                mesh_fs = f.read()
            
            self.mesh_program = self.ctx.program(
                vertex_shader=mesh_vs,
                fragment_shader=mesh_fs
            )
            
            # Hair shader
            with open(shader_dir / "hair_vertex.glsl") as f:
                hair_vs = f.read()
            with open(shader_dir / "hair_fragment.glsl") as f:
                hair_fs = f.read()
            
            self.hair_program = self.ctx.program(
                vertex_shader=hair_vs,
                fragment_shader=hair_fs
            )
            
            logger.debug("Shaders loaded successfully")
        
        except Exception as e:
            logger.error(f"Failed to load shaders: {e}")
            logger.exception(e)
    
    def resizeGL(self, w, h):
        """Handle viewport resize"""
        if self.ctx:
            self.ctx.viewport = (0, 0, w, h)
            self.camera.aspect_ratio = w / max(1, h)
    
    def paintGL(self):
        """Render the scene"""
        if not self.ctx:
            return
        
        # Clear
        self.ctx.clear(*self.background_color)
        
        # Get matrices
        view = self.camera.get_view_matrix()
        projection = self.camera.get_projection_matrix()
        
        # Render meshes
        if self.mesh_program:
            self.mesh_program['u_view'].write(view.tobytes())
            self.mesh_program['u_projection'].write(projection.tobytes())
            self.mesh_program['u_light_dir'].write(np.array([1, 1, 1], dtype=np.float32).tobytes())
            self.mesh_program['u_light_color'].write(np.array([1, 1, 1], dtype=np.float32).tobytes())
            self.mesh_program['u_ambient'].value = 0.3
            self.mesh_program['u_wireframe'].value = self.show_wireframe
            
            for name, vao in self.mesh_vaos.items():
                if vao:
                    model = np.eye(4, dtype=np.float32)
                    self.mesh_program['u_model'].write(model.tobytes())
                    self.mesh_program['u_base_color'].write(np.array([0.7, 0.7, 0.7], dtype=np.float32).tobytes())
                    
                    if self.show_wireframe:
                        vao.render(moderngl.LINE_STRIP)
                    else:
                        vao.render(moderngl.TRIANGLES)
        
        # Render hair
        if self.hair_program and (self.show_guides or self.show_strands):
            self.hair_program['u_view'].write(view.tobytes())
            self.hair_program['u_projection'].write(projection.tobytes())
            self.hair_program['u_hair_color'].write(np.array([0.3, 0.2, 0.1], dtype=np.float32).tobytes())
            
            for name, vao in self.hair_vaos.items():
                if vao:
                    vao.render(moderngl.LINE_STRIP)
    
    def add_mesh(self, name: str, vertices: np.ndarray, triangles: np.ndarray, normals: np.ndarray, uvs: np.ndarray):
        """Add a mesh to the viewport"""
        if not self.ctx:
            return
        
        try:
            # Create vertex buffer
            vbo_data = np.column_stack([
                vertices,
                normals,
                uvs
            ]).astype(np.float32).tobytes()
            
            vbo = self.ctx.buffer(vbo_data)
            ibo = self.ctx.buffer(triangles.astype(np.int32).tobytes())
            
            vao = self.ctx.vertex_array(
                self.mesh_program,
                [
                    (vbo, '3f 3f 2f', 'in_position', 'in_normal', 'in_uv')
                ],
                ibo
            )
            
            self.mesh_vaos[name] = vao
            logger.debug(f"Added mesh to viewport: {name}")
        
        except Exception as e:
            logger.error(f"Failed to add mesh {name}: {e}")
            logger.exception(e)
    
    def clear_meshes(self):
        """Clear all meshes"""
        for vao in self.mesh_vaos.values():
            if vao:
                vao.release()
        self.mesh_vaos.clear()
    
    def mousePressEvent(self, event):
        """Handle mouse press"""
        self.last_mouse_pos = event.pos()
        self.mouse_button = event.button()
    
    def mouseMoveEvent(self, event):
        """Handle mouse move"""
        delta = event.pos() - self.last_mouse_pos
        self.last_mouse_pos = event.pos()
        
        if self.mouse_button == Qt.LeftButton:
            # Orbit
            self.camera.orbit(delta.x() * 0.5, delta.y() * 0.5)
            self.update()
        
        elif self.mouse_button == Qt.MiddleButton:
            # Pan
            sensitivity = 0.01
            self.camera.pan(delta.x() * sensitivity, -delta.y() * sensitivity)
            self.update()
    
    def wheelEvent(self, event):
        """Handle mouse wheel"""
        delta = event.angleDelta().y()
        zoom_factor = -delta * 0.001
        self.camera.zoom(zoom_factor)
        self.update()
    
    def frame_all(self):
        """Frame camera to view all content"""
        # Calculate bounds of all meshes
        bounds_min = np.array([float('inf')] * 3)
        bounds_max = np.array([float('-inf')] * 3)
        
        # TODO: Track mesh bounds
        # For now, use default bounds
        bounds_min = np.array([-5, -5, -5])
        bounds_max = np.array([5, 5, 5])
        
        self.camera.frame_bounds(bounds_min, bounds_max)
        self.update()
