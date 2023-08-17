#include <stdio.h>

#include <SDL.h>

#include "game.h"
#include "draw.h"

SDL_Renderer *renderer;

void init_renderer(SDL_Window *window){
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer != NULL){
        //init_ui(renderer);
    }else {
        printf("SDL: Error Creating Renderer - %s", SDL_GetError());
    }
}

void update_renderer(){
    switch(get_game()->state) {
        case TITLE:
            draw_title_screen(renderer);
            break;
        default:
            break;
    }
}

void present_renderer(){
    SDL_RenderPresent(renderer);
}