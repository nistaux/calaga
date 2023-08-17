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
    int w;
    int h;
    bool selected;
} Text;

void create_text(SDL_Renderer *renderer, FontType fontType, int size, const char *string, Text *text);