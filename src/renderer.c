#include <stdio.h>

#include <SDL.h>

#include "game.h"
#include "draw.h"

SDL_Renderer *renderer;

void init_renderer(SDL_Window *window){
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        printf("SDL: Error Creating Renderer - %s", SDL_GetError());
        exit(0);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
}

void clear_renderer(){
    int return_code;
    return_code = SDL_RenderClear(renderer);
    if(return_code != 0){
        printf("SDL: Error Clearing Renderer - %s\n", SDL_GetError());
    }
}

void update_renderer(){
    switch(get_game_state()) {
        case GAME_STATE_TITLE:
            draw_title_screen(renderer);
            break;
        case GAME_STATE_PLAY:
            draw_play_screen(renderer);
        default:
            break;
    }
}

void present_renderer(){
    SDL_RenderPresent(renderer);
}