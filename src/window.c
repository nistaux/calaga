#include <stdio.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "defs.h"
#include "renderer.h"

SDL_Window *window;

void init_sdl(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_OPENGL);
    init_renderer(window);
}

void end_sdl(){
    SDL_DestroyRenderer(SDL_GetRenderer(window));
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Window *get_window() {
    return window;
}

void update_window() {
    update_renderer();
    present_renderer();
}