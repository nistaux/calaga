#pragma once

#include <SDL.h>

typedef enum {
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
Player *get_player();
SDL_Texture *get_player_texture();
SDL_Rect *get_player_rect();
void load_player(SDL_Renderer *renderer);
void set_player_state(PlayerState state);
void draw_player(SDL_Renderer *renderer);
void move_player();