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

GameObject player1 = {};
GameObject player2 = {};
GameObject ball = {};

float player1_pos, player1_vel;
float player2_pos, player2_vel;

float ball_pos_x, ball_pos_y, ball_vel_x = 100, ball_vel_y, ball_acc_x, ball_acc_y;

float player_half_size_x = 2.5f, player_half_size_y = 12.f;
float arena_half_size_x = 88, arena_half_size_y = 45;
float ball_half_size = 1.f;

float speed = 50.f;

u32 player1_color = 0x00ffff;
u32 player2_color = 0xff4564;
u32 ball_color = 0x00ff00;
u32 border_color = 0xf7f7f7;
u32 background_color = 0x000000;

int player1_score, player2_score;

internal void simulate_player(float* pos, float* vel, float* acc, float dt) {
    // Friction
    *acc -= *vel * 10.f;

    // Calculate Player Movement
    *pos = *pos + *vel * dt + *acc * dt * dt * .5f;
    *vel = *vel + *acc * dt;

    // Check Arena Collision Top
    if (*pos + player_half_size_y > arena_half_size_y) {
        *pos = arena_half_size_y - player_half_size_y;
        *vel *= -.6f;
    }

    // Check Arena Collision Bottom
    if (*pos - player_half_size_y < -arena_half_size_y) {
        *pos = -arena_half_size_y + player_half_size_y;
        *vel *= -.6f;
    }
}

internal bool aabb_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
    return (
        p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y - hs1y < p2y + hs2y);
}

internal void simulate_game(Input* input, float dt) {

    // Draw Background
    clear_screen(border_color);
    draw_rect(0, 0, 88, 45, background_color);

    float player1_acc = 0.f;
    float player2_acc = 0.f;

    // Player Input
    if (is_down(BUTTON_UP)) player1_acc += 2000;
    if (is_down(BUTTON_DOWN)) player1_acc -= 2000;

    if (is_down(BUTTON_W)) player2_acc += 2000;
    if (is_down(BUTTON_S)) player2_acc -= 2000;

    simulate_player(&player1_pos, &player1_vel, &player1_acc, dt);
    simulate_player(&player2_pos, &player2_vel, &player2_acc, dt);

    // Simulate Ball
    {
        // Ball Movement
        ball_pos_x += ball_vel_x * dt;
        ball_pos_y += ball_vel_y * dt;

        // Ball Player Collision
        if (aabb_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 80, player1_pos, player_half_size_x, player_half_size_y))
        {
            ball_pos_x = 80 - player_half_size_x - ball_half_size;
            ball_vel_x *= -1;
            ball_vel_y = player1_vel * 0.75f;
        }
        else if (ball_pos_x + ball_half_size > -80 - player_half_size_x &&
            ball_pos_x - ball_half_size < -80 + player_half_size_x &&
            ball_pos_y + ball_half_size > player2_pos - player_half_size_y &&
            ball_pos_y - ball_half_size < player2_pos + player_half_size_y)
        {
            ball_pos_x = -80 + player_half_size_x + ball_half_size;
            ball_vel_x *= -1;
            ball_vel_y = player2_vel * 0.75f;
        }

        // Ball Horizontal Collision
        if (ball_pos_y + ball_half_size > arena_half_size_y) {
            ball_pos_y = arena_half_size_y - ball_half_size;
            ball_vel_y *= -1;
        }
        if (ball_pos_y - ball_half_size < -arena_half_size_y) {
            ball_pos_y = -arena_half_size_y + ball_half_size;
            ball_vel_y *= -1;
        }

        // Ball Vertical Collision
        if (ball_pos_x + ball_half_size > arena_half_size_x) {
            ball_vel_x *= -1;
            ball_vel_y = 0;
            ball_pos_x = ball_pos_y = 0;
            player2_score += 1;
        }
        else if (ball_pos_x - ball_half_size < -arena_half_size_x) {
            ball_vel_x *= -1;
            ball_vel_y = 0;
            ball_pos_x = ball_pos_y = 0;
            player1_score += 1;
        }
    }

    // Draw Players
    draw_rect(80, player1_pos, player_half_size_x, player_half_size_y, player1_color);
    draw_rect(-80, player2_pos, player_half_size_x, player_half_size_y, player2_color);

    // Draw Ball
    draw_rect(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, ball_color);
    
}