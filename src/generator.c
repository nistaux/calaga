#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "enemies.h"
#include "field.h"

bool test_created = false;
int last_gen_time = 0;
int enemy_creation_cd = 1234;
void tick_generator(){
    if(SDL_GetTicks64()-last_gen_time < enemy_creation_cd && last_gen_time != 0){return;}
    if(get_field_available() == 0){return;}

    MoveType moveType;
    if(get_field_available() >= 12){
        moveType = (rand() % 2);
    }else{
        moveType = MOVE_TYPE_LEFTRIGHT;
    }
    int randomEnemyNumber = (rand() % 100);
    EnemyType enemyType;
    if(randomEnemyNumber <= 49){
        enemyType = ENEMY_TYPE_BEADER;
    }else{
        enemyType = ENEMY_TYPE_DAGGER;
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