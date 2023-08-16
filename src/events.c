#include <stdbool.h>

#include <SDL.h>

#include "game.h"

void check_events(SDL_Event event) {
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) {
            set_game_running(false);
        }
        if(event.type == SDL_KEYDOWN) {
            //handle_keypress(event);
        }
    }
}