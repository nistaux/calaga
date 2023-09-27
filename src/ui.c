#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>

#include "ui.h"
#include "window.h"
#include "font.h"
#include "player.h"
#include "defs.h"
#include "generator.h"

ScoreText scoreTexts[POSSIBLE_SCORE_TEXTS];
Text totalScoreText;
Text diffScaleText;
int ui_timer_last = 0;
int total_scoreTexts = 0;
unsigned int score = 0;
char scoreStr[POSSIBLE_SCORE_LENGTH] = "0";
const char *pScoreStr = scoreStr;
char totalScoreStr[POSSIBLE_SCORE_LENGTH] = "0";
const char *pTotalScoreStr = totalScoreStr;
char diffScaleStr[6] = "1.0";
const char *pDiffScaleStr = diffScaleStr;

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

const char* get_p_score_string(){
    return pTotalScoreStr;
}

unsigned int get_score(){
    return score;
}

void reset_score(){
    score = 0;
    sprintf(totalScoreStr, "%d", score);
    update_total_score_texture();
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
    create_text(renderer, REGULAR_FONT, 25, pScoreStr, &temp.text);

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
    sprintf(scoreStr, "%d", amount);
    sprintf(totalScoreStr, "%d", score);
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

void draw_player_hp(SDL_Renderer *renderer){
    int player_hp_size = 35;
    int side_border = 15;
    SDL_Rect dstRect = {
        .y = GAME_HEIGHT-player_hp_size-side_border,
        .h = player_hp_size,
        .w = player_hp_size
    };
    for(int i = 0; i < get_player_hp(); i++){
        dstRect.x = side_border + (player_hp_size*i);
        if(i != 0){dstRect.x+=2;}
        if(SDL_RenderCopy(renderer, get_player_texture(), NULL, &dstRect) != 0){
            printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
        }
    }
}

void update_total_score_texture(){
    SDL_Renderer *renderer = SDL_GetRenderer(get_window());
    
    SDL_DestroyTexture(totalScoreText.texture);
    SDL_DestroyTexture(totalScoreText.selected_texture);
    create_text(renderer, REGULAR_FONT, 27, pTotalScoreStr, &totalScoreText);
}

void update_diff_scale_texture(){
    SDL_Renderer *renderer = SDL_GetRenderer(get_window());

    sprintf(diffScaleStr, "%.1f", get_difficulty_scale());
    SDL_DestroyTexture(diffScaleText.texture);
    SDL_DestroyTexture(diffScaleText.selected_texture);
    create_text(renderer, TITLE_FONT, 27, pDiffScaleStr, &diffScaleText);
}

void draw_total_score(SDL_Renderer *renderer){
    int ret = 0;
    SDL_Rect src = {
        .x = 0,
        .y = 0,
        .w = totalScoreText.w,
        .h = totalScoreText.h,
    };
    SDL_Rect dst = {
        .x = (GAME_WIDTH - (totalScoreText.w+15)),
        .y = (GAME_HEIGHT - (totalScoreText.h+15)),
        .w = src.w,
        .h = src.h,
    };
    ret = SDL_RenderCopy(renderer, totalScoreText.texture, &src, &dst);
    if(ret != 0){printf("SDL: Error Rendering Image - %s\n", SDL_GetError());}
}

void draw_diff_scale(SDL_Renderer *renderer){
    int ret = 0;
    SDL_Rect src = {
        .x = 0,
        .y = 0,
        .w = diffScaleText.w,
        .h = diffScaleText.h,
    };
    SDL_Rect dst = {
        .x = (GAME_WIDTH/2) - (diffScaleText.w/2),
        .y = (GAME_HEIGHT - (diffScaleText.h+15)),
        .w = src.w,
        .h = src.h,
    };
    ret = SDL_RenderCopy(renderer, diffScaleText.texture, &src, &dst);
    if(ret != 0){printf("SDL: Error Rendering Image - %s\n", SDL_GetError());}
}

void draw_ui(SDL_Renderer *renderer){
    if(SDL_GetTicks64()-ui_timer_last > 100 || ui_timer_last == 0){
        ui_timer_last = SDL_GetTicks64();
        update_diff_scale_texture();
        update_total_score_texture();
    }
    draw_player_hp(renderer);
    draw_total_score(renderer);
    draw_diff_scale(renderer);
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