#include <windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "utils.cpp"

global_variable bool running = true;

struct Render_State {
    int height, width;
    void* memory;

    BITMAPINFO bitmapinfo;
};

global_variable Render_State render_state;

#include "renderer.cpp"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Game Window";

    WNDCLASS window_class = {};

    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = CLASS_NAME;
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = hInstance;

    RegisterClass(&window_class);

    // Create the window.
    HWND window = CreateWindow(CLASS_NAME, L"PONG++", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);

    if (window == NULL) { return 0; }

    HDC hdc = GetDC(window);

    ShowWindow(window, nCmdShow);

    while (running) {
        // Input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // Simulate
        clear_screen(0xff5500);
        draw_rect(0, 0, .2, .2, 0x00ff00);

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        }
        break;
        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;

            int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);

            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
            render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
            render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
            render_state.bitmapinfo.bmiHeader.biPlanes = 1;
            render_state.bitmapinfo.bmiHeader.biBitCount = 32;
            render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;
        }
        break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return result;
}