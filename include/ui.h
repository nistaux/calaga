#pragma once
#include <stdbool.h>

#include <SDL.h>

#define POSSIBLE_SCORE_TEXTS 10
#define POSSIBLE_SCORE_LENGTH 10

typedef struct {
    bool created;
    float timeCreated;
    float x;
    float y;
    float speed;
    const char *text;
    SDL_Rect dstRect;
    SDL_Texture *texture;
} ScoreText;

void create_score_text(SDL_Renderer *renderer, ScoreText temp);
void increase_score(int amount);
void draw_ui(SDL_Renderer *renderer);