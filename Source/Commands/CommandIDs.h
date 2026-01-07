#pragma once

namespace KnobSmith
{
namespace CommandIDs
{
    // File menu
    static constexpr int fileNew = 0x2001;
    static constexpr int fileOpen = 0x2002;
    static constexpr int fileSave = 0x2003;
    static constexpr int fileSaveAs = 0x2004;
    static constexpr int fileImportImage = 0x2005;
    static constexpr int fileExportPNG = 0x2006;
    static constexpr int fileExportAs = 0x2007;
    static constexpr int fileExit = 0x2008;

    // Edit menu
    static constexpr int editUndo = 0x2010;
    static constexpr int editRedo = 0x2011;
    static constexpr int editCut = 0x2012;
    static constexpr int editCopy = 0x2013;
    static constexpr int editPaste = 0x2014;
    static constexpr int editDelete = 0x2015;
    static constexpr int editPreferences = 0x2016;

    // View menu
    static constexpr int viewZoomIn = 0x2020;
    static constexpr int viewZoomOut = 0x2021;
    static constexpr int viewFitToScreen = 0x2022;
    static constexpr int viewGrid = 0x2023;
    static constexpr int viewSnap = 0x2024;
    static constexpr int viewTogglePanels = 0x2025;

    // Tools menu
    static constexpr int toolSelect = 0x2030;
    static constexpr int toolBrush = 0x2031;
    static constexpr int toolEraser = 0x2032;
    static constexpr int toolFill = 0x2033;
    static constexpr int toolShapes = 0x2034;
    static constexpr int toolColorPicker = 0x2035;

    // Filmstrip menu
    static constexpr int filmstripSettings = 0x2040;
    static constexpr int filmstripGenerate = 0x2041;
    static constexpr int filmstripPreview = 0x2042;

    // Help menu
    static constexpr int helpAbout = 0x2050;
    static constexpr int helpShortcuts = 0x2051;
}
} // namespace KnobSmith
