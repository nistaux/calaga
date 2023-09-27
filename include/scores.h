#pragma once

#include <time.h>
#include <stdbool.h>

typedef struct tm tm;

typedef struct Score {
    bool exists;
    tm date;
    unsigned int score;
} Score;

void init_scores();
void add_to_scores(unsigned int score);
Score *get_scores();