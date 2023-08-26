#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "projectile.h"
#include "defs.h"

#define PLAYER_PROJECTILE_DIR "./assets/images/player_projectile.png"

int total_projectiles = 0;
Projectile projectiles[possible_projectiles];
SDL_Texture *playerProjectile;

bool projectile_textures_loaded = false;
void init_projectile_textures(SDL_Renderer *renderer){
    playerProjectile = IMG_LoadTexture(renderer, PLAYER_PROJECTILE_DIR);
    projectile_textures_loaded = true;
}

int redistribute_projectile_array(){
    if(total_projectiles == 0){return 0;}
    int arr_count = 0;
    for(int i = 0; i < possible_projectiles; i++){
        if(projectiles[i].created == true){
            //printf("Set index[%i] from prev index[%i]\n", arr_count, i);
            projectiles[arr_count] = projectiles[i];
            arr_count++;
        }
    }
    for(int i = 0; i < (possible_projectiles-arr_count); i++) {
        Projectile proj = {
            .created = false,
        };
        //printf("Zeroing out index[%i]\n", arr_count+i);
        projectiles[arr_count+i] = proj;
    }
    return arr_count;
}
void create_projectile(Projectile temp){
    Projectile projectile = {
        .created = true,
        .x = temp.x,
        .y = temp.y,
        .x_vel = temp.x_vel,
        .y_vel = temp.y_vel,
        .speed = temp.speed,
        .type = temp.type,
        .rect = temp.rect,
    };

    int next_element = redistribute_projectile_array();
    projectiles[next_element] = projectile;
    total_projectiles++;
}
void destroy_projectile(int index){
    Projectile zero = {
        .created = false,
    };
    projectiles[index] = zero;
    total_projectiles--;
}
void check_projectile_out_of_bounds(Projectile proj, int index){
    if((proj.x+proj.rect.w) < 0 || proj.x > GAME_WIDTH){
        destroy_projectile(index);
        //printf("set proj out of width bounds\n");
    }
    if(proj.y > GAME_HEIGHT || (proj.y+proj.rect.h) < 0){
        destroy_projectile(index);
        //printf("set proj out of height bounds\n");
    }
}
void move_projectiles(){
    for(int i = 0; i < possible_projectiles; i++){
        if(projectiles[i].created == true){
            projectiles[i].x += projectiles[i].x_vel * projectiles[i].speed;
            projectiles[i].y += projectiles[i].y_vel * projectiles[i].speed;

            projectiles[i].rect.x = round(projectiles[i].x);
            projectiles[i].rect.y = round(projectiles[i].y);
            check_projectile_out_of_bounds(projectiles[i], i);
        }
    }
}
void draw_projectiles(SDL_Renderer *renderer){
    if(!projectile_textures_loaded){init_projectile_textures(renderer);}
    for(int i = 0; i < possible_projectiles; i++){
        if(projectiles[i].created == true){
            switch(projectiles[i].type){
                case PROJ_PLAYER:
                    SDL_RenderCopy(renderer, playerProjectile, NULL, &projectiles[i].rect);
                    break;
                default:
                    break;
            }
        }
    }
}