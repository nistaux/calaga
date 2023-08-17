#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "player.h"
#include "font.h"
#include "defs.h"

#define TITLE_PATH      "./res/images/title.png"
#define BACKGROUND_PATH "./res/images/background.png"

bool title_loaded = false;
SDL_Texture *title_img;
SDL_Texture *background_img;

void init_title(SDL_Renderer *renderer) {
    title_img = IMG_LoadTexture(renderer, TITLE_PATH);
    background_img = IMG_LoadTexture(renderer, BACKGROUND_PATH);
    title_loaded = true;
}

void draw_title_screen(SDL_Renderer *renderer){
    if(!title_loaded){
        init_title(renderer);
    }

    int return_code;

    return_code = SDL_RenderClear(renderer);
    if(return_code != 0){
        printf("SDL: Error Clearing Renderer - %s\n", SDL_GetError());
    }

    return_code = SDL_RenderCopy(renderer, background_img, NULL, NULL);
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
    draw_title_font_selections(renderer);

}