#pragma once

#include "utils.h"
#include "platform_common.h"
#include "main.h"

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

void clear_screen(u32 color);

void draw_rect_in_pixels(int x0, int y0, int x1, int y1, int color);

void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color);