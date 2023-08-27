#pragma once
#include <stdbool.h>

#include <SDL.h>

#include "font.h"

#define POSSIBLE_SCORE_TEXTS 10
#define POSSIBLE_SCORE_LENGTH 10

typedef struct {
    bool created;
    float timeCreated;
    float x;
    float y;
    float y_vel;
    float x_vel;
    float speed;
    //const char *text;
    Text text;
} ScoreText;

void create_score_text(SDL_Renderer *renderer, ScoreText temp);
void increase_score(int amount, ScoreText temp);
void tick_ui();
void draw_ui(SDL_Renderer *renderer);