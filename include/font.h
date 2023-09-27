#pragma once
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#define TITLE_FONT_DIR "./assets/fonts/Handjet-Light.ttf"
#define THIN_FONT_DIR "./assets/fonts/Handjet-Thin.ttf"
#define REGULAR_FONT_DIR "./assets/fonts/Handjet-Thin.ttf"

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

void create_text(SDL_Renderer *renderer, FontType fontType, int size, const char *string, Text *text);
void free_scores();
void draw_title_scores_selections(SDL_Renderer *renderer);
void draw_title_main_selections(SDL_Renderer *renderer);
void destroy_game_over_score();
void draw_play_over_main_selections(SDL_Renderer *renderer);
void draw_play_paused_main_selections(SDL_Renderer *renderer);