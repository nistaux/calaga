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
    if(!test_created){
        create_enemy(ENEMY_TYPE_BEADER,  moveType);
        counter++;
    }
    if(counter >= 4){
        test_created = true;
    }
}