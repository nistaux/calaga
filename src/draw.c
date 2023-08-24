#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "player.h"
#include "enemies.h"
#include "font.h"
#include "defs.h"
#include "game.h"
#include "renderer.h"
#include "projectile.h"

#define TITLE_PATH      "./res/images/title.png"
#define BACKGROUND_PATH "./res/images/background.png"

bool title_loaded = false;
SDL_Texture *title_img;

float player_x = 63.0f;
float player_y = 220.0f;

void draw_background(SDL_Renderer *renderer) {
    int return_code;

    SDL_Rect b2Rect = {
        .h = GAME_HEIGHT,
        .w = GAME_WIDTH,
        .x = 0,
        .y = round(get_background()->b2_loc)
    };
    return_code = SDL_RenderCopy(renderer, get_background()->texture, NULL, &b2Rect);
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    SDL_Rect b1Rect = {
        .h = GAME_HEIGHT,
        .w = GAME_WIDTH,
        .x = 0,
        .y = round(get_background()->b1_loc)
    };
    return_code = SDL_RenderCopy(renderer, get_background()->texture, NULL, &b1Rect);
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}

// -------------- TITLE SECTION --------------
void init_title(SDL_Renderer *renderer) {
    title_img = IMG_LoadTexture(renderer, TITLE_PATH);
    get_background()->texture = IMG_LoadTexture(renderer, BACKGROUND_PATH);

    init_player(player_x,player_y);
    load_player(renderer);
    title_loaded = true;
}
void draw_title_image(SDL_Renderer *renderer){
    SDL_Rect titleRect = {
        .w = 445,
        .h = 105,
        .x = 95,
        .y = 75
    };
    int return_code;
    return_code = SDL_RenderCopy(renderer, title_img, NULL, &titleRect);
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}
void draw_title_options(SDL_Renderer *renderer){
    int return_code;

    return_code = SDL_RenderClear(renderer);
    if(return_code != 0){
        printf("SDL: Error Clearing Renderer - %s\n", SDL_GetError());
    }
    
}
void draw_title_scores(SDL_Renderer *renderer){
    int return_code;

    return_code = SDL_RenderClear(renderer);
    if(return_code != 0){
        printf("SDL: Error Clearing Renderer - %s\n", SDL_GetError());
    }
}
void draw_title_fade(SDL_Renderer *renderer) {
    
}
void draw_title_screen(SDL_Renderer *renderer){
    if(!title_loaded){
        init_title(renderer);
    }
    clear_renderer();
    draw_background(renderer);
    switch(get_game()->title.state){
        case TITLE_STATE_MAIN:
            draw_title_image(renderer);
            draw_title_main_selections(renderer);
            break;
        case TITLE_STATE_OPTIONS:
            draw_title_image(renderer);
            draw_title_options(renderer);
            break;
        case TITLE_STATE_SCORES:
            draw_title_image(renderer);
            draw_title_scores(renderer);
        case TITLE_STATE_FADING:
            draw_title_fade(renderer);
        default:
            break;
    }
    draw_player(renderer);
}

// -------------- PLAY SECTION --------------
bool play_screen_pause_loaded = false;
SDL_Rect pauseRect = {
    .x = 0,
    .y = 0,
    .w = GAME_WIDTH,
    .h = GAME_HEIGHT
};
void draw_play_screen_paused(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 185);
    SDL_RenderFillRect(renderer, &pauseRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    draw_play_paused_main_selections(renderer);
}
void draw_play_screen(SDL_Renderer *renderer) {
    clear_renderer();
    draw_background(renderer);
    draw_projectiles(renderer);
    draw_enemies(renderer);
    draw_player(renderer);
    // everything else

    // last draw pause if game is paused
    if(get_game()->play.state == PLAY_STATE_PAUSED){
        draw_play_screen_paused(renderer);
    }
}