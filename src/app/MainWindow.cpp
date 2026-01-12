#include "MainWindow.h"
#include "Application.h"

#ifdef _WIN32
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

namespace BrokenArrow {
namespace App {

MainWindow::MainWindow() : hwnd_(nullptr), viewportPanel_(nullptr), 
                            toolbar_(nullptr), statusBar_(nullptr), hInstance_(nullptr) {
}

MainWindow::~MainWindow() {
}

bool MainWindow::Create(HINSTANCE hInstance, int nCmdShow) {
    hInstance_ = hInstance;

    RegisterWindowClass(hInstance);

    hwnd_ = CreateWindowExW(
        0,
        L"BrokenArrowHairStudio",
        L"Broken Arrow Hair Studio",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    if (!hwnd_) {
        return false;
    }

    CreateMenu();
    CreateToolbar();
    CreateStatusBar();

    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);

    return true;
}

void MainWindow::RegisterWindowClass(HINSTANCE hInstance) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"BrokenArrowHairStudio";

    RegisterClassExW(&wc);
}

void MainWindow::CreateMenu() {
    HMENU hMenuBar = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();

    AppendMenuW(hFileMenu, MF_STRING, 1001, L"&New Project\tCtrl+N");
    AppendMenuW(hFileMenu, MF_STRING, 1002, L"&Open Project\tCtrl+O");
    AppendMenuW(hFileMenu, MF_STRING, 1003, L"&Save Project\tCtrl+S");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hFileMenu, MF_STRING, 1004, L"&Import Mesh");
    AppendMenuW(hFileMenu, MF_STRING, 1005, L"&Export Mesh");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hFileMenu, MF_STRING, 1006, L"E&xit");

    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");

    SetMenu(hwnd_, hMenuBar);
}

void MainWindow::CreateToolbar() {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    toolbar_ = CreateWindowExW(
        0,
        TOOLBARCLASSNAMEW,
        nullptr,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0,
        hwnd_,
        nullptr,
        hInstance_,
        nullptr
    );

    if (toolbar_) {
        SendMessage(toolbar_, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    }
}

void MainWindow::CreateStatusBar() {
    statusBar_ = CreateWindowExW(
        0,
        STATUSCLASSNAMEW,
        nullptr,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0,
        hwnd_,
        nullptr,
        hInstance_,
        nullptr
    );
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (MainWindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    } else {
        pThis = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE: {
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);
        
        if (pThis && pThis->toolbar_) {
            SendMessage(pThis->toolbar_, TB_AUTOSIZE, 0, 0);
        }
        
        if (pThis && pThis->statusBar_) {
            SendMessage(pThis->statusBar_, WM_SIZE, 0, 0);
        }
        return 0;
    }

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case 1001: // New Project
            Application::Instance().NewProject();
            break;
        case 1002: // Open Project
            break;
        case 1003: // Save Project
            break;
        case 1004: // Import Mesh
            break;
        case 1005: // Export Mesh
            break;
        case 1006: // Exit
            DestroyWindow(hwnd);
            break;
        }
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

} // namespace App
} // namespace BrokenArrow

#endif // _WIN32
