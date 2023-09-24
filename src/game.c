#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL.h>

#include "window.h"
#include "events.h"
#include "game.h"
#include "player.h"
#include "defs.h"
#include "audio.h"
#include "projectile.h"
#include "generator.h"
#include "enemies.h"
#include "ui.h"

Game game;
Timer timer;
SDL_Event event;

// variable for setting the fps
double render_frame_rate_per_second = 100.0;

double system_timer = 0.0;
double system_post_interval = 5.0;
int frame_count = 0;
int physTicks_count = 0;
Background background = {
    .b1_loc = 0.0f,
    .b2_loc = -800.0f,
    .vel = 0.72f
};

Game *get_game() {
    return &game;
}

Background *get_background() {
    return &background;
}

double get_tick_delta() {
    return timer.deltaTick;
}

GameState get_game_state() {
    return game.state;
}
void set_game_state(GameState state) {
    game.state = state;
    switch(state){
    case GAME_STATE_PLAY:
        game.play.active = true;
        game.title.active = false;
        break;
    case GAME_STATE_TITLE:
        game.play.active = false;
        game.play.active = false;
        break;
    default:
        break;
    }
}

void set_game_running(bool running) {
    game.running = running;
}

void go_to_main_menu(){
    reset_player();
    reset_difficulty();
    start_title_music();
    game.state = GAME_STATE_TITLE;
    game.title.state = TITLE_STATE_MAIN;
    game.title.selection = 0;
    game.play.state = PLAY_STATE_ALIVE;
    game.play.paused_selection = 0;
    game.play.over_selection = 0;
}

void init_game() {
    srand(time(NULL));
    init_player(0.0f,0.0f);
    // Setting Game Struct Init Settings
    Title titleTemp = {
        .active = true,
        .selection = 0,
        .state = TITLE_STATE_MAIN,
    };
    Play playTemp = {
        .active = false,
        .state = PLAY_STATE_ALIVE,
        .paused_selection = 0,
        .over_selection = 0,
    };
    Game gameTemp = {
        .running = true,
        .state = GAME_STATE_TITLE,
        .title = titleTemp,
        .play = playTemp,
        .score = 0
    };
    game = gameTemp;

    // Setting Timer Struct Init Settings
    timer.prevTime = 0;
    timer.currentTime = SDL_GetTicks64();;
    timer.deltaTime = 0.0;
    timer.deltaTick = 0.0;
    timer.physicsTime = 0.0;
    timer.renderTime = 0.0;

    // variable for setting game tick rate
    float physics_tick_rate_per_second = 250.0;
    
    timer.physicsIter = (1.0/physics_tick_rate_per_second);
    timer.renderIter = (1.0/render_frame_rate_per_second);

    start_title_music();
}

void title_main_selection_down() {
    int sel = game.title.selection;
    if (sel < 3){
        sel ++;
    }else {
        sel = 0;
    }
    game.title.selection = sel;
}
void title_main_selection_up() {
    int sel = game.title.selection;
    if (sel > 0){
        sel --;
    }else {
        sel = 3;
    }
    game.title.selection = sel;
}
void play_paused_main_selection_down() {
    int sel = game.play.paused_selection;
    if (sel < 2){
        sel ++;
    }else {
        sel = 0;
    }
    game.play.paused_selection = sel;
}
void play_paused_main_selection_up() {
    int sel = game.play.paused_selection;
    if (sel > 0){
        sel --;
    }else {
        sel = 2;
    }
    game.play.paused_selection = sel;
}
void play_over_main_selection_down() {
    int sel = game.play.over_selection;
    if (sel < 1){
        sel ++;
    }else {
        sel = 0;
    }
    game.play.over_selection = sel;
}
void play_over_main_selection_up() {
    int sel = game.play.over_selection;
    if (sel > 0){
        sel --;
    }else {
        sel = 1;
    }
    game.play.over_selection = sel;
}

void move_background(){
    if(background.b1_loc + background.vel >= GAME_HEIGHT){
        background.b1_loc += (-1600.0f + background.vel);
    }else {
        background.b1_loc += background.vel;
    }
    if(background.b2_loc + background.vel >= GAME_HEIGHT){
        background.b2_loc += (-1600.0f + background.vel);
    }else {
        background.b2_loc += background.vel;
    }
}

void tick(){
    // Getting information on current processing speed
    // and using that to set FPS and Processing rate
    double miliseconds_in_second = 1000.0;
    timer.prevTime = timer.currentTime;
    timer.currentTime = SDL_GetTicks64();
    timer.deltaTime = (timer.currentTime-timer.prevTime)/miliseconds_in_second;
    timer.physicsTime += timer.deltaTime;
    timer.renderTime += timer.deltaTime;
    timer.deltaTick += timer.deltaTime;
    //printf("%d\n", game.play.state);

    system_timer += timer.deltaTime;
    

    if (timer.physicsTime >= timer.physicsIter) {
        timer.physicsTime = timer.physicsTime - timer.physicsIter;
        physTicks_count++;

        // Check control input
        check_events(event);
        bool paused = false;
        if(game.play.state == PLAY_STATE_PAUSED){paused = true;}
        if(game.play.state == PLAY_STATE_OVER){paused = true;}
        if(!paused){
            move_background();
            tick_player(timer.deltaTime);
            tick_enemies();
            tick_ui();
        }
        if(game.state == GAME_STATE_PLAY && game.play.state == PLAY_STATE_ALIVE){
            tick_generator();
            move_projectiles();
        }
        timer.deltaTick = 0.0;
    }
    if (timer.renderIter <= timer.renderTime) {
        timer.renderTime = timer.renderTime - timer.renderIter;
        frame_count++;

        // render updates
        update_window();
    }
    if(system_timer >= system_post_interval){
        printf("\n----------\n");
        printf("FPS_TARGET: %.1f\n", 1.0f/timer.renderIter);
        printf("FPS_CURRENT: %.1f\n\n", frame_count/system_post_interval);
        printf("TPS_TARGET: %.1f\n", 1.0f/timer.physicsIter);
        printf("TPS_CURRENT: %.1f\n", physTicks_count/system_post_interval);
        system_timer = system_timer - system_post_interval;
        frame_count = 0;
        physTicks_count = 0;
    }
}