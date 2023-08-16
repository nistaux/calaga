#include <stdio.h>
#include <SDL.h>

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

}

void present_renderer(){
    SDL_RenderPresent(renderer);
}