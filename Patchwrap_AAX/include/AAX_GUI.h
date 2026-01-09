#pragma once

#include <cstdint>
#include <memory>

namespace Patchwrap {

class VST3Host;

// AAX GUI implementation
// This would inherit from AAX_CEffectGUI in a real implementation
class AAX_GUI {
public:
    AAX_GUI();
    virtual ~AAX_GUI();

    // Create GUI
    void CreateGUI(void* parentWindow);
    
    // Destroy GUI
    void DestroyGUI();

    // Update GUI with parameter changes
    void UpdateParameter(uint32_t paramID);

    // Set VST3 host for embedding VST3 editor
    void SetVST3Host(VST3Host* host);

    // Get GUI size
    void GetSize(uint32_t& width, uint32_t& height);
    
    // Set GUI size (for resizable)
    void SetSize(uint32_t width, uint32_t height);

    // DPI scaling
    void SetDPIScale(float scale);

private:
    void* hwnd;
    VST3Host* vst3Host;
    float dpiScale;
    uint32_t currentWidth;
    uint32_t currentHeight;
    
    void* vst3EditorHandle;
    
    void CreateControls();
    void UpdateMeters(float leftLevel, float rightLevel);
    void OnPluginSelect();
    void OnLoadPlugin();
    void OnUnloadPlugin();
};

} // namespace Patchwrap
