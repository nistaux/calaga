#include <stdbool.h>

#include "enemies.h"

bool test_created = false;
void tick_generator(){
    if(!test_created){
        create_enemy(ENEMY_TYPE_BEADER);
    }
}