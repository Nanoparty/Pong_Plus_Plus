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
#include "platform_common.cpp"
#include "game.cpp"

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

    Input input = {};

    ShowWindow(window, nCmdShow);

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

    while (running) {
        // Input
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk: {\
input.buttons[b].is_down = is_down;\
input.buttons[b].changed = true;\
} break;

                    switch (vk_code) {
                        process_button(BUTTON_UP, VK_UP);
                        process_button(BUTTON_DOWN, VK_DOWN);
                        process_button(BUTTON_LEFT, VK_LEFT);
                        process_button(BUTTON_RIGHT, VK_RIGHT);
                    }
                }
                break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }

        // Simulate
        simulate_game(&input, delta_time);
       
        

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
    
        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
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