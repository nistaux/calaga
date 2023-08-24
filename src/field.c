#include <stdbool.h>
#include <stdlib.h>

#include "enemies.h"

bool field[24] = {false, false, false, false, false, false, 
                  false, false, false, false, false, false,
                  false, false, false, false, false, false,
                  false, false, false, false, false, false
                };
int find_field_space(MoveType moveType){
    int rand_int = rand() % 23;
    if(rand_int % 2 != 0){rand_int++;}
    while(field[rand_int] == true){
        if(rand_int != 22){
            rand_int += 2;
        }else {
            rand_int = 0;
        }
    }
    return rand_int;
}

void take_field_space(int startLoc, MoveType moveType){
    switch(moveType){
    case MOVE_TYPE_LEFTRIGHT:
        field[startLoc] = true;
        field[startLoc+1] = true;
        break;
    case MOVE_TYPE_CIRCLE:
        field[startLoc] = true;
        field[startLoc+1] = true;
        field[startLoc+6] = true;
        field[startLoc+7] = true;
        break;
    default:
        break;
    }
}

void free_field_space(int startLoc, MoveType moveType){
    switch(moveType){
    case MOVE_TYPE_LEFTRIGHT:
        field[startLoc] = false;
        field[startLoc+1] = false;
        break;
    case MOVE_TYPE_CIRCLE:
        field[startLoc] = false;
        field[startLoc+1] = false;
        field[startLoc+6] = false;
        field[startLoc+7] = false;
        break;
    default:
        break;
    }
}