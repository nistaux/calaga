#pragma once

#include <SDL.h>

typedef enum {
    TITLE,
    IDLE,
    SHOOTING,
    RECHARGING,
    DEAD
} PlayerState;

typedef struct {
    float x;
    float y;
    int hp;
    float x_vel;
    float y_vel;
    PlayerState state;
} Player;

void init_player(float x, float y);
void set_player_default_location();
void add_player_x_vel(float vel);
void load_player(SDL_Renderer *renderer);
void draw_player(SDL_Renderer *renderer);
void move_player();