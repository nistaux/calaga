#include "player.h"

Player player;

void init_player(int x, int y){
    Player temp = {
        .x = x,
        .y = y
    };

    player = temp;
}

Player *get_player() {
    return &player;
}

void set_player_state(PlayerState state){
    player.state = state;
}