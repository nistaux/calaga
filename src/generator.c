#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "enemies.h"
#include "field.h"
#include "defs.h"

const int CD_ENEMY_CREATION = 2000;
const int CD_ASTEROID_CREATION = 1000;
const int CD_ASTEROID_CREATION_ATTEMPT = 1000;
const int SPAWN_RATE = 20;
const int ASTEROID_RATE = 5;

int aster_creation_cd = CD_ASTEROID_CREATION;
int aster_attempt_cd = CD_ASTEROID_CREATION_ATTEMPT;
int enemy_creation_cd = CD_ENEMY_CREATION;
int enemy_spawn_rate = SPAWN_RATE;
int asteroid_spawn_rate = ASTEROID_RATE;

float difficulty_scale = 1.0f;
int game_start_time = 0;
int last_aster_time = 0;
int last_attempt_time = 0;
int last_gen_time = 0;

void gen_asteroid(){
    if(get_asteroid_amount() >= 3){return;}
    if(SDL_GetTicks64()-last_aster_time < aster_creation_cd && last_aster_time != 0){return;}
    if(SDL_GetTicks64()-last_attempt_time < aster_attempt_cd && last_attempt_time != 0){return;}

    if((rand() % 100) > asteroid_spawn_rate){
        last_attempt_time = SDL_GetTicks64();
        return;
    }
    int startLocation = (rand() % (GAME_WIDTH+1))-35;
    float speedScale = (float)((rand() % 10)+1);
    float speed = (speedScale*0.08f)+1.0f;

    last_aster_time = SDL_GetTicks64();
    create_asteroid(startLocation, speed);
}

void gen_enemy(){
    if(SDL_GetTicks64()-last_gen_time < enemy_creation_cd && last_gen_time != 0){return;}
    if(get_field_available() == 0){return;}
    //printf("\nEnemy Spawn Timer: %d\nEnemy Spawn Rate: %d\nDIFFICULTY: %.1f\n", enemy_creation_cd, enemy_spawn_rate, difficulty_scale);

    MoveType moveType;
    if(get_field_available() >= 12){
        moveType = (rand() % 2);
    }else{
        moveType = MOVE_TYPE_LEFTRIGHT;
    }
    int randomEnemyNumber = (rand() % 100)+1;
    bool beader_spawn = randomEnemyNumber <= (100-enemy_spawn_rate);
    bool dagger_spawn = randomEnemyNumber > (100-enemy_spawn_rate) && randomEnemyNumber <= (100-(enemy_spawn_rate/2));
    bool tshot_spawn = randomEnemyNumber > (100-(enemy_spawn_rate/2));
    EnemyType enemyType;
    if(beader_spawn){
        enemyType = ENEMY_TYPE_BEADER;
    }else if(dagger_spawn){
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

float get_difficulty_scale(){
    return difficulty_scale;
}

void reset_difficulty(){
    game_start_time = 0;
    difficulty_scale = 1.0f;
    enemy_creation_cd = CD_ENEMY_CREATION;
    enemy_spawn_rate = SPAWN_RATE;
    asteroid_spawn_rate = ASTEROID_RATE;
}

void increase_difficulty(){
    float time = (float)SDL_GetTicks64() - game_start_time;

    enemy_creation_cd = CD_ENEMY_CREATION - round(time*0.01f);
    if(enemy_creation_cd < 500){
        enemy_creation_cd = 500;
    }

    enemy_spawn_rate = SPAWN_RATE + (round(time*0.0005f));
    if(enemy_spawn_rate > 80){
        enemy_spawn_rate = 80;
    }

    asteroid_spawn_rate = ASTEROID_RATE + (round(time*0.00025f));
    if(asteroid_spawn_rate > 50){
        asteroid_spawn_rate = 50;
    }
    float enemy_type_difficulty = (CD_ENEMY_CREATION-enemy_creation_cd)*0.01f;
    float enemy_spawn_difficulty = (enemy_spawn_rate - SPAWN_RATE)*0.1f;
    float asteroid_rate_difficulty = (asteroid_spawn_rate - ASTEROID_RATE)*0.1f;
    difficulty_scale = (enemy_type_difficulty+enemy_spawn_difficulty+asteroid_rate_difficulty)+1.0f;
    if(SDL_GetTicks64()-last_gen_time < enemy_creation_cd && last_gen_time != 0){return;}
    printf("\nEnemy Type Diff: %.1f\nEnemy Rate Diff: %.1f\nAsteroid Rate Diff: %.1f\nOVERALL DIFF: %.1f\n", enemy_type_difficulty, enemy_spawn_difficulty, asteroid_rate_difficulty, difficulty_scale);
}

void tick_generator(){
    if(game_start_time == 0){game_start_time = SDL_GetTicks64();}
    increase_difficulty();
    gen_enemy();
    gen_asteroid();
    
}