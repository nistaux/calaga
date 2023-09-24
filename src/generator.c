#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "enemies.h"
#include "field.h"
#include "defs.h"

bool test_created = false;
int last_aster_time = 0;
int last_attempt_time = 0;
int aster_creation_cd = 1000;
int aster_attempt_cd = 1000;
int last_gen_time = 0;
int enemy_creation_cd = 1234;

void gen_asteroid(){
    if(get_asteroid_amount() >= 3){return;}
    if(SDL_GetTicks64()-last_aster_time < aster_creation_cd && last_aster_time != 0){return;}
    if(SDL_GetTicks64()-last_attempt_time < aster_attempt_cd && last_attempt_time != 0){return;}

    // 20% chance to create asteroid
    if((rand() % 100) > 20){
        last_attempt_time = SDL_GetTicks64();
        return;
    }
    int startLocation = (rand() % GAME_WIDTH);
    float speedScale = (float)((rand() % 10)+1);
    float speed = (speedScale*0.08f)+1.0f;

    last_aster_time = SDL_GetTicks64();
    create_asteroid(startLocation, speed);
}

void gen_enemy(){
    if(SDL_GetTicks64()-last_gen_time < enemy_creation_cd && last_gen_time != 0){return;}
    if(get_field_available() == 0){return;}

    MoveType moveType;
    if(get_field_available() >= 12){
        moveType = (rand() % 2);
    }else{
        moveType = MOVE_TYPE_LEFTRIGHT;
    }
    int randomEnemyNumber = (rand() % 100)+1;
    EnemyType enemyType;
    if(randomEnemyNumber <= 33){
        enemyType = ENEMY_TYPE_BEADER;
    }else if(randomEnemyNumber >= 34 && randomEnemyNumber <= 66){
        enemyType = ENEMY_TYPE_DAGGER;
    }else {
        enemyType = ENEMY_TYPE_TSHOT;
    }
    int startLoc;
    switch(moveType){
    case MOVE_TYPE_CIRCLE:
        startLoc = rand() % 4;
        break;
    case MOVE_TYPE_LEFTRIGHT:
        startLoc = rand() % 2;
        break;
    default:
        break;
    }
    create_enemy(enemyType,  moveType, startLoc);
    last_gen_time = SDL_GetTicks64();
}

void tick_generator(){
    gen_enemy();
    gen_asteroid();
}