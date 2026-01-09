#include "AAX_GUI.h"
#include "VST3Host.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Patchwrap {

AAX_GUI::AAX_GUI()
    : hwnd(nullptr)
    , vst3Host(nullptr)
    , dpiScale(1.0f)
    , currentWidth(800)
    , currentHeight(600)
    , vst3EditorHandle(nullptr)
{
}

AAX_GUI::~AAX_GUI()
{
    DestroyGUI();
}

void AAX_GUI::CreateGUI(void* parentWindow)
{
    hwnd = parentWindow;
    
    if (!hwnd) return;
    
    // In a real implementation:
    // 1. Create Windows controls (combo box for plugin selection, buttons, sliders)
    // 2. Create meter displays
    // 3. If VST3 plugin has editor, embed it in a child window
    // 4. Set up message handling for control events
    // 5. Apply DPI scaling
    
    CreateControls();
}

void AAX_GUI::DestroyGUI()
{
    if (vst3EditorHandle && vst3Host) {
        vst3Host->closeEditor();
        vst3EditorHandle = nullptr;
    }
    
    // Destroy Windows controls
    hwnd = nullptr;
}

void AAX_GUI::UpdateParameter(uint32_t paramID)
{
    // Update GUI controls to reflect parameter changes
    // This is called when parameters change from automation or preset recall
}

void AAX_GUI::SetVST3Host(VST3Host* host)
{
    vst3Host = host;
    
    // If plugin is loaded and has editor, create it
    if (vst3Host && vst3Host->hasEditor() && hwnd) {
        vst3EditorHandle = vst3Host->createEditor(hwnd);
        
        // Get and apply editor size
        uint32_t width, height;
        vst3Host->getEditorSize(width, height);
        currentWidth = width;
        currentHeight = height;
    }
}

void AAX_GUI::GetSize(uint32_t& width, uint32_t& height)
{
    width = currentWidth;
    height = currentHeight;
    
    // If VST3 editor is embedded, get its size
    if (vst3EditorHandle && vst3Host) {
        vst3Host->getEditorSize(width, height);
        currentWidth = width;
        currentHeight = height;
    }
}

void AAX_GUI::SetSize(uint32_t width, uint32_t height)
{
    currentWidth = width;
    currentHeight = height;
    
    // Resize VST3 editor if present
    if (vst3EditorHandle && vst3Host) {
        vst3Host->setEditorSize(width, height);
    }
    
    // Resize wrapper controls
#ifdef _WIN32
    if (hwnd) {
        HWND hWnd = static_cast<HWND>(hwnd);
        SetWindowPos(hWnd, NULL, 0, 0, width, height, 
                    SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
#endif
}

void AAX_GUI::SetDPIScale(float scale)
{
    dpiScale = scale;
    
    // Recalculate control sizes based on DPI
    // In a real implementation, all control coordinates would be scaled
}

void AAX_GUI::CreateControls()
{
    // Create wrapper GUI controls:
    // 1. Plugin selector dropdown (scan and list VST3 plugins)
    // 2. Load/Unload buttons
    // 3. Bypass checkbox
    // 4. Output gain slider with label
    // 5. Input/output meters
    // 6. Container for VST3 editor or generated parameter controls
    
#ifdef _WIN32
    if (!hwnd) return;
    
    HWND hParent = static_cast<HWND>(hwnd);
    
    // Example: Create a label (in real implementation, would create all controls)
    // HWND hLabel = CreateWindowEx(0, L"STATIC", L"Patchwrap",
    //     WS_CHILD | WS_VISIBLE,
    //     10, 10, 200, 30,
    //     hParent, NULL, GetModuleHandle(NULL), NULL);
#endif
}

void AAX_GUI::UpdateMeters(float leftLevel, float rightLevel)
{
    // Update meter display with current audio levels
    // In a real implementation, this would be called periodically
    // to update visual meters showing input/output levels
}

void AAX_GUI::OnPluginSelect()
{
    // Handle plugin selection from dropdown
    // Update available parameters in generated controls view
}

void AAX_GUI::OnLoadPlugin()
{
    // Handle load button click
    // Load selected VST3 plugin
    // Create embedded editor if plugin has one
}

void AAX_GUI::OnUnloadPlugin()
{
    // Handle unload button click
    // Close VST3 editor
    // Unload plugin
    // Reset to pass-through state
}

} // namespace Patchwrap
