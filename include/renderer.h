#pragma once
#include <SDL.h>

void init_renderer(SDL_Window *window);
void clear_renderer();
void update_renderer();
void present_renderer();