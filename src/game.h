#pragma once
#include <stdbool.h>

typedef enum {
    TITLE,
    GAME,
    PAUSE,
    END
} State;

typedef struct {
    bool running;
    bool debug;
    State state;
} Game;

typedef struct {
    unsigned int prevTime;
    unsigned int currentTime;
    float deltaTime;
    float physicsTime;
    float physicsIter;
    float renderTime;
    float renderIter;
} Timer;

Game *get_game();
float get_timer_delta();
void init_game();
void set_game_state(State state);
void set_game_running(bool running);

void tick();