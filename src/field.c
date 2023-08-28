#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "enemies.h"

bool field[24] = {false, false, false, false, false, false, 
                  false, false, false, false, false, false,
                  false, false, false, false, false, false,
                  false, false, false, false, false, false
                };
int field_available = 24;
int get_field_available(){
    return field_available;
}
void clear_field(){
    for(int i = 0; i < 24; i++){
        field[i] = false;
    }
    field_available = 24;
}
int find_field_space(MoveType moveType){
    int rand_int = rand() % 23;
    if(rand_int % 2 != 0){rand_int++;}
    if(moveType == MOVE_TYPE_LEFTRIGHT){
        while(field[rand_int] == true){
            if(rand_int != 22){
                rand_int += 2;
            }else {
                rand_int = 0;
            }
        }
    }else{
        bool top_bottom_available = false;
        while(!top_bottom_available){
            // printf("randint is %d\n", rand_int);
            if(rand_int >= 18){rand_int -= 18;}
            if(field[rand_int+6] == false && field[rand_int] == false){
                top_bottom_available = true;
            }else{
                rand_int += 2;
            }
        }
    }
    return rand_int;
}

void take_field_space(int startLoc, MoveType moveType){
    switch(moveType){
    case MOVE_TYPE_LEFTRIGHT:
        field[startLoc] = true;
        field[startLoc+1] = true;
        field_available -= 2;
        break;
    case MOVE_TYPE_CIRCLE:
        field[startLoc] = true;
        field[startLoc+1] = true;
        field[startLoc+6] = true;
        field[startLoc+7] = true;
        field_available -= 4;
        break;
    default:
        break;
    }
    // printf("Taking Field Space...\n");
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[0], field[1], field[2], field[3], field[4], field[5]);
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[6], field[7], field[8], field[9], field[10], field[11]);
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[12], field[13], field[14], field[15], field[16], field[17]);
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[18], field[19], field[20], field[21], field[22], field[23]);
}

void free_field_space(int startLoc, MoveType moveType){
    switch(moveType){
    case MOVE_TYPE_LEFTRIGHT:
        field[startLoc] = false;
        field[startLoc+1] = false;
        field_available += 2;
        break;
    case MOVE_TYPE_CIRCLE:
        field[startLoc] = false;
        field[startLoc+1] = false;
        field[startLoc+6] = false;
        field[startLoc+7] = false;
        field_available += 4;
        break;
    default:
        break;
    }
    // printf("Freeing Field Space...\n");
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[0], field[1], field[2], field[3], field[4], field[5]);
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[6], field[7], field[8], field[9], field[10], field[11]);
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[12], field[13], field[14], field[15], field[16], field[17]);
    // printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", field[18], field[19], field[20], field[21], field[22], field[23]);
}