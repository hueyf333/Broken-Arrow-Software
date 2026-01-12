"""Texture management and export"""

import numpy as np
from pathlib import Path
from PIL import Image
from typing import Optional
from ..core.logger import logger


def save_texture(
    filepath: Path,
    data: np.ndarray,
    format: str = "PNG"
) -> bool:
    """
    Save texture to file
    
    Args:
        filepath: Output path
        data: Texture data (H x W) or (H x W x C)
        format: Image format (PNG, JPEG, etc.)
    
    Returns:
        True if successful
    """
    try:
        filepath = Path(filepath)
        filepath.parent.mkdir(parents=True, exist_ok=True)
        
        # Normalize to 0-255
        if data.dtype == np.float32 or data.dtype == np.float64:
            data = (np.clip(data, 0, 1) * 255).astype(np.uint8)
        
        # Convert to PIL Image
        if len(data.shape) == 2:
            img = Image.fromarray(data, mode='L')
        elif data.shape[2] == 3:
            img = Image.fromarray(data, mode='RGB')
        elif data.shape[2] == 4:
            img = Image.fromarray(data, mode='RGBA')
        else:
            logger.error(f"Unsupported texture shape: {data.shape}")
            return False
        
        img.save(filepath, format=format)
        logger.info(f"Saved texture: {filepath}")
        return True
    
    except Exception as e:
        logger.error(f"Failed to save texture {filepath}: {e}")
        logger.exception(e)
        return False


def load_texture(filepath: Path) -> Optional[np.ndarray]:
    """
    Load texture from file
    
    Returns:
        Texture data as numpy array or None
    """
    try:
        img = Image.open(filepath)
        data = np.array(img, dtype=np.float32) / 255.0
        logger.info(f"Loaded texture: {filepath} {data.shape}")
        return data
    
    except Exception as e:
        logger.error(f"Failed to load texture {filepath}: {e}")
        logger.exception(e)
        return None
