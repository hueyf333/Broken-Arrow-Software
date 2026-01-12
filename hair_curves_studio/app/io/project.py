"""Project file management with save/load functionality"""

import json
import gzip
from pathlib import Path
from typing import Optional, Dict, Any, List
from datetime import datetime
from ..core.logger import logger
from ..core.utils import safe_json_dump


class Project:
    """Manages project data and file operations"""
    
    VERSION = "1.0.0"
    
    def __init__(self):
        self.filepath: Optional[Path] = None
        self.modified = False
        self.created_time = datetime.now()
        self.modified_time = datetime.now()
        
        # Project data
        self.scalp_meshes: List[Dict] = []
        self.hair_descriptions: List[Dict] = []
        self.settings: Dict[str, Any] = {
            "autosave_enabled": True,
            "autosave_interval": 300,  # seconds
            "viewport_settings": {},
        }
    
    def save(self, filepath: Optional[Path] = None) -> bool:
        """Save project to file"""
        if filepath:
            self.filepath = Path(filepath)
        
        if not self.filepath:
            logger.error("No filepath specified for save")
            return False
        
        try:
            self.modified_time = datetime.now()
            
            data = {
                "version": self.VERSION,
                "created_time": self.created_time.isoformat(),
                "modified_time": self.modified_time.isoformat(),
                "scalp_meshes": self.scalp_meshes,
                "hair_descriptions": self.hair_descriptions,
                "settings": self.settings,
            }
            
            # Save as compressed JSON
            json_str = safe_json_dump(data)
            
            self.filepath.parent.mkdir(parents=True, exist_ok=True)
            
            with gzip.open(self.filepath, 'wt', encoding='utf-8') as f:
                f.write(json_str)
            
            self.modified = False
            logger.info(f"Project saved: {self.filepath}")
            return True
        
        except Exception as e:
            logger.error(f"Failed to save project: {e}")
            logger.exception(e)
            return False
    
    def load(self, filepath: Path) -> bool:
        """Load project from file"""
        try:
            filepath = Path(filepath)
            
            if not filepath.exists():
                logger.error(f"Project file not found: {filepath}")
                return False
            
            with gzip.open(filepath, 'rt', encoding='utf-8') as f:
                data = json.load(f)
            
            # Validate version
            version = data.get("version", "0.0.0")
            if version != self.VERSION:
                logger.warning(f"Project version mismatch: {version} vs {self.VERSION}")
            
            # Load data
            self.created_time = datetime.fromisoformat(data.get("created_time", datetime.now().isoformat()))
            self.modified_time = datetime.fromisoformat(data.get("modified_time", datetime.now().isoformat()))
            self.scalp_meshes = data.get("scalp_meshes", [])
            self.hair_descriptions = data.get("hair_descriptions", [])
            self.settings = data.get("settings", {})
            
            self.filepath = filepath
            self.modified = False
            
            logger.info(f"Project loaded: {filepath}")
            return True
        
        except Exception as e:
            logger.error(f"Failed to load project: {e}")
            logger.exception(e)
            return False
    
    def create_autosave(self) -> bool:
        """Create autosave backup"""
        if not self.filepath:
            return False
        
        try:
            autosave_path = self.filepath.parent / f"{self.filepath.stem}.autosave{self.filepath.suffix}"
            
            data = {
                "version": self.VERSION,
                "created_time": self.created_time.isoformat(),
                "modified_time": datetime.now().isoformat(),
                "scalp_meshes": self.scalp_meshes,
                "hair_descriptions": self.hair_descriptions,
                "settings": self.settings,
            }
            
            json_str = safe_json_dump(data)
            
            with gzip.open(autosave_path, 'wt', encoding='utf-8') as f:
                f.write(json_str)
            
            logger.debug(f"Autosave created: {autosave_path}")
            return True
        
        except Exception as e:
            logger.error(f"Autosave failed: {e}")
            return False
    
    def recover_from_autosave(self) -> bool:
        """Recover from autosave file"""
        if not self.filepath:
            return False
        
        autosave_path = self.filepath.parent / f"{self.filepath.stem}.autosave{self.filepath.suffix}"
        
        if autosave_path.exists():
            return self.load(autosave_path)
        
        return False
    
    def mark_modified(self):
        """Mark project as modified"""
        self.modified = True
        self.modified_time = datetime.now()
    
    def is_modified(self) -> bool:
        """Check if project has unsaved changes"""
        return self.modified
