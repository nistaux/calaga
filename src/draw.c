#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "player.h"
#include "font.h"
#include "defs.h"
#include "game.h"

#define TITLE_PATH      "./res/images/title.png"
#define BACKGROUND_PATH "./res/images/background.png"

bool title_loaded = false;
SDL_Texture *title_img;

float player_x = 63.0f;
float player_y = 220.0f;

void init_title(SDL_Renderer *renderer) {
    title_img = IMG_LoadTexture(renderer, TITLE_PATH);
    get_background()->texture = IMG_LoadTexture(renderer, BACKGROUND_PATH);

    init_player(player_x,player_y);
    load_player(renderer);
    title_loaded = true;
}

void draw_title_main(SDL_Renderer *renderer){
    int return_code;

    return_code = SDL_RenderClear(renderer);
    if(return_code != 0){
        printf("SDL: Error Clearing Renderer - %s\n", SDL_GetError());
    }

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

    SDL_Rect titleRect = {
        .w = 445,
        .h = 105,
        .x = 95,
        .y = 75
    };
    return_code = SDL_RenderCopy(renderer, title_img, NULL, &titleRect);
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
    draw_player(renderer);

    return_code = SDL_RenderCopy(renderer, get_player_texture(), NULL, get_player_rect());
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
    draw_title_font_selections(renderer);
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

void draw_title_screen(SDL_Renderer *renderer){
    if(!title_loaded){
        init_title(renderer);
    }
    switch(get_game()->title.state){
        case TITLE_STATE_MAIN:
            draw_title_main(renderer);
            break;
        case TITLE_STATE_OPTIONS:
            draw_title_options(renderer);
            break;
        case TITLE_STATE_SCORES:
            draw_title_scores(renderer);
        default:
            break;
    }
}