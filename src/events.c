#include <stdbool.h>
#include <stdio.h>

#include <SDL.h>

#include "game.h"
#include "audio.h"
#include "player.h"

void handle_play_keypress_alive(SDL_Event event){
    int key = event.key.keysym.sym;
    switch(key) {
        case SDLK_ESCAPE:
            set_music_volume_paused();
            get_game()->play.state = PLAY_STATE_PAUSED;
            break;
        default:
            break;
    }
}
void handle_play_keypress_paused(SDL_Event event){
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
            //asd
            break;
        case PLAY_STATE_PAUSED:
            handle_play_keypress_paused(event);
            break;
        default:
            break;
    }
}

void handle_title_keypress_main(SDL_Event event) {
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
        if(event.type == SDL_KEYDOWN) {
            handle_keypress(event);
        }
    }
}