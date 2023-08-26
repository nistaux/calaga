#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "enemies.h"
#include "field.h"

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

void create_beader(MoveType moveType, int startLocation){
    int fieldLoc = find_field_space(moveType);
    take_field_space(fieldLoc, moveType);
    
    int top_buffer = 50;
    int field_border = 15;
    int x = 0;
    int y = 0;

    if(fieldLoc < 6){
        x = (100*fieldLoc)+field_border;
        y = (top_buffer+field_border);
    }else if(6 <= fieldLoc && fieldLoc < 12){
        x = (100*(fieldLoc-6)+field_border);
        y = (top_buffer+field_border+100);
    }else if(12 <= fieldLoc && fieldLoc < 18){
        x = (100*(fieldLoc-12)+field_border);
        y = (top_buffer+field_border+200);
    }else {
        x = (100*(fieldLoc-18)+field_border);
        y = (top_buffer+field_border+300);
    }
    
    SDL_Rect spriteRect = { .x = 0, .y = 0, .w = 70, .h = 70};
    Enemy enemy = {
        .created = true,
        .hp = 1,
        .last_shot = 0.0f,
        .reloading = false,
        .shoot_reload_interval_seconds = 3.0,
        .spriteRect = spriteRect,
        .state = ENEMY_STATE_RECHARGING,
        .type = ENEMY_TYPE_BEADER,
        .moveType = moveType,
        .fieldLocation = startLocation,
        .x_loc = (float)x,
        .y_loc = (float)y,
        .x = (float)x,
        .x_vel = 0.0f,
        .y = (float)y,
        .y_vel = 0.0f,
        .speed = 0.20f
    };

    switch(enemy.fieldLocation){
    case 1:
        enemy.x += + 100.0f;
        if(enemy.moveType == MOVE_TYPE_LEFTRIGHT){
            enemy.x_vel = -1.0f;
        }else{
            enemy.y_vel = 1.0f;
        }
        break;
    case 2:
        enemy.y += 100.0f;
        enemy.y_vel = -1.0f;
        break;
    case 3:
        enemy.x += 100.0f;
        enemy.y += 100.0f;
        enemy.x_vel = -1.0f;
        break;
    default:
        enemy.x_vel = 1.0f;
        break;
    }

    int next_element = redistribute_enemy_array();
    enemies[next_element] = enemy;
    total_enemies++;
}
void create_enemy(EnemyType type, MoveType moveType, int startLocation){
    switch (type){
    case ENEMY_TYPE_BEADER:
        create_beader(moveType, startLocation);
        break;
    default:
        break;
    }
}

void move_type_circle(int enemyIndex){
    if(enemies[enemyIndex].x == enemies[enemyIndex].x_loc && enemies[enemyIndex].y < enemies[enemyIndex].y_loc){
        enemies[enemyIndex].y = enemies[enemyIndex].y_loc;
        enemies[enemyIndex].y_vel = 0.0f;
        enemies[enemyIndex].x_vel = 1.0f;
    }
    if(enemies[enemyIndex].x == enemies[enemyIndex].x_loc+100.0f && enemies[enemyIndex].y > enemies[enemyIndex].y_loc+100.0f){
        enemies[enemyIndex].y = enemies[enemyIndex].y_loc+100.0f;
        enemies[enemyIndex].y_vel = 0.0f;
        enemies[enemyIndex].x_vel = -1.0f;
    }
    if(enemies[enemyIndex].y == enemies[enemyIndex].y_loc && enemies[enemyIndex].x > enemies[enemyIndex].x_loc+100.0f){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc+100.0f;
        enemies[enemyIndex].y_vel = 1.0f;
        enemies[enemyIndex].x_vel = 0.0f;
    }
    if(enemies[enemyIndex].y == enemies[enemyIndex].y_loc+100.0f && enemies[enemyIndex].x < enemies[enemyIndex].x_loc){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc;
        enemies[enemyIndex].y_vel = -1.0f;
        enemies[enemyIndex].x_vel = 0.0f;
    }
    enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
    enemies[enemyIndex].y += enemies[enemyIndex].y_vel*enemies[enemyIndex].speed;
}

void move_type_leftright(int enemyIndex){
    if(enemies[enemyIndex].x < enemies[enemyIndex].x_loc){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc;
        enemies[enemyIndex].x_vel = 1.0f;
    }
    if(enemies[enemyIndex].x > enemies[enemyIndex].x_loc+100.0f){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc+100.0f;
        enemies[enemyIndex].x_vel = -1.0f;
    }
    enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
}

void move_enemies(){
    for(int enemy = 0; enemy < POSSIBLE_ENEMIES; enemy++){
        if(enemies[enemy].created == true){
            switch(enemies[enemy].moveType){
            case MOVE_TYPE_CIRCLE:
                move_type_circle(enemy);
                break;
            case MOVE_TYPE_LEFTRIGHT:
                move_type_leftright(enemy);
                break;
            default:
                break;
            }
        }
    }
}

void tick_enemies(){
    move_enemies();
}

void draw_enemies(SDL_Renderer *renderer){
    if(!enemyTexturesLoaded){init_enemies(renderer);}
    for(int i = 0; i < POSSIBLE_ENEMIES; i++){
        if(enemies[i].created == true){
            int x = (int)round(enemies[i].x);
            int y = (int)round(enemies[i].y);
            SDL_Rect renderRect = {
                .x = x,
                .y = y,
                .w = 70,
                .h = 70
            };
            switch(enemies[i].type){
            case ENEMY_TYPE_BEADER:
                if(SDL_RenderCopy(renderer, enemiesTexture, &enemies[i].spriteRect, &renderRect) != 0){
                    printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
                }
                break;
            default:
                break;
            }
        }
    }
}