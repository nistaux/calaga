#pragma once

#include <SDL.h>

#define possible_projectiles 10

typedef enum {
    PROJ_PLAYER,
    PROJ_TSHOT,
    PROJ_BEADER
} ProjectileType;

typedef struct {
    bool created;
    float x;
    float y;
    float x_vel;
    float y_vel;
    float speed;
    ProjectileType type;
    SDL_Rect rect;
    SDL_Texture *texture;
} Projectile;

void create_projectile(SDL_Renderer *renderer, Projectile temp);
void destroy_projectile(int index);
void move_projectiles();
void draw_projectiles(SDL_Renderer *renderer);