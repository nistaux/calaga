#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

#include "projectile.h"

int total_projectiles = 0;
Projectile projectiles[possible_projectiles];

int redistribute_projectile_array(){
    if(total_projectiles == 0){return 0;}
    Projectile* ptr = projectiles;
    int arr_count = 0;
    for(int i = 0; i < possible_projectiles; i++){
        if(projectiles[i].created == true){
            printf("Set index[%i] from prev index[%i]\n", arr_count, i);
            projectiles[arr_count] = projectiles[i];
            arr_count++;
        }
    }
    for(int i = 0; i < (possible_projectiles-arr_count); i++) {
        Projectile proj = {
            .created = false,
        };
        printf("Zeroing out index[%i]\n", arr_count+i);
        projectiles[arr_count+i] = proj;
    }
    return arr_count;
}
void create_projectile(SDL_Renderer *renderer, Projectile temp){
    SDL_Texture *texture;
    switch(temp.type){
        case PROJ_PLAYER:
            // load in projectile texture
            break;
        default:
            break;
    }
    Projectile projectile = {
        .created = true,
        .x = temp.x,
        .y = temp.y,
        .x_vel = temp.x_vel,
        .y_vel = temp.y_vel,
        .speed = temp.speed,
        .type = temp.type,
        .rect = temp.rect,
        .texture = texture,
    };

    int next_element = redistribute_projectile_array();
    projectiles[next_element] = projectile;
    total_projectiles++;
}
void destroy_projectile(int index){
    projectiles[index].created = false;
    total_projectiles--;
    SDL_DestroyTexture(projectiles[index].texture);
}
void move_projectiles(){

}
void draw_projectiles(SDL_Renderer *renderer){

}