#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "font.h"

#define TITLE_FONT_DIR "./res/fonts/Handjet-Light.ttf"
#define THIN_FONT_DIR "./res/fonts/Handjet-Thin.ttf"
#define REGULAR_FONT_DIR "./res/fonts/Handjet-Thin.ttf"

bool is_ttf_init = false;

void create_text(SDL_Renderer *renderer, FontType fontType, int size, const char *string, Text *text){
    if(!is_ttf_init){
        printf("TTF Initialized");
        TTF_Init();
        is_ttf_init = true;
    }
    SDL_Color selectedColor = { 180,75,255 };
    SDL_Color notSelectedColor = { 180,255,75 };

    

    //pass address to text struct already made and then just add stuff to it
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

    SDL_Surface *textSurface;
    if(text->selected){
        textSurface = TTF_RenderText_Blended(font, string, selectedColor);
    }else {
        textSurface = TTF_RenderText_Blended(font, string, notSelectedColor);
    }
    
    if(!textSurface) {
        printf("Unable to render text surface!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(!texture) {
        printf("Unable to create texture from rendered text!\n"
                "SDL2 Error: %s\n", SDL_GetError());
        return;
    }

    Text ret = {
        .texture = texture,
        .w = textSurface->w,
        .h = textSurface->h
    };

    *text = ret;
}