#include <stdbool.h>

#include <SDL.h>

#include "ui.h"

ScoreText scoreTexts[POSSIBLE_SCORE_TEXTS];
int total_scoreTexts = 0;
unsigned int score = 0;

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

    int next_element = redistribute_score_text_array();
    scoreTexts[next_element] = temp;
    total_scoreTexts++;
}

void increase_score(int amount){
    score += amount;
}

void draw_ui(SDL_Renderer *renderer){

}