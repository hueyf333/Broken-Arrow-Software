"""Modifier system for non-destructive hair curve editing"""

import numpy as np
from typing import List, Optional, Dict, Any
from abc import ABC, abstractmethod
from .curves import Curve
from ..core.utils import normalize_vector, lerp


class Modifier(ABC):
    """Base class for all hair modifiers"""
    
    def __init__(self, name: str):
        self.name = name
        self.enabled = True
        self.bypassed = False
    
    @abstractmethod
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        """
        Apply modifier to curves
        
        Args:
            curves: List of curves to modify (modified in-place)
            context: Additional context data (mesh, masks, etc.)
        """
        pass
    
    @abstractmethod
    def get_parameters(self) -> Dict[str, Any]:
        """Get modifier parameters for UI"""
        pass
    
    @abstractmethod
    def set_parameter(self, name: str, value: Any):
        """Set a modifier parameter"""
        pass
    
    def to_dict(self) -> Dict[str, Any]:
        """Serialize to dictionary"""
        return {
            "type": self.__class__.__name__,
            "name": self.name,
            "enabled": self.enabled,
            "bypassed": self.bypassed,
            "parameters": self.get_parameters()
        }


class LengthModifier(Modifier):
    """Modify hair length"""
    
    def __init__(self, name: str = "Length"):
        super().__init__(name)
        self.factor = 1.0  # Multiply length by this
        self.offset = 0.0  # Add this to length
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        for curve in curves:
            # Scale from root
            if len(curve.points) > 0:
                direction = curve.points - curve.root_position
                new_points = curve.root_position + direction * self.factor
                curve.points = new_points + \
                              curve.get_tangent_at(0.5) * self.offset
    
    def get_parameters(self) -> Dict[str, Any]:
        return {"factor": self.factor, "offset": self.offset}
    
    def set_parameter(self, name: str, value: Any):
        if name == "factor":
            self.factor = float(value)
        elif name == "offset":
            self.offset = float(value)


class WidthModifier(Modifier):
    """Modify hair width profile"""
    
    def __init__(self, name: str = "Width"):
        super().__init__(name)
        self.root_width = 1.0
        self.tip_width = 0.1
        self.profile_power = 1.0  # 1.0 = linear, >1 = taper faster
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        for curve in curves:
            curve.width_root = self.root_width
            curve.width_tip = self.tip_width
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "root_width": self.root_width,
            "tip_width": self.tip_width,
            "profile_power": self.profile_power
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "root_width":
            self.root_width = float(value)
        elif name == "tip_width":
            self.tip_width = float(value)
        elif name == "profile_power":
            self.profile_power = float(value)


class ClumpModifier(Modifier):
    """Clump strands together"""
    
    def __init__(self, name: str = "Clump"):
        super().__init__(name)
        self.strength = 0.5
        self.radius = 5.0
        self.randomness = 0.1
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        # Group curves by clump_id
        clumps: Dict[int, List[Curve]] = {}
        for curve in curves:
            cid = curve.clump_id
            if cid not in clumps:
                clumps[cid] = []
            clumps[cid].append(curve)
        
        # For each clump, pull curves toward center
        for clump_curves in clumps.values():
            if len(clump_curves) < 2:
                continue
            
            # Find center curve (average)
            num_points = len(clump_curves[0].points)
            center_points = np.zeros((num_points, 3), dtype=np.float32)
            
            for curve in clump_curves:
                if len(curve.points) == num_points:
                    center_points += curve.points
            
            center_points /= len(clump_curves)
            
            # Pull each curve toward center
            for curve in clump_curves:
                if len(curve.points) != num_points:
                    continue
                
                for i in range(len(curve.points)):
                    t = i / max(1, len(curve.points) - 1)
                    # More clumping toward tip
                    local_strength = self.strength * t
                    
                    direction = center_points[i] - curve.points[i]
                    curve.points[i] += direction * local_strength
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "strength": self.strength,
            "radius": self.radius,
            "randomness": self.randomness
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "strength":
            self.strength = float(value)
        elif name == "radius":
            self.radius = float(value)
        elif name == "randomness":
            self.randomness = float(value)


class NoiseModifier(Modifier):
    """Add noise/frizz to hair"""
    
    def __init__(self, name: str = "Noise"):
        super().__init__(name)
        self.amplitude = 0.1
        self.frequency = 5.0
        self.seed = 42
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        for curve in curves:
            curve.apply_noise(self.amplitude, self.frequency, self.seed)
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "amplitude": self.amplitude,
            "frequency": self.frequency,
            "seed": self.seed
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "amplitude":
            self.amplitude = float(value)
        elif name == "frequency":
            self.frequency = float(value)
        elif name == "seed":
            self.seed = int(value)


class CurlModifier(Modifier):
    """Add curl to hair"""
    
    def __init__(self, name: str = "Curl"):
        super().__init__(name)
        self.radius = 2.0
        self.tightness = 1.0
        self.variation = 0.0
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        for curve in curves:
            if len(curve.points) < 2:
                continue
            
            # Create a spiral around the original curve
            original_points = curve.points.copy()
            
            for i in range(1, len(curve.points)):
                t = i / max(1, len(curve.points) - 1)
                
                # Get tangent from original curve
                if i == 0:
                    tangent = original_points[1] - original_points[0]
                elif i == len(curve.points) - 1:
                    tangent = original_points[i] - original_points[i - 1]
                else:
                    tangent = original_points[i + 1] - original_points[i - 1]
                
                tangent = normalize_vector(tangent)
                
                # Create perpendicular vectors
                if abs(tangent[2]) < 0.9:
                    perp1 = normalize_vector(np.cross(tangent, [0, 0, 1]))
                else:
                    perp1 = normalize_vector(np.cross(tangent, [1, 0, 0]))
                perp2 = normalize_vector(np.cross(tangent, perp1))
                
                # Spiral offset
                angle = t * self.tightness * 2 * np.pi * 4
                offset = (np.cos(angle) * perp1 + np.sin(angle) * perp2) * self.radius * t
                
                curve.points[i] = original_points[i] + offset
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "radius": self.radius,
            "tightness": self.tightness,
            "variation": self.variation
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "radius":
            self.radius = float(value)
        elif name == "tightness":
            self.tightness = float(value)
        elif name == "variation":
            self.variation = float(value)


class BendModifier(Modifier):
    """Bend/gravity modifier"""
    
    def __init__(self, name: str = "Bend"):
        super().__init__(name)
        self.strength = 1.0
        self.direction = np.array([0.0, 0.0, -1.0])
        self.damping = 0.0
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        for curve in curves:
            curve.apply_gravity(self.strength, self.direction)
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "strength": self.strength,
            "direction": self.direction.tolist(),
            "damping": self.damping
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "strength":
            self.strength = float(value)
        elif name == "direction":
            self.direction = np.array(value, dtype=np.float32)
        elif name == "damping":
            self.damping = float(value)


class SmoothModifier(Modifier):
    """Smooth/relax curves"""
    
    def __init__(self, name: str = "Smooth"):
        super().__init__(name)
        self.iterations = 1
        self.strength = 0.5
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        for curve in curves:
            curve.smooth(self.iterations, self.strength)
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "iterations": self.iterations,
            "strength": self.strength
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "iterations":
            self.iterations = int(value)
        elif name == "strength":
            self.strength = float(value)


class MirrorModifier(Modifier):
    """Mirror curves across axis"""
    
    def __init__(self, name: str = "Mirror"):
        super().__init__(name)
        self.axis = 'X'  # X, Y, or Z
        self.merge_distance = 0.01
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        # Note: This modifier would typically generate mirrored curves
        # For simplicity, we just mark it as applied
        if self.bypassed or not self.enabled:
            return
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "axis": self.axis,
            "merge_distance": self.merge_distance
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "axis":
            self.axis = str(value)
        elif name == "merge_distance":
            self.merge_distance = float(value)


class PartingModifier(Modifier):
    """Create parting line in hair"""
    
    def __init__(self, name: str = "Parting"):
        super().__init__(name)
        self.position = 0.0  # -1 to 1
        self.width = 0.1
        self.falloff = 1.0
    
    def apply(self, curves: List[Curve], context: Dict[str, Any]):
        if self.bypassed or not self.enabled:
            return
        
        # Apply parting effect based on curve root position
        for curve in curves:
            # Simple parting based on X position
            dist_from_part = abs(curve.root_position[0] - self.position)
            
            if dist_from_part < self.width:
                # Bend away from parting line
                factor = (self.width - dist_from_part) / self.width
                factor = pow(factor, self.falloff)
                
                direction = np.sign(curve.root_position[0] - self.position)
                offset = np.array([direction * factor * 2.0, 0, 0])
                
                for i in range(len(curve.points)):
                    t = i / max(1, len(curve.points) - 1)
                    curve.points[i] += offset * t
    
    def get_parameters(self) -> Dict[str, Any]:
        return {
            "position": self.position,
            "width": self.width,
            "falloff": self.falloff
        }
    
    def set_parameter(self, name: str, value: Any):
        if name == "position":
            self.position = float(value)
        elif name == "width":
            self.width = float(value)
        elif name == "falloff":
            self.falloff = float(value)


class ModifierStack:
    """Manages a stack of modifiers"""
    
    def __init__(self):
        self.modifiers: List[Modifier] = []
    
    def add_modifier(self, modifier: Modifier):
        """Add modifier to stack"""
        self.modifiers.append(modifier)
    
    def remove_modifier(self, index: int):
        """Remove modifier at index"""
        if 0 <= index < len(self.modifiers):
            self.modifiers.pop(index)
    
    def move_modifier(self, from_index: int, to_index: int):
        """Reorder modifiers"""
        if 0 <= from_index < len(self.modifiers) and 0 <= to_index < len(self.modifiers):
            modifier = self.modifiers.pop(from_index)
            self.modifiers.insert(to_index, modifier)
    
    def apply_all(self, curves: List[Curve], context: Dict[str, Any]):
        """Apply all modifiers in order"""
        for modifier in self.modifiers:
            if modifier.enabled and not modifier.bypassed:
                modifier.apply(curves, context)
    
    def to_dict(self) -> Dict[str, Any]:
        """Serialize stack"""
        return {
            "modifiers": [m.to_dict() for m in self.modifiers]
        }
    
    @staticmethod
    def from_dict(data: Dict[str, Any]) -> 'ModifierStack':
        """Deserialize stack"""
        stack = ModifierStack()
        
        modifier_classes = {
            "LengthModifier": LengthModifier,
            "WidthModifier": WidthModifier,
            "ClumpModifier": ClumpModifier,
            "NoiseModifier": NoiseModifier,
            "CurlModifier": CurlModifier,
            "BendModifier": BendModifier,
            "SmoothModifier": SmoothModifier,
            "MirrorModifier": MirrorModifier,
            "PartingModifier": PartingModifier,
        }
        
        for mod_data in data.get("modifiers", []):
            mod_type = mod_data.get("type")
            if mod_type in modifier_classes:
                mod = modifier_classes[mod_type](mod_data.get("name", mod_type))
                mod.enabled = mod_data.get("enabled", True)
                mod.bypassed = mod_data.get("bypassed", False)
                
                for param_name, param_value in mod_data.get("parameters", {}).items():
                    mod.set_parameter(param_name, param_value)
                
                stack.add_modifier(mod)
        
        return stack
