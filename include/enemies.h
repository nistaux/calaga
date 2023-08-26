#pragma once

#include <stdbool.h>

#include <SDL.h>

#define ENEMY_SPRITE_DIR "./assets/images/enemies.png"
#define POSSIBLE_ENEMIES 10

typedef enum {
    ENEMY_TYPE_BEADER,
    ENEMY_TYPE_DAGGER,
    ENEMY_TYPE_TSHOT,
    ENEMY_TYPE_ASTEROID
} EnemyType;

typedef enum {
    MOVE_TYPE_LEFTRIGHT,
    MOVE_TYPE_CIRCLE
} MoveType;

typedef enum {
    ENEMY_STATE_SHOOTING,
    ENEMY_STATE_RECHARGING,
    ENEMY_STATE_DEAD
} EnemyState;

typedef struct {
    EnemyType type;
    EnemyState state;
    MoveType moveType;
    SDL_Rect spriteRect;
    bool created;
    float x;
    float y;
    int hp;
    float x_vel;
    float y_vel;
    float shoot_reload_interval_seconds;
    float last_shot;
    bool reloading;
} Enemy;

void init_enemies(SDL_Renderer *renderer);
void create_enemy(EnemyType type, MoveType moveType, int startLocation);
void tick_enemies();
void draw_enemies(SDL_Renderer *renderer);