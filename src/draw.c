#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

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

void draw_background(SDL_Renderer *renderer) {
    if(SDL_RenderCopy(renderer, background_img, NULL, NULL) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}

void draw_title_image(SDL_Renderer *renderer){
    SDL_Rect title = {
        .w = 445,
        .h = 105,
        .x = 95,
        .y = 75
    };
    if(SDL_RenderCopy(renderer, title_img, NULL, &title) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}

void draw_player(SDL_Renderer *renderer) {

}

void draw_title_screen(SDL_Renderer *renderer){
    if(!title_loaded){
        init_title(renderer);
    }
    if(SDL_RenderClear(renderer) != 0){
        printf("SDL: Error Clearing Renderer - %s\n", SDL_GetError());
    }

    draw_background(renderer);
    draw_title_image(renderer);
    draw_player(renderer);

}