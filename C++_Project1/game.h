#pragma once

#include "utils.h"
#include "renderer.h"
#include "platform_common.h"

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) ((!input->buttons[b].is_down) && input->buttons[b].changed)

struct GameObject {
    float pos_x, pos_y;
    float vel_x, vel_y;
    float acc_x, acc_y;
    float half_size_x, half_size_y;
    int color;
};

void simulate_player(GameObject& player, float dt);

bool aabb_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y);

void reset_ball();

void simulate_game(Input* input, float dt);

