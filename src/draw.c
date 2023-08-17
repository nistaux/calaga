#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

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

Text startText;
SDL_Rect startRect;
Text optionsText;
SDL_Rect optionsRect;
Text scoreText;
SDL_Rect scoreRect;
Text quitText;
SDL_Rect quitRect;

bool title_selections_loaded = false;
void load_title_font_selections(SDL_Renderer *renderer){
    startText.selected = true;
    optionsText.selected = false;
    scoreText.selected = false;
    quitText.selected = false;
    create_text(renderer, TITLE_FONT, 95, "Start", &startText);
    create_text(renderer, TITLE_FONT, 95, "Options", &optionsText);
    create_text(renderer, TITLE_FONT, 95, "Scores", &scoreText);
    create_text(renderer, TITLE_FONT, 95, "Quit", &quitText);
    title_selections_loaded = true;
}

void draw_title_font_selections(SDL_Renderer *renderer){
    if(!title_selections_loaded){
        load_title_font_selections(renderer);
    }
    int start_x = 255;
    int interval = 115;

    startRect.x = (GAME_WIDTH-startText.w)/2;
    startRect.y = start_x;
    startRect.w = startText.w;
    startRect.h = startText.h;

    if(SDL_RenderCopy(renderer, startText.texture, NULL, &startRect) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    optionsRect.x = (GAME_WIDTH-optionsText.w)/2;
    optionsRect.y = start_x+(interval*1);
    optionsRect.w = optionsText.w;
    optionsRect.h = optionsText.h;

    if(SDL_RenderCopy(renderer, optionsText.texture, NULL, &optionsRect) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    scoreRect.x = (GAME_WIDTH-scoreText.w)/2;
    scoreRect.y = start_x+(interval*2);
    scoreRect.w = scoreText.w;
    scoreRect.h = scoreText.h;

    if(SDL_RenderCopy(renderer, scoreText.texture, NULL, &scoreRect) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    quitRect.x = (GAME_WIDTH-quitText.w)/2;
    quitRect.y = start_x+(interval*3);
    quitRect.w = quitText.w;
    quitRect.h = quitText.h;

    if(SDL_RenderCopy(renderer, quitText.texture, NULL, &quitRect) != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
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
    draw_title_font_selections(renderer);

}