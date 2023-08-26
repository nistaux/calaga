#include <stdbool.h>
#include <stdlib.h>

#include "enemies.h"
#include "field.h"

bool test_created = false;
int counter = 0;
void tick_generator(){
    MoveType moveType;
    if(get_field_available() >= 12){
        moveType = (rand() % 2);
    }else{
        moveType = MOVE_TYPE_LEFTRIGHT;
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

    if(!test_created){
        create_enemy(ENEMY_TYPE_BEADER,  moveType, startLoc);
        counter++;
    }
    if(counter >= 4){
        test_created = true;
    }
}