#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "window.h"
#include "events.h"
#include "game.h"
#include "player.h"
#include "defs.h"

Game game;
Timer timer;
SDL_Event event;
Background background = {
    .b1_loc = 0.0f,
    .b2_loc = -800.0f,
    .vel = 3.00f
};

Game *get_game() {
    return &game;
}

Background *get_background() {
    return &background;
}

float get_timer_delta() {
    return timer.deltaTime;
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

void init_game() {
    // Setting Game Struct Init Settings
    Title titleTemp = {
        .active = true,
        .selection = 0,
        .state = TITLE_STATE_MAIN,
    };
    Play playTemp = {
        .active = false,
        .state = PLAY_STATE_ALIVE,
        .paused_selection = 0
    };
    Game gameTemp = {
        .running = true,
        .state = GAME_STATE_TITLE,
        .title = titleTemp,
        .play = playTemp
    };
    game = gameTemp;

    // Setting Timer Struct Init Settings
    timer.prevTime = 0;
    timer.currentTime = 0;
    timer.deltaTime = 0.0f;
    timer.physicsTime = 0.0f;
    timer.renderTime = 0.0f;

    float physics_tick_rate_per_second = 60.0;
    float render_frame_rate_per_second = 100.0;
    timer.physicsIter = (1.0/physics_tick_rate_per_second);
    timer.renderIter = (1.0/render_frame_rate_per_second);
}

void down_title_selection() {
    int sel = game.title.selection;
    if (sel < 3){
        sel ++;
    }else {
        sel = 0;
    }
    game.title.selection = sel;
}
void up_title_selection() {
    int sel = game.title.selection;
    if (sel > 0){
        sel --;
    }else {
        sel = 3;
    }
    game.title.selection = sel;
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
    timer.prevTime = timer.currentTime;
    timer.currentTime = SDL_GetTicks();
    timer.deltaTime = (timer.currentTime-timer.prevTime)/1000.0f;
    timer.physicsTime += timer.deltaTime;
    timer.renderTime += timer.deltaTime;

    if (timer.physicsIter <= timer.physicsTime) {
        timer.physicsTime = 0.0;

        // Check control input
        check_events(event);
        move_background();
        move_player();
        
    }
    if (timer.renderIter <= timer.renderTime) {
        timer.renderTime = 0.0;
        // render updates
        update_window();
    }
}