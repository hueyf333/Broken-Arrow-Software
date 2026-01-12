#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <string>

namespace BrokenArrow {
namespace App {

class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    bool Create(HINSTANCE hInstance, int nCmdShow);
    HWND GetHandle() const { return hwnd_; }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    void RegisterWindowClass(HINSTANCE hInstance);
    void CreateMenu();
    void CreateToolbar();
    void CreateStatusBar();

    HWND hwnd_;
    HWND viewportPanel_;
    HWND toolbar_;
    HWND statusBar_;
    HINSTANCE hInstance_;
};

} // namespace App
} // namespace BrokenArrow

#endif // _WIN32
