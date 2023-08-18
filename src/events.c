#include <stdbool.h>

#include <SDL.h>

#include "game.h"
#include "audio.h"

void handle_play_keypress(SDL_Event event) {
    int key = event.key.keysym.sym;
}

void handle_title_keypress(SDL_Event event) {
    int key = event.key.keysym.sym;
    switch(key) {
        case SDLK_ESCAPE:
            set_game_running(false);
            break;
        case SDLK_w:
            up_title_selection();
            play_sound(SELECT_SOUND);
            break;
        case SDLK_s:
            play_sound(SELECT_SOUND);
            down_title_selection();
            break;
        case SDLK_SPACE:
            play_sound(ENTER_SOUND);
            switch(get_game()->title.selection){
                case 0:
                    set_game_state(GAME_STATE_PLAY);
                    break;
                case 1:
                    set_game_state(GAME_STATE_TITLE);
                    break;
                case 2:
                    set_game_state(GAME_STATE_TITLE);
                    break;
                case 3:
                    set_game_running(false);;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void handle_keypress(SDL_Event event) {
    switch(get_game()->state){
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