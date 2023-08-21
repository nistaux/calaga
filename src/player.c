#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "player.h"
#include "game.h"
#include "defs.h"
#include "projectile.h"
#include "audio.h"

#define PLAYER_IMAGE_DIR "./res/images/player.png"

Player player;
SDL_Texture *player_texture;
SDL_Rect playerRect;
const float PLAYER_DEFAULT_X = 63.0f;
const float PLAYER_DEFAULT_Y = 220.0f;
bool toggle_shoot = false;

void init_player(float x, float y){
    Player temp = {
        .x = x,
        .y = y,
        .x_vel = 0.0f,
        .y_vel = 0.0f,
        .hp = 3,
        .shoot_reload_interval_seconds = 0.18,
        .last_shot = 0.0,
        .reloading = false,
        .state = TITLE
    };

    player = temp;
}
void shoot_player(){
    if(!player.reloading && toggle_shoot){
        SDL_Rect rect = {
            .x = 0,
            .y = 0,
            .w = 17,
            .h = 20
        };
        Projectile proj = {
            .rect = rect,
            .speed = 18.0,
            .type = PROJ_PLAYER,
            .x = (player.x + (((float)playerRect.w/2)-9)),
            .y = (player.y-10),
            .x_vel = 0.0,
            .y_vel = -1.0,
        };
        create_projectile(proj);
        play_sound(PLAYER_SHOOT_SOUND);
        player.reloading = true;
        player.last_shot = 0.0;
    }
    
}
void set_toggle_shoot_player(bool set){
    toggle_shoot = set;
    
}

void set_player_default_location(){
    player.x = PLAYER_DEFAULT_X;
    player.y = PLAYER_DEFAULT_Y;
}
void reset_player(){
    Player temp = {
        .x = 0.0f,
        .y = 0.0f,
        .x_vel = 0.0f,
        .y_vel = 0.0f,
        .hp = 3,
        .shoot_reload_interval_seconds = 0.18,
        .last_shot = 0.0,
        .reloading = false,
        .state = TITLE
    };

    player = temp;
    toggle_shoot = false;
    set_player_default_location();
}
void add_player_x_vel(float vel){
    player.x_vel = vel+player.x_vel;
}

// -------------- DRAW PLAYER SECTION --------------
bool player_loaded = false;
void load_player(SDL_Renderer *renderer){
    if(!player_loaded){
        player_texture = IMG_LoadTexture(renderer, PLAYER_IMAGE_DIR);
        player_loaded = true;
    }
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

// -------------- MOVE TITLE SECTION --------------
bool flip = false;
void move_player_title_main() {
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
bool target_set = false;
float target_x;
float target_y;
float x_timing;
float y_timing;
void move_player_title_fading(){
    if(!target_set){
        target_x = ((GAME_WIDTH/2) - (playerRect.w/2));
        target_y = (GAME_HEIGHT-playerRect.h)-55;
        float x_diffy = target_x - player.x;
        float y_diffy = target_y - player.y;

        float ticksPerSecond = 60.0;
        float secondsToTarget = 1.5;
        x_timing = x_diffy/(ticksPerSecond*secondsToTarget);
        y_timing = y_diffy/(ticksPerSecond*secondsToTarget);
        target_set = true;
    }
    if(player.y <= target_y){
        player.x += x_timing;
        player.y += y_timing;
    }else{
        get_game()->title.state = TITLE_STATE_MAIN;
        set_game_state(GAME_STATE_PLAY);
    }
}
void move_player_title(){
    switch(get_game()->title.state){
        case TITLE_STATE_FADING:
            move_player_title_fading();
            break;
        default:
            move_player_title_main();
            break;
    }
}

// -------------- MOVE PLAY SECTION --------------
void move_player_play(){
    bool player_outside_right_bounds = ((player.x+player.x_vel+70) >= GAME_WIDTH);
    bool player_outside_left_bounds = ((player.x+player.x_vel) <= 0);
    switch(get_game()->title.state){
        case PLAY_STATE_ALIVE:
            if(!player_outside_left_bounds && !player_outside_right_bounds){
                player.x += player.x_vel;
            }
            break;
        default:
            break;
    }
}

// -------------- SYSTEM PLAY SECTION --------------
void move_player(){
    switch(get_game_state()){
        case GAME_STATE_TITLE:
            move_player_title();
        case GAME_STATE_PLAY:
            move_player_play();
        default:
            break;
    }
}
void check_reloading(float deltaTime){
    if(player.reloading){
        float delta = get_tick_delta();
        player.last_shot += delta;
        if(player.last_shot >= player.shoot_reload_interval_seconds){
            player.reloading = false;
        }
    }
}
void tick_player(float deltaTime){
    move_player();
    check_reloading(deltaTime);
    shoot_player();
}