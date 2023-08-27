#pragma once

#include <SDL.h>

#define possible_projectiles 30
#define PROJECTILE_MAP_DIR "./assets/images/projectiles_map.png"

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
    double angle;
    ProjectileType type;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
} Projectile;

void init_projectile_textures(SDL_Renderer *renderer);
Projectile *get_projectiles();
void create_projectile(Projectile temp);
void destroy_projectile(int index);
void move_projectiles();
void draw_projectiles(SDL_Renderer *renderer);