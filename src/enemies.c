#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "enemies.h"

SDL_Texture *enemiesTexture;
int total_enemies = 0;
Enemy enemies[POSSIBLE_ENEMIES];
bool enemyTexturesLoaded = false;
void init_enemies(SDL_Renderer *renderer){
    // create sprite sheet
    enemiesTexture = IMG_LoadTexture(renderer, ENEMY_SPRITE_DIR);
    enemyTexturesLoaded = true;
}

int redistribute_enemy_array(){
    if(total_enemies == 0){return 0;}
    int arr_count = 0;
    for(int i = 0; i < POSSIBLE_ENEMIES; i++){
        if(enemies[i].created == true){
            //printf("Set index[%i] from prev index[%i]\n", arr_count, i);
            enemies[arr_count] = enemies[i];
            arr_count++;
        }
    }
    for(int i = 0; i < (POSSIBLE_ENEMIES-arr_count); i++) {
        Enemy proj = {
            .created = false,
        };
        //printf("Zeroing out index[%i]\n", arr_count+i);
        enemies[arr_count+i] = proj;
    }
    return arr_count;
}

void create_beader(){
    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .w = 70,
        .h = 70
    };
    Enemy enemy = {
        .created = true,
        .hp = 1,
        .last_shot = 0.0f,
        .reloading = false,
        .shoot_reload_interval_seconds = 3.0,
        .spriteRect = rect,
        .state = ENEMY_STATE_RECHARGING,
        .type = ENEMY_TYPE_BEADER,
        .x = 0.0f,
        .x_vel = 0.0f,
        .y = 0.0f,
        .y_vel = 0.0f
    };

    int next_element = redistribute_enemy_array();
    enemies[next_element] = enemy;
}
void create_enemy(EnemyType type){
    switch (type){
        case ENEMY_TYPE_BEADER:
            create_beader();
            break;
        default:
            break;
    }
}

void move_enemies(){

}

void tick_enemies(){
    move_enemies();
}

void draw_enemies(SDL_Renderer *renderer){
    if(!enemyTexturesLoaded){init_enemies(renderer);}
    for(int i = 0; i < POSSIBLE_ENEMIES; i++){
        if(enemies[i].created == true){
            int x = round(enemies[i].x);
            int y = round(enemies[i].y);
            SDL_Rect temp = {
                .x = x,
                .y = y,
                .w = 70,
                .h = 70
            };
            switch(enemies[i].type){
            case ENEMY_TYPE_BEADER:
                if(SDL_RenderCopy(renderer, enemiesTexture, &enemies[i].spriteRect, &temp) != 0){
                    printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
                }
                break;
            default:
                break;
            }
        }
    }
}