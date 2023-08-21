#include <stdbool.h>

#include "window.h"
#include "audio.h"
#include "game.h"

int main(int argc, char **argv) {
    init_sdl();
    init_mixer();
    init_game();

    while (get_game()->running == true){
        tick();
    }

    end_sdl();
    
    return 0;
}