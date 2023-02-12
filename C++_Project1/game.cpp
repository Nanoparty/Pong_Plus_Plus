#include "game.h"

GameObject player1 = { 80, 0, 0, 0, 0, 0, 2.5f, 12.f, 0x00ffff };
GameObject player2 = { -80, 0, 0, 0, 0, 0, 2.5f, 12.f, 0xff4564 };
GameObject ball = { 0, 0, 120, 0, 0, 0, 1.f, 1.f, 0x00ff00 };

float arena_half_size_x = 88, arena_half_size_y = 45;
float speed = 50.f;
float bounce = -0.4f;
int border_color = 0xd6d6d6;
int background_color = 0x000000;
int player1_score, player2_score;

void simulate_player(GameObject& player, float dt) {
    // Friction
    player.acc_y -= player.vel_y * 10.f;

    // Calculate Player Movement
    player.pos_y = player.pos_y + player.vel_y * dt + player.acc_y * dt * dt * .5f;
    player.vel_y = player.vel_y + player.acc_y * dt;

    // Check Arena Collision Top
    if (player.pos_y + player.half_size_y > arena_half_size_y) {
        player.pos_y = arena_half_size_y - player.half_size_y;
        player.vel_y *= bounce;
    }

    // Check Arena Collision Bottom
    if (player.pos_y - player.half_size_y < -arena_half_size_y) {
        player.pos_y = -arena_half_size_y + player.half_size_y;
        player.vel_y *= bounce;
    }
}

bool aabb_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
    return (
        p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y - hs1y < p2y + hs2y);
}

void reset_ball() {
    ball.vel_x *= -1;
    ball.vel_y = 0;
    ball.pos_x = ball.pos_y = 0;
}

void simulate_game(Input* input, float dt) {

    // Draw Background

    clear_screen(border_color);
    draw_rect(0, 0, 88, 45, background_color);

    player1.acc_y = 0.f;
    player2.acc_y = 0.f;

    // Player Input
    if (is_down(BUTTON_UP)) player1.acc_y += 2000;
    if (is_down(BUTTON_DOWN)) player1.acc_y -= 2000;

#if 0
    if (is_down(BUTTON_W)) player2.acc_y += 2000;
    if (is_down(BUTTON_S)) player2.acc_y -= 2000;
#else
    //if (ball.pos_y < player2.pos_y + player2.half_size_y / 2) player2.acc_y -= 1500;
    //if (ball.pos_y > player2.pos_y - player2.half_size_y / 2) player2.acc_y += 1500;
    player2.acc_y = (ball.pos_y - player2.pos_y) * 100;
    player2.acc_y = clamp(-1300, player2.acc_y, 1300);
#endif

    simulate_player(player1, dt);
    simulate_player(player2, dt);

    // Simulate Ball
    {
        // Ball Movement
        ball.pos_x += ball.vel_x * dt;
        ball.pos_y += ball.vel_y * dt;

        // Ball Player Collision
        if (aabb_aabb(ball.pos_x, ball.pos_y, ball.half_size_x, ball.half_size_y, player1.pos_x, player1.pos_y, player1.half_size_x, player1.half_size_y))
        {
            ball.pos_x = player1.pos_x - player1.half_size_x - ball.half_size_x;
            ball.vel_x *= -1;
            ball.vel_y = player1.vel_y * 0.75f + abs(ball.pos_y - player1.pos_y);
        }
        else if (aabb_aabb(ball.pos_x, ball.pos_y, ball.half_size_x, ball.half_size_y, player2.pos_x, player2.pos_y, player2.half_size_x, player2.half_size_y))
        {
            ball.pos_x = player2.pos_x + player2.half_size_x + ball.half_size_x;
            ball.vel_x *= -1;
            ball.vel_y = player2.vel_y * 0.75f + abs(ball.pos_y - player2.pos_y);
        }

        // Ball Horizontal Collision
        if (ball.pos_y + ball.half_size_y > arena_half_size_y) {
            ball.pos_y = arena_half_size_y - ball.half_size_y;
            ball.vel_y *= -1;
        }
        if (ball.pos_y - ball.half_size_y < -arena_half_size_y) {
            ball.pos_y = -arena_half_size_y + ball.half_size_y;
            ball.vel_y *= -1;
        }

        // Ball Vertical Collision
        if (ball.pos_x + ball.half_size_x > arena_half_size_x) {
            reset_ball();
            player2_score += 1;
        }
        else if (ball.pos_x - ball.half_size_x < -arena_half_size_x) {
            reset_ball();
            player1_score += 1;
        }
    }

    // Rendering
    draw_rect(player1.pos_x, player1.pos_y, player1.half_size_x, player1.half_size_y, player1.color);
    draw_rect(player2.pos_x, player2.pos_y, player2.half_size_x, player2.half_size_y, player2.color);

    draw_rect(ball.pos_x, ball.pos_y, ball.half_size_x, ball.half_size_y, ball.color);
}