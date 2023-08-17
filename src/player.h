#pragma once

typedef enum {
    IDLE,
    SHOOTING,
    RECHARGING,
    DEAD
} PlayerState;

typedef struct {
    int x;
    int y;
    PlayerState state;
} Player;

void init_player(int x, int y);
Player *get_player();
void set_player_state(PlayerState state);