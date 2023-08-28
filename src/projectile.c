#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "projectile.h"
#include "defs.h"

int total_projectiles = 0;
Projectile projectiles[possible_projectiles];
SDL_Texture *projectileMap;

bool projectile_textures_loaded = false;
void init_projectile_textures(SDL_Renderer *renderer){
    projectileMap = IMG_LoadTexture(renderer, PROJECTILE_MAP_DIR);
    projectile_textures_loaded = true;
}

Projectile *get_projectiles(){
    Projectile *p;
    p = projectiles;
    return p;
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
    temp.created = true;

    int next_element = redistribute_projectile_array();
    projectiles[next_element] = temp;
    total_projectiles++;
}
void clear_projectiles(){
    for(int i = 0; i < (possible_projectiles); i++) {
        Projectile proj = {
            .created = false,
        };
        projectiles[i] = proj;
    }
}
void destroy_projectile(int index){
    Projectile zero = {
        .created = false,
    };
    projectiles[index] = zero;
    total_projectiles--;
}
void check_projectile_out_of_bounds(Projectile proj, int index){
    if((proj.x+proj.dstRect.w) < 0 || proj.x > GAME_WIDTH){
        destroy_projectile(index);
        //printf("set proj out of width bounds\n");
    }
    if(proj.y > GAME_HEIGHT || (proj.y+proj.dstRect.h) < 0){
        destroy_projectile(index);
        //printf("set proj out of height bounds\n");
    }
}
void move_projectiles(){
    for(int i = 0; i < possible_projectiles; i++){
        if(projectiles[i].created == true){
            projectiles[i].x += projectiles[i].x_vel * projectiles[i].speed;
            projectiles[i].y += projectiles[i].y_vel * projectiles[i].speed;

            projectiles[i].dstRect.x = round(projectiles[i].x);
            projectiles[i].dstRect.y = round(projectiles[i].y);
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
                SDL_RenderCopy(renderer, projectileMap, &projectiles[i].srcRect, &projectiles[i].dstRect);
                break;
            case PROJ_BEADER:
                if(SDL_RenderCopyEx(renderer, projectileMap, &projectiles[i].srcRect, &projectiles[i].dstRect, projectiles[i].angle, NULL, SDL_FLIP_VERTICAL) != 0){
                    printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
                }
            default:
                break;
            }
        }
    }
}