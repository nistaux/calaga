#pragma once

#include <SDL.h>

typedef enum {
    PLAYER_STATE_TITLE,
    PLAYER_STATE_IDLE,
    PLAYER_STATE_SHOOTING,
    PLAYER_STATE_RECHARGING,
    PLAYER_STATE_DEAD
} PlayerState;

typedef struct {
    float x;
    float y;
    int hp;
    float x_vel;
    float y_vel;
    float shoot_reload_interval_seconds;
    float last_shot;
    bool reloading;
    PlayerState state;
} Player;

void init_player(float x, float y);
void reset_player();
void add_player_x_vel(float vel);
void load_player(SDL_Renderer *renderer);
void draw_player(SDL_Renderer *renderer);
void tick_player(float deltaTime);
void set_toggle_shoot_player(bool set);