#include <stdio.h>
#include <stdlib.h>

#include "scores.h"

void sort_scores(Score *scores){
    int i, j, max_buffer;

    for(i = 0; i < 10; i++){
        max_buffer = i;
        for(j = i + 1; j < 11; j++){
            if(!scores[j].exists) continue;
            if(scores[j].score > scores[max_buffer].score) max_buffer = j;
        }
        Score temp = scores[i];
        scores[i] = scores[max_buffer];
        scores[max_buffer] = temp;
    }
}

void add_to_scores_exists(Score newScore, FILE *file){
    Score allScores[11];
    Score *pAllScores = allScores;
    for(int i = 0; i < 10; i++){
        fread(pAllScores+i, sizeof(Score), 1, file);
    }
    allScores[10] = newScore;
    sort_scores(pAllScores);
    
    Score retScores[10];
    Score *pRetScores = retScores;
    for(int i = 0; i < 10; i++) retScores[i] = allScores[i];

    rewind(file);

    for(int i = 0; i < 10; i++){
        fwrite(pRetScores+i, sizeof(Score), 1, file);
    }
    fclose(file);
}

void add_to_scores_null(Score newScore, FILE *rFile){
    fclose(rFile);
    FILE *wFile;
    wFile = fopen("scores.dat", "wb");
    Score nullScore = {
        .score = 0,
        .exists = false,
    };
    Score scores[10] = {nullScore};
    Score *p = scores;
    scores[0] = newScore;
    
    for(int i = 0; i < 10; i++){
        fwrite(p+i, sizeof(scores[0]), 1, wFile);
    }
    fclose(wFile);
}

void add_to_scores(unsigned int scoreNum){
    time_t t = time(NULL);
    tm tm = *localtime(&t);
    Score score = {
        .exists = true,
        .date = tm,
        .score = scoreNum
    };
    FILE *file;
    file = fopen("scores.dat", "rb+");
    if(file == NULL){
        add_to_scores_null(score, file);
    }else{
        add_to_scores_exists(score, file);
    }
}

Score *get_scores(){
    Score nullScore = {
        .exists = false,
    };

    Score scores[10] = {nullScore};
    Score *p = malloc(sizeof(scores));
    for(int i = 0; i < 10; i++) p[i] = scores[i];

    FILE *file;
    file = fopen("scores.dat", "rb");
    if(file == NULL) return p; 
    for(int i = 0; i < 10; i++){
        fread(p+i, sizeof(Score), 1, file);
    }
    fclose(file);
    return p;
}