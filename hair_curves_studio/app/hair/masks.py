"""Mask system for hair painting"""

import numpy as np
from typing import Dict, Optional
from PIL import Image


class Mask:
    """Represents a painted mask (density, clump, curl, etc.)"""
    
    def __init__(self, name: str, width: int = 512, height: int = 512):
        self.name = name
        self.width = width
        self.height = height
        self.data = np.zeros((height, width), dtype=np.float32)
    
    def sample_uv(self, u: float, v: float) -> float:
        """Sample mask at UV coordinate"""
        u = np.clip(u, 0.0, 1.0)
        v = np.clip(v, 0.0, 1.0)
        
        x = int(u * (self.width - 1))
        y = int((1.0 - v) * (self.height - 1))
        
        return self.data[y, x]
    
    def paint_uv(self, u: float, v: float, radius: float, value: float, strength: float):
        """Paint on mask at UV coordinate"""
        u = np.clip(u, 0.0, 1.0)
        v = np.clip(v, 0.0, 1.0)
        
        center_x = u * (self.width - 1)
        center_y = (1.0 - v) * (self.height - 1)
        
        radius_pixels = radius * max(self.width, self.height)
        
        for y in range(max(0, int(center_y - radius_pixels)),
                      min(self.height, int(center_y + radius_pixels) + 1)):
            for x in range(max(0, int(center_x - radius_pixels)),
                          min(self.width, int(center_x + radius_pixels) + 1)):
                
                dist = np.sqrt((x - center_x)**2 + (y - center_y)**2)
                
                if dist <= radius_pixels:
                    falloff = 1.0 - (dist / radius_pixels)
                    blend = strength * falloff
                    
                    self.data[y, x] = self.data[y, x] * (1.0 - blend) + value * blend
    
    def save_to_file(self, filepath: str):
        """Save mask as PNG image"""
        image_data = (np.clip(self.data, 0, 1) * 255).astype(np.uint8)
        img = Image.fromarray(image_data, mode='L')
        img.save(filepath)
    
    def load_from_file(self, filepath: str):
        """Load mask from PNG image"""
        img = Image.open(filepath).convert('L')
        self.width = img.width
        self.height = img.height
        self.data = np.array(img, dtype=np.float32) / 255.0
    
    def clear(self, value: float = 0.0):
        """Clear mask to a value"""
        self.data.fill(value)
    
    def to_dict(self) -> dict:
        """Serialize to dictionary"""
        return {
            "name": self.name,
            "width": self.width,
            "height": self.height,
            "data": self.data.tolist()
        }
    
    @staticmethod
    def from_dict(data: dict) -> 'Mask':
        """Deserialize from dictionary"""
        mask = Mask(data["name"], data["width"], data["height"])
        mask.data = np.array(data["data"], dtype=np.float32)
        return mask


class MaskManager:
    """Manages collection of masks"""
    
    def __init__(self):
        self.masks: Dict[str, Mask] = {}
        self.active_mask: Optional[str] = None
    
    def create_mask(self, name: str, width: int = 512, height: int = 512) -> Mask:
        """Create a new mask"""
        mask = Mask(name, width, height)
        self.masks[name] = mask
        if self.active_mask is None:
            self.active_mask = name
        return mask
    
    def get_mask(self, name: str) -> Optional[Mask]:
        """Get mask by name"""
        return self.masks.get(name)
    
    def delete_mask(self, name: str):
        """Delete a mask"""
        if name in self.masks:
            del self.masks[name]
            if self.active_mask == name:
                self.active_mask = next(iter(self.masks.keys())) if self.masks else None
    
    def rename_mask(self, old_name: str, new_name: str):
        """Rename a mask"""
        if old_name in self.masks and new_name not in self.masks:
            self.masks[new_name] = self.masks.pop(old_name)
            self.masks[new_name].name = new_name
            if self.active_mask == old_name:
                self.active_mask = new_name
    
    def get_active_mask(self) -> Optional[Mask]:
        """Get currently active mask"""
        if self.active_mask:
            return self.masks.get(self.active_mask)
        return None
    
    def set_active_mask(self, name: str):
        """Set active mask"""
        if name in self.masks:
            self.active_mask = name
