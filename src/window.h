#pragma once
#include <SDL2/SDL.h>

void init_sdl();
void end_sdl();

SDL_Window *get_window();

void update_window();