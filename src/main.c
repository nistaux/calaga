#include <stdbool.h>
#include "game.h"
#include "window.h"

int main(int argc, char **argv) {
    init_sdl();
    init_game();

    while (get_game()->running == true){
        tick();
    }

    end_sdl();
    
}