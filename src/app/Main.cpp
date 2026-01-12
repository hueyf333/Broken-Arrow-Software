#ifdef _WIN32
#include <Windows.h>
#include "Application.h"
#include "MainWindow.h"

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
) {
    using namespace BrokenArrow::App;

    if (!Application::Instance().Initialize()) {
        MessageBoxW(nullptr, L"Failed to initialize application", L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    MainWindow mainWindow;
    if (!mainWindow.Create(hInstance, nCmdShow)) {
        MessageBoxW(nullptr, L"Failed to create main window", L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Application::Instance().Shutdown();

    return (int)msg.wParam;
}
#else
int main() {
    return 0;
}
#endif
