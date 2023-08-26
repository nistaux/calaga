#pragma once

#include <SDL.h>

#define possible_projectiles 15

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
} Projectile;

void init_projectile_textures(SDL_Renderer *renderer);
void create_projectile(Projectile temp);
void destroy_projectile(int index);
void move_projectiles();
void draw_projectiles(SDL_Renderer *renderer);