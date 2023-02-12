#pragma once

#include "utils.h"
#include "platform_common.h"
#include "game.h"

#include <windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmapinfo;
};

extern Render_State render_state;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
