#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>

#include "enemies.h"

SDL_Texture *enemiesTexture;
int total_enemies = 0;
Enemy enemies[POSSIBLE_ENEMIES];

void init_enemies(SDL_Renderer *renderer){
    // create sprite sheet
    enemiesTexture = IMG_LoadTexture(renderer, ENEMY_SPRITE_DIR);
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
    Enemy enemy;
    // create the struct w/ props

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
    
}