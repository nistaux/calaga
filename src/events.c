#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

#include "game.h"
#include "audio.h"
#include "player.h"
#include "projectile.h"
#include "window.h"
#include "defs.h"
#include "font.h"
#include "ui.h"

bool w_pressed = false;
bool a_pressed = false;
bool s_pressed = false;
bool d_pressed = false;
bool space_pressed = false;

void reset_keyboard_events(){
    w_pressed = false;
    a_pressed = false;
    s_pressed = false;
    d_pressed = false;
    space_pressed = false;
}


void handle_play_keydown_alive(int key){
    switch(key) {
        case SDLK_ESCAPE:
            set_music_volume_paused();
            reset_keyboard_events();
            reset_player_vel();
            get_game()->play.paused_selection = 0;
            get_game()->play.state = PLAY_STATE_PAUSED;
            break;
        case SDLK_a:
            if(!a_pressed){ 
                add_player_x_vel(-0.96);
                a_pressed = true;
            }
            break;
        case SDLK_d:
            if(!d_pressed){ 
                add_player_x_vel(0.96);
                d_pressed = true;
            }
            break;
        case SDLK_SPACE:
            if(!space_pressed){ 
                set_toggle_shoot_player(true);
                space_pressed = true;
            }
            break;
        default:
            break;
    }
}
void handle_play_keyup_alive(int key){
    switch(key) {
        case SDLK_a:
            if(a_pressed){
                add_player_x_vel(0.96);
                a_pressed = false;
            }
            
            break;
        case SDLK_d:
            if(d_pressed){
                add_player_x_vel(-0.96);
                d_pressed = false;
            }
            break;
        case SDLK_SPACE:
            if(space_pressed){ 
                set_toggle_shoot_player(false);
                space_pressed = false;
            }
            break;
        default:
            break;
    }
}

void handle_play_keypress_alive(SDL_Event event){
    int key = event.key.keysym.sym;
    switch(event.type){
        case SDL_KEYDOWN:
            handle_play_keydown_alive(key);
            break;
        case SDL_KEYUP:
            handle_play_keyup_alive(key);
            break;
        default:
            break;
    }

    
}

void handle_play_keydown_dead(int key){
    switch(key) {
        case SDLK_ESCAPE:
            set_music_volume_paused();
            get_game()->play.paused_selection = 0;
            get_game()->play.state = PLAY_STATE_PAUSED;
            break;
        case SDLK_a:
            if(!a_pressed){ 
                add_player_x_vel(-0.96);
                a_pressed = true;
            }
            break;
        case SDLK_d:
            if(!d_pressed){ 
                add_player_x_vel(0.96);
                d_pressed = true;
            }
            break;
        default:
            break;
    }
}
void handle_play_keyup_dead(int key){
    switch(key) {
        case SDLK_a:
            if(a_pressed){
                add_player_x_vel(0.96);
                a_pressed = false;
            }
            
            break;
        case SDLK_d:
            if(d_pressed){
                add_player_x_vel(-0.96);
                d_pressed = false;
            }
            break;
        default:
            break;
    }
}

void handle_play_keypress_dead(SDL_Event event){
    int key = event.key.keysym.sym;
    switch(event.type){
        case SDL_KEYDOWN:
            handle_play_keydown_dead(key);
            break;
        case SDL_KEYUP:
            handle_play_keyup_dead(key);
            break;
        default:
            break;
    }
}

void handle_play_keypress_paused(SDL_Event event){
    if(event.type != SDL_KEYDOWN){return;}
    int key = event.key.keysym.sym;
    switch(key) {
        case SDLK_ESCAPE:
            set_music_volume_play();
            get_game()->play.state = PLAY_STATE_ALIVE;
            break;
        case SDLK_w:
            play_paused_main_selection_up();
            play_sound(SELECT_SOUND);
            break;
        case SDLK_s:
            play_paused_main_selection_down();
            play_sound(SELECT_SOUND);
            break;
        case SDLK_SPACE:
            play_sound(ENTER_SOUND);
            switch(get_game()->play.paused_selection){
                case 0:
                    set_music_volume_play();
                    get_game()->play.state = PLAY_STATE_ALIVE;
                    break;
                case 1:
                    break;
                case 2:
                    destroy_game_over_score();
                    reset_score();
                    go_to_main_menu();
                    break;
                default:
                    printf("this shouldn't happen");
                    break;
            }
            break;
        default:
            break;
    }
}

void handle_play_keypress_over(SDL_Event event){
    if(event.type != SDL_KEYDOWN){return;}
    int key = event.key.keysym.sym;
    switch(key) {
        case SDLK_ESCAPE:
            // set_music_volume_play();
            // get_game()->play.state = PLAY_STATE_ALIVE;
            break;
        case SDLK_w:
            play_over_main_selection_up();
            play_sound(SELECT_SOUND);
            break;
        case SDLK_s:
            play_over_main_selection_down();
            play_sound(SELECT_SOUND);
            break;
        case SDLK_SPACE:
            play_sound(ENTER_SOUND);
            switch(get_game()->play.over_selection){
                case 0:
                    destroy_game_over_score();
                    reset_score();
                    start_play_music();
                    reset_player();
                    reset_keyboard_events();
                    float x = ((GAME_WIDTH/2) - (35));
                    float y = (GAME_HEIGHT-70)-55;
                    set_player_loc(x, y);
                    get_game()->play.state = PLAY_STATE_ALIVE;
                    break;
                case 1:
                    destroy_game_over_score();
                    reset_score();
                    go_to_main_menu();
                    break;
                default:
                    printf("this shouldn't happen");
                    break;
            }
            break;
        default:
            break;
    }
}

void handle_play_keypress(SDL_Event event) {
    switch(get_game()->play.state){
        case PLAY_STATE_ALIVE:
            handle_play_keypress_alive(event);
            break;
        case PLAY_STATE_DEAD:
            handle_play_keypress_dead(event);
            break;
        case PLAY_STATE_PAUSED:
            handle_play_keypress_paused(event);
            break;
        case PLAY_STATE_OVER:
            handle_play_keypress_over(event);
        default:
            break;
    }
}

void handle_title_keypress_main(SDL_Event event) {
    if(event.type != SDL_KEYDOWN){return;}
    int key = event.key.keysym.sym;
    switch(key) {
        case SDLK_ESCAPE:
            set_game_running(false);
            break;
        case SDLK_w:
            title_main_selection_up();
            play_sound(SELECT_SOUND);
            break;
        case SDLK_s:
            play_sound(SELECT_SOUND);
            title_main_selection_down();
            break;
        case SDLK_SPACE:
            play_sound(ENTER_SOUND);
            switch(get_game()->title.selection){
                case 0:
                    get_game()->title.state = TITLE_STATE_FADING;
                    get_game()->play.state = PLAY_STATE_ALIVE;
                    start_play_music();
                    break;
                case 1:
                    set_game_state(GAME_STATE_TITLE);
                    break;
                case 2:
                    set_game_state(GAME_STATE_TITLE);
                    break;
                case 3:
                    set_game_running(false);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
void handle_title_keypress_options(SDL_Event event) {
    
}
void handle_title_keypress_scores(SDL_Event event) {
    
}
void handle_title_keypress_fading(SDL_Event event) {
    
}
void handle_title_keypress(SDL_Event event) {
    switch(get_game()->title.state){
        case TITLE_STATE_MAIN:
            handle_title_keypress_main(event);
            break;
        default:
            break;
    }
}

void handle_keypress(SDL_Event event) {
    switch(get_game_state()){
        case GAME_STATE_PLAY:
            handle_play_keypress(event);
            break;
        case GAME_STATE_TITLE:
            handle_title_keypress(event);
            break;
        default:
            break;
    }
}

void check_events(SDL_Event event) {
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) {
            set_game_running(false);
        }
        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            handle_keypress(event);
        }
    }
}