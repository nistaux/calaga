#pragma once
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

typedef enum {
    MENU_NAME_FONT,
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

void draw_title_main_selections(SDL_Renderer *renderer);
void draw_play_paused_main_selections(SDL_Renderer *renderer);