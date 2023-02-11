#include <windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

bool running = true;
void* buffer_memory;
int buffer_width;
int buffer_height;
BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void OnSize(HWND hwnd, UINT flag, int width, int height);

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
    HWND window = CreateWindow(
        CLASS_NAME,
        L"P0NG",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (window == NULL)
    {
        return 0;
    }

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
        unsigned int* pixel = (unsigned int*)buffer_memory;
        unsigned int color = 0xeb5905;

        for (int y = 0; y < buffer_height; y++) {
            for (int x = 0; x < buffer_width; x++) {
                *pixel++ = sin(x*y)*(x*y);
            }
        }

        // Render
        StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
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
            buffer_width = rect.right - rect.left;
            buffer_height = rect.bottom - rect.top;

            int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

            if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);
            buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
            buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
            buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
            buffer_bitmap_info.bmiHeader.biPlanes = 1;
            buffer_bitmap_info.bmiHeader.biBitCount = 32;
            buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
        }
        break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return result;
}

void OnSize(HWND hwnd, UINT flag, int width, int height)
{
    // Handle resizing
}