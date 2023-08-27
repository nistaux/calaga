#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>

#include "ui.h"
#include "window.h"
#include "font.h"

ScoreText scoreTexts[POSSIBLE_SCORE_TEXTS];
int total_scoreTexts = 0;
unsigned int score = 0;
char scoreStr[POSSIBLE_SCORE_LENGTH];

int redistribute_score_text_array(){
    if(total_scoreTexts == 0){return 0;}
    int arr_count = 0;
    for(int i = 0; i < POSSIBLE_SCORE_TEXTS; i++){
        if(scoreTexts[i].created == true){
            //printf("Set index[%i] from prev index[%i]\n", arr_count, i);
            scoreTexts[arr_count] = scoreTexts[i];
            arr_count++;
        }
    }
    for(int i = 0; i < (POSSIBLE_SCORE_TEXTS-arr_count); i++) {
        ScoreText text = {
            .created = false,
        };
        //printf("Zeroing out index[%i]\n", arr_count+i);
        scoreTexts[arr_count+i] = text;
    }
    return arr_count;
}

void create_score_text(SDL_Renderer *renderer, ScoreText temp){
    temp.created = true;
    temp.timeCreated = SDL_GetTicks64();
    Text text;
    temp.text = text;
    bool dir = rand() % 2;
    if(dir){
        temp.x_vel = ((float)(rand() % 20))*0.18f;
    }else {
        temp.x_vel = ((float)(rand() % 20))*-0.18f;
    }
    temp.y_vel = -7.0*0.5;
    const char *p = scoreStr;
    create_text(renderer, REGULAR_FONT, 25, p, &temp.text);

    int next_element = redistribute_score_text_array();
    scoreTexts[next_element] = temp;
    total_scoreTexts++;
}

void delete_score_text(int index){
    ScoreText zero = {
        .created = false,
    };
    SDL_DestroyTexture(scoreTexts[index].text.texture);
    SDL_DestroyTexture(scoreTexts[index].text.selected_texture);
    scoreTexts[index] = zero;
    total_scoreTexts--;
}

void increase_score(int amount, ScoreText temp){
    score += amount;
    sprintf(scoreStr, "%d", score);
    create_score_text(SDL_GetRenderer(get_window()), temp);
}

void check_score_text(int index){
    if(SDL_GetTicks64() - scoreTexts[index].timeCreated >=1000){
        delete_score_text(index);
    }
}
void move_score_text(int index){
    scoreTexts[index].x += scoreTexts[index].x_vel*0.15;
    scoreTexts[index].y += scoreTexts[index].y_vel*0.2;
    scoreTexts[index].y_vel += 0.04f;
}
void tick_ui(){
    for(int scoreText = 0; scoreText < POSSIBLE_SCORE_TEXTS; scoreText++){
        if(scoreTexts[scoreText].created == true){
            check_score_text(scoreText);
            move_score_text(scoreText);
        }
    }
    
}

void draw_ui(SDL_Renderer *renderer){
    for(int scoreText = 0; scoreText < POSSIBLE_SCORE_TEXTS; scoreText++){
        if(scoreTexts[scoreText].created == true){
            SDL_Rect dstRect = {
                .x = (int)round(scoreTexts[scoreText].x),
                .y = (int)round(scoreTexts[scoreText].y),
                .w = scoreTexts[scoreText].text.w,
                .h = scoreTexts[scoreText].text.h
            };
            if(SDL_RenderCopy(renderer, scoreTexts[scoreText].text.texture, NULL, &dstRect) != 0){
                printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
            }
        }
    }
}