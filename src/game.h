#pragma once
#include <stdbool.h>

typedef struct {
    unsigned int prevTime;
    unsigned int currentTime;
    float deltaTime;
    float physicsTime;
    float physicsIter;
    float renderTime;
    float renderIter;
} Timer;

typedef struct {
    SDL_Texture *texture;
    float b1_loc;
    float b2_loc;
    float vel;
} Background;

typedef enum {
    TITLE_STATE_MAIN,
    TITLE_STATE_OPTIONS,
    TITLE_STATE_SCORES,
    TITLE_STATE_FADING
} TitleState;

typedef struct {
    bool active;
    int selection;
    TitleState state;
} Title;

typedef enum {
    PLAY_STATE_ALIVE,
    PLAY_STATE_PAUSED,
    PLAY_STATE_DEAD
} PlayState;

typedef struct {
    bool active;
    PlayState state;
    int paused_selection;
} Play;

typedef enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAY,
} GameState;

typedef struct {
    bool running;
    int score;
    GameState state;
    Title title;
    Play play;
} Game;

Game *get_game();
Background *get_background();
float get_timer_delta();
void init_game();
GameState get_game_state();
void set_game_state(GameState state);
void set_game_running(bool running);
void go_to_main_menu();
void title_main_selection_up();
void title_main_selection_down();
void play_paused_main_selection_up();
void play_paused_main_selection_down();

void move_background();
void tick();