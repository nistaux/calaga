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
    printf("title init'd");
}

void draw_title(SDL_Renderer *renderer){
    if(!title_loaded){
        init_title(renderer);
    }
}