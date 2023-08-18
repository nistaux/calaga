#pragma once
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

typedef enum {
    TITLE_FONT,
    THIN_FONT,
    REGULAR_FONT
} FontType;

typedef struct {
    SDL_Texture *texture;
    SDL_Texture *selected_texture;
    int w;
    int h;
    int id;
} Text;

void draw_title_font_selections(SDL_Renderer *renderer);