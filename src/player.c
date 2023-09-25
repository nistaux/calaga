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
#include "enemies.h"
#include "events.h"

Player player;
SDL_Texture *player_texture;
SDL_Rect playerRect;
const float PLAYER_DEFAULT_X = 63.0;
const float PLAYER_DEFAULT_Y = 220.0;
bool toggle_shoot = false;

void init_player(float x, float y){
    Player temp = {
        .x = x,
        .y = y,
        .x_vel = 0.0,
        .y_vel = 0.0,
        .hp = 3,
        .shoot_reload_interval_seconds = 0.333f,
        .last_shot = 0.0,
        .transparent = false,
        .reloading = false,
        .state = PLAYER_STATE_TITLE
    };

    player = temp;
}
void set_player_loc(float x, float y){
    player.x = x;
    player.y = y;
}
void reset_player_vel(){
    player.x_vel = 0.0f;
    player.y_vel = 0.0f;
}
float get_player_x(){
    return player.x;
}
int get_player_hp(){
    return player.hp;
}
SDL_Texture *get_player_texture(){
    return player_texture;
}
void shoot_player(){
    if(!player.reloading && toggle_shoot){
        SDL_Rect srcRect = {
            .x = 0,
            .y = 0,
            .w = 17,
            .h = 20
        };
        SDL_Rect dstRect = {
            .x = 0,
            .y = 0,
            .w = 17,
            .h = 20
        };
        Projectile proj = {
            .srcRect = srcRect,
            .dstRect = dstRect,
            .speed = 18.0f,
            .type = PROJ_PLAYER,
            .x = (player.x + (((float)playerRect.w/2)-9)),
            .y = (player.y-10),
            .x_vel = 0.0,
            .y_vel = -0.24,
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
    player.x = 0.0f;
    player.y = 0.0f;
    player.x_vel = 0.0f;
    player.y_vel = 0.0f;
    player.hp = 3;
    player.last_shot = 0.0;
    player.reloading = false;
    player.state = PLAYER_STATE_TITLE;
    toggle_shoot = false;
    set_player_default_location();
    clear_enemies();
    clear_projectiles();
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
        
    float speed = 0.0002f;
    float flipNumber = 0.06f;
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

bool fading_target_set = false;
float fading_target_x;
float fading_target_y;
float fading_x_timing;
float fading_y_timing;
void clear_fading_data(){
    fading_target_set = false;
    fading_target_x = 0.0f;
    fading_target_y = 0.0f;
    fading_x_timing = 0.0f;
    fading_y_timing = 0.0f;
}

void move_player_title_fading(){
    if(!fading_target_set){
        fading_target_x = ((GAME_WIDTH/2) - (playerRect.w/2));
        fading_target_y = (GAME_HEIGHT-playerRect.h)-55;
        float x_diffy = fading_target_x - player.x;
        float y_diffy = fading_target_y - player.y;

        float ticksPerSecond = 250.0;
        float secondsToTarget = 1.25;
        fading_x_timing = x_diffy/(ticksPerSecond*secondsToTarget);
        fading_y_timing = y_diffy/(ticksPerSecond*secondsToTarget);
        fading_target_set = true;
    }
    if(player.y <= fading_target_y || player.x <= fading_target_x){
        player.x += fading_x_timing;
        player.y += fading_y_timing;
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
    switch(get_game()->play.state){
    case PLAY_STATE_ALIVE:
        if(!player_outside_left_bounds && !player_outside_right_bounds){
            player.x += player.x_vel;
        }
        break;
    case PLAY_STATE_DEAD:
        //printf("just testing huh\n");
        player.x = ((GAME_WIDTH/2) - (playerRect.w/2));
        player.y = (GAME_HEIGHT-playerRect.h)-55;
        if(SDL_GetTicks64() - player.t_time >= 400){
            player.transparent = !player.transparent;
            player.t_time = SDL_GetTicks64();
        }
        if(player.transparent){
            //printf("player is tp\n");
            SDL_SetTextureAlphaMod(player_texture, 0);
        }else{{
            SDL_SetTextureAlphaMod(player_texture, 255);
            //printf("player is NOT tp\n");
        }}
        if(SDL_GetTicks64() - player.dead_time >= 2000){
            SDL_SetTextureAlphaMod(player_texture, 255);
            get_game()->play.state = PLAY_STATE_ALIVE;
        }
            
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

bool check_player_projectiles(){
    Projectile proj;
    Projectile *p = get_projectiles();

    bool player_hit_by_projectile;

    for(int i = 0; i < possible_projectiles; i++){

        proj = *(p + i);
        if(proj.created == false || proj.type == PROJ_PLAYER){continue;}

        float player_buffer = 15.0f;
        float bottom_of_proj = proj.y+proj.srcRect.h;
        float bottom_of_player = player.y+70.0f-player_buffer;

        float top_of_proj = proj.y;
        float top_of_player = player.y+player_buffer;
        
        float left_of_proj = proj.x;
        float left_of_player = player.x+player_buffer;
        
        float right_of_proj = proj.x+proj.srcRect.w;
        float right_of_player = player.x+70.0f-player_buffer;
        
        bool bottom_left_proj_in_player_hit_box = (
            bottom_of_proj > top_of_player &&
            bottom_of_proj < bottom_of_player &&
            left_of_proj < right_of_player &&
            left_of_proj > left_of_player
        );
        bool bottom_right_proj_in_player_hit_box = (
            bottom_of_proj > top_of_player &&
            bottom_of_proj < bottom_of_player &&
            right_of_proj > left_of_player &&
            right_of_proj < right_of_player
        );
        bool top_left_proj_in_player_hit_box = (
            top_of_proj > top_of_player &&
            top_of_proj < bottom_of_player &&
            left_of_proj < right_of_player &&
            left_of_proj > left_of_player
        );
        bool top_right_proj_in_player_hit_box = (
            top_of_proj > top_of_player &&
            top_of_proj < bottom_of_player &&
            right_of_proj > left_of_player &&
            right_of_proj < right_of_player
        );

        player_hit_by_projectile = (
            bottom_left_proj_in_player_hit_box ||
            bottom_right_proj_in_player_hit_box ||
            top_left_proj_in_player_hit_box ||
            top_right_proj_in_player_hit_box
        );

        if(player_hit_by_projectile){
            printf("hit by projectile!\n");
            return true;
        }
    }
    return false;
}

bool check_player_enemies(){
    Enemy enemy;
    Enemy *pEnemy = get_enemies();

    bool player_hit_by_enemy;

    for(int i = 0; i < POSSIBLE_ENEMIES; i++){

        enemy = *(pEnemy + i);
        // if enemy isnt created or the enemy isnt a dagger skip to next enemy
        if(!enemy.created || enemy.type == ENEMY_TYPE_BEADER || enemy.type == ENEMY_TYPE_TSHOT){continue;}

        float player_buffer = 15.0f;
        float bottom_of_enemy = enemy.y+enemy.spriteRect.h;
        float bottom_of_player = player.y+70.0f-player_buffer;

        float top_of_enemy = enemy.y;
        float top_of_player = player.y+player_buffer;
        
        float left_of_enemy = enemy.x+12;
        float left_of_player = player.x+player_buffer;
        
        float right_of_enemy = enemy.x+enemy.spriteRect.w-12;
        float right_of_player = player.x+70.0f-player_buffer;
        
        bool bottom_left_enemy_in_player_hit_box = (
            bottom_of_enemy > top_of_player &&
            bottom_of_enemy < bottom_of_player &&
            left_of_enemy < right_of_player &&
            left_of_enemy > left_of_player
        );
        bool bottom_right_enemy_in_player_hit_box = (
            bottom_of_enemy > top_of_player &&
            bottom_of_enemy < bottom_of_player &&
            right_of_enemy > left_of_player &&
            right_of_enemy < right_of_player
        );
        bool top_left_enemy_in_player_hit_box = (
            top_of_enemy > top_of_player &&
            top_of_enemy < bottom_of_player &&
            left_of_enemy < right_of_player &&
            left_of_enemy > left_of_player
        );
        bool top_right_enemy_in_player_hit_box = (
            top_of_enemy > top_of_player &&
            top_of_enemy < bottom_of_player &&
            right_of_enemy > left_of_player &&
            right_of_enemy < right_of_player
        );

        player_hit_by_enemy = (
            bottom_left_enemy_in_player_hit_box ||
            bottom_right_enemy_in_player_hit_box ||
            top_left_enemy_in_player_hit_box ||
            top_right_enemy_in_player_hit_box
        );

        if(player_hit_by_enemy){
            printf("hit by enemy!\n");
            return true;
        }
    }
    return false;
}

void check_player(){
    if(player.hp == 0){get_game()->play.state = PLAY_STATE_OVER;}

    bool player_hit_by_projectile = check_player_projectiles();
    bool player_hit_by_enemy = check_player_enemies();

    if((player_hit_by_projectile || player_hit_by_enemy) && player.hp > 1){
        player.dead_time = SDL_GetTicks64();
        player.t_time = SDL_GetTicks64();
        player.reloading = false;
        set_toggle_shoot_player(false);
        get_game()->play.state = PLAY_STATE_DEAD;
        player.hp -= 1;
        clear_projectiles();
        clear_enemies();
        stop_all_sounds();
        play_sound(PLAYERDEAD_SOUND);
        clear_fading_data();
    }else if((player_hit_by_projectile || player_hit_by_enemy) && player.hp == 1){
        player.hp -= 1;
        stop_all_sounds();
        set_toggle_shoot_player(false);
        start_over_music();
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
    if(get_game_state() != GAME_STATE_TITLE){
        check_player();
        check_reloading(deltaTime);
        shoot_player();
    }
}