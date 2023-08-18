#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "font.h"
#include "defs.h"
#include "game.h"

#define TITLE_FONT_DIR "./res/fonts/Handjet-Light.ttf"
#define THIN_FONT_DIR "./res/fonts/Handjet-Thin.ttf"
#define REGULAR_FONT_DIR "./res/fonts/Handjet-Thin.ttf"

bool is_ttf_init = false;

void init_ttf() {
    TTF_Init();
    is_ttf_init = true;
}

void create_text(SDL_Renderer *renderer, FontType fontType, int size, const char *string, Text *text){
    if(!is_ttf_init){
        init_ttf();
    }
    SDL_Color selected_color = { 180,75,255 };
    SDL_Color not_selected_color = { 180,255,75 };

    TTF_Font *font;
    switch (fontType)
    {
    case TITLE_FONT:
        font = TTF_OpenFont(TITLE_FONT_DIR, size);
        break;
    case THIN_FONT:
        font = TTF_OpenFont(THIN_FONT_DIR, size);
        break;
    case REGULAR_FONT:
        font = TTF_OpenFont(REGULAR_FONT_DIR, size);
        break;
    default:
        font = NULL;
        break;
    }

    if(!font) {
        printf("Unable to create font!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }

    SDL_Surface *selected_surface = TTF_RenderText_Blended(font, string, selected_color);
    if(!selected_surface) {
        printf("Unable to render text surface!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, string, not_selected_color);
    if(!text_surface) {
        printf("Unable to render text surface!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }

    SDL_Texture *selected_texture = SDL_CreateTextureFromSurface(renderer, selected_surface);
    if(!selected_texture) {
        printf("Unable to create texture from rendered text!\n"
                "SDL2 Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if(!texture) {
        printf("Unable to create texture from rendered text!\n"
                "SDL2 Error: %s\n", SDL_GetError());
        return;
    }
    Text temp_text = {
        .texture = texture,
        .selected_texture = selected_texture,
        .w = text_surface->w,
        .h = text_surface->h,
        .id = text->id
    };

    *text = temp_text;
}

Text startText = {
    .id = 0
};
SDL_Rect startRect;

Text optionsText = {
    .id = 1
};
SDL_Rect optionsRect;

Text scoreText = {
    .id = 2
};
SDL_Rect scoreRect;

Text quitText = {
    .id = 3
};
SDL_Rect quitRect;

bool title_selections_loaded = false;
void load_title_main_selections(SDL_Renderer *renderer){
    create_text(renderer, TITLE_FONT, 95, "Start", &startText);
    create_text(renderer, TITLE_FONT, 95, "Options", &optionsText);
    create_text(renderer, TITLE_FONT, 95, "Scores", &scoreText);
    create_text(renderer, TITLE_FONT, 95, "Quit", &quitText);
    title_selections_loaded = true;
}

void draw_title_main_selections(SDL_Renderer *renderer){
    if(!title_selections_loaded){
        load_title_main_selections(renderer);
    }
    int start_x = 255;
    int interval = 115;
    int return_code;

    startRect.x = (GAME_WIDTH-startText.w)/2;
    startRect.y = start_x;
    startRect.w = startText.w;
    startRect.h = startText.h;

    if(get_game()->title.selection == startText.id){
        return_code = SDL_RenderCopy(renderer, startText.selected_texture, NULL, &startRect);
    }else {
        return_code = SDL_RenderCopy(renderer, startText.texture, NULL, &startRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    optionsRect.x = (GAME_WIDTH-optionsText.w)/2;
    optionsRect.y = start_x+(interval*1);
    optionsRect.w = optionsText.w;
    optionsRect.h = optionsText.h;

    if(get_game()->title.selection == optionsText.id){
        return_code = SDL_RenderCopy(renderer, optionsText.selected_texture, NULL, &optionsRect);
    }else {
        return_code = SDL_RenderCopy(renderer, optionsText.texture, NULL, &optionsRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    scoreRect.x = (GAME_WIDTH-scoreText.w)/2;
    scoreRect.y = start_x+(interval*2);
    scoreRect.w = scoreText.w;
    scoreRect.h = scoreText.h;

    if(get_game()->title.selection == scoreText.id){
        return_code = SDL_RenderCopy(renderer, scoreText.selected_texture, NULL, &scoreRect);
    }else {
        return_code = SDL_RenderCopy(renderer, scoreText.texture, NULL, &scoreRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    quitRect.x = (GAME_WIDTH-quitText.w)/2;
    quitRect.y = start_x+(interval*3);
    quitRect.w = quitText.w;
    quitRect.h = quitText.h;

    if(get_game()->title.selection == quitText.id){
        return_code = SDL_RenderCopy(renderer, quitText.selected_texture, NULL, &quitRect);
    }else {
        return_code = SDL_RenderCopy(renderer, quitText.texture, NULL, &quitRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}