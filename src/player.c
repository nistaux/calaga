#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "player.h"
#include "game.h"

#define PLAYER_IMAGE_DIR "./res/images/player.png"

Player player;
SDL_Texture *player_texture;
SDL_Rect playerRect;

void init_player(float x, float y){
    Player temp = {
        .x = x,
        .y = y,
        .x_vel = 0.0f,
        .y_vel = 0.0f,
        .hp = 3,
        .state = IDLE
    };

    player = temp;
}

Player *get_player() {
    return &player;
}

SDL_Texture *get_player_texture() {
    return player_texture;
}

SDL_Rect *get_player_rect() {
    return &playerRect;
}

bool player_loaded = false;
void load_player(SDL_Renderer *renderer){
    if(!player_loaded){
        player_texture = IMG_LoadTexture(renderer, PLAYER_IMAGE_DIR);
        player_loaded = true;
    }
}

void set_player_state(PlayerState state){
    player.state = state;
}

void draw_player(SDL_Renderer *renderer) {
    int return_code;
    int x = round(player.x);
    int y = round(player.y);
    SDL_Rect temp = {
        .x = x,
        .y = y,
        .w = 70,
        .h = 70
    };
    playerRect = temp;
    if(SDL_RenderCopy(renderer, player_texture, NULL, &playerRect) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}

bool flip = false;
void move_player(){
    if(get_game_state() == GAME_STATE_TITLE){
        player.y = player.y + player.y_vel;
        
        float speed = 0.002f;
        float flipNumber = 0.2f;
        float oppNumber = flipNumber*-1.0;

        if(player.y_vel >= oppNumber && flip){
            player.y_vel = player.y_vel - speed;
        }
        if(player.y_vel <= flipNumber && !flip){
            player.y_vel = player.y_vel + speed;
        }

        if(player.y_vel >= flipNumber || player.y_vel <= oppNumber){
            flip = !flip;
        }
    }
}