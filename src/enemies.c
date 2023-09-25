#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#include "enemies.h"
#include "field.h"
#include "player.h"
#include "projectile.h"
#include "ui.h"
#include "defs.h"
#include "audio.h"
#include "generator.h"

SDL_Texture *enemiesTexture;
int total_enemies = 0;
int total_asteroids = 0;
Enemy enemies[POSSIBLE_ENEMIES];
bool enemyTexturesLoaded = false;

void init_enemies(SDL_Renderer *renderer){
    // create sprite sheet
    enemiesTexture = IMG_LoadTexture(renderer, ENEMY_SPRITE_DIR);
    enemyTexturesLoaded = true;
}

Enemy *get_enemies(){
    Enemy *p;
    p = enemies;
    return p;
}

int redistribute_enemy_array(){
    if(total_enemies+total_asteroids == 0){return 0;}
    int arr_count = 0;
    for(int i = 0; i < POSSIBLE_ENEMIES; i++){
        if(enemies[i].created == true){
            //printf("Set index[%i] from prev index[%i]\n", arr_count, i);
            enemies[arr_count] = enemies[i];
            arr_count++;
        }
    }
    for(int i = 0; i < (POSSIBLE_ENEMIES-arr_count); i++) {
        Enemy proj = {
            .created = false,
        };
        //printf("Zeroing out index[%i]\n", arr_count+i);
        enemies[arr_count+i] = proj;
    }
    return arr_count;
}

int get_asteroid_amount(){
    return total_asteroids;
}

void create_asteroid(int startLocation, float speed){
    SDL_Rect spriteRect = { .x = 0, .y = 70, .w = 70, .h = 70};
    Enemy enemy = {
        .created = true,
        .spriteRect = spriteRect,
        .state = ENEMY_STATE_RECHARGING,
        .type = ENEMY_TYPE_ASTEROID,
        .x = (float)startLocation,
        .x_vel = 0.0f,
        .y = -70.0f,
        .y_vel = 1.0f,
        .speed = speed
    };

    int next_element = redistribute_enemy_array();
    enemies[next_element] = enemy;
    total_asteroids++;
}

void create_beader(MoveType moveType, int startLocation){
    int fieldLoc = find_field_space(moveType);
    take_field_space(fieldLoc, moveType);
    
    int top_buffer = 50;
    int field_border = 15;
    int x = 0;
    int y = 0;

    if(fieldLoc < 6){
        x = (100*fieldLoc)+field_border;
        y = (top_buffer+field_border);
    }else if(6 <= fieldLoc && fieldLoc < 12){
        x = (100*(fieldLoc-6)+field_border);
        y = (top_buffer+field_border+100);
    }else if(12 <= fieldLoc && fieldLoc < 18){
        x = (100*(fieldLoc-12)+field_border);
        y = (top_buffer+field_border+200);
    }else {
        x = (100*(fieldLoc-18)+field_border);
        y = (top_buffer+field_border+300);
    }
    
    SDL_Rect spriteRect = { .x = 0, .y = 0, .w = 70, .h = 70};
    Enemy enemy = {
        .created = true,
        .hp = 1,
        .last_shot = 0.0f,
        .reloading = false,
        .shoot_reload_interval_seconds = 3.0,
        .spriteRect = spriteRect,
        .state = ENEMY_STATE_RECHARGING,
        .type = ENEMY_TYPE_BEADER,
        .spawn_time = SDL_GetTicks64(),
        .moveType = moveType,
        .fieldReservation = fieldLoc,
        .fieldLocation = startLocation,
        .x_loc = (float)x,
        .y_loc = (float)y,
        .x = (float)x,
        .x_vel = 0.0f,
        .y = (float)y,
        .y_vel = 0.0f,
        .speed = 0.20f
    };
    enemy.reloading = true;
    enemy.last_shot = ((float)SDL_GetTicks64()/1000.0f)-(enemy.shoot_reload_interval_seconds*0.75);

    switch(enemy.fieldLocation){
    case 1:
        enemy.x += + 100.0f;
        if(enemy.moveType == MOVE_TYPE_LEFTRIGHT){
            enemy.x_vel = -1.0f;
        }else{
            enemy.y_vel = 1.0f;
        }
        break;
    case 2:
        enemy.y += 100.0f;
        enemy.y_vel = -1.0f;
        break;
    case 3:
        enemy.x += 100.0f;
        enemy.y += 100.0f;
        enemy.x_vel = -1.0f;
        break;
    default:
        enemy.x_vel = 1.0f;
        break;
    }

    int next_element = redistribute_enemy_array();
    enemies[next_element] = enemy;
    total_enemies++;
}

void create_tshot(MoveType moveType, int startLocation){
    int fieldLoc = find_field_space(moveType);
    take_field_space(fieldLoc, moveType);
    
    int top_buffer = 50;
    int field_border = 15;
    int x = 0;
    int y = 0;

    if(fieldLoc < 6){
        x = (100*fieldLoc)+field_border;
        y = (top_buffer+field_border);
    }else if(6 <= fieldLoc && fieldLoc < 12){
        x = (100*(fieldLoc-6)+field_border);
        y = (top_buffer+field_border+100);
    }else if(12 <= fieldLoc && fieldLoc < 18){
        x = (100*(fieldLoc-12)+field_border);
        y = (top_buffer+field_border+200);
    }else {
        x = (100*(fieldLoc-18)+field_border);
        y = (top_buffer+field_border+300);
    }
    
    SDL_Rect spriteRect = { .x = 140, .y = 0, .w = 70, .h = 70};
    Enemy enemy = {
        .created = true,
        .hp = 1,
        .last_shot = 0.0f,
        .reloading = false,
        .shoot_reload_interval_seconds = 0.666f,
        .spriteRect = spriteRect,
        .state = ENEMY_STATE_RECHARGING,
        .type = ENEMY_TYPE_TSHOT,
        .moveType = moveType,
        .spawn_time = SDL_GetTicks64(),
        .fieldReservation = fieldLoc,
        .fieldLocation = startLocation,
        .x_loc = (float)x,
        .y_loc = (float)y,
        .x = (float)x,
        .x_vel = 0.0f,
        .y = (float)y,
        .y_vel = 0.0f,
        .speed = 0.20f
    };
    enemy.reloading = true;
    enemy.last_shot = ((float)SDL_GetTicks64()/1000.0f)-(enemy.shoot_reload_interval_seconds*0.75);

    switch(enemy.fieldLocation){
    case 1:
        enemy.x += + 100.0f;
        if(enemy.moveType == MOVE_TYPE_LEFTRIGHT){
            enemy.x_vel = -1.0f;
        }else{
            enemy.y_vel = 1.0f;
        }
        break;
    case 2:
        enemy.y += 100.0f;
        enemy.y_vel = -1.0f;
        break;
    case 3:
        enemy.x += 100.0f;
        enemy.y += 100.0f;
        enemy.x_vel = -1.0f;
        break;
    default:
        enemy.x_vel = 1.0f;
        break;
    }

    int next_element = redistribute_enemy_array();
    enemies[next_element] = enemy;
    total_enemies++;
}

void create_dagger(MoveType moveType, int startLocation){
    int fieldLoc = find_field_space(moveType);
    take_field_space(fieldLoc, moveType);
    
    int top_buffer = 50;
    int field_border = 15;
    int x = 0;
    int y = 0;

    if(fieldLoc < 6){
        x = (100*fieldLoc)+field_border;
        y = (top_buffer+field_border);
    }else if(6 <= fieldLoc && fieldLoc < 12){
        x = (100*(fieldLoc-6)+field_border);
        y = (top_buffer+field_border+100);
    }else if(12 <= fieldLoc && fieldLoc < 18){
        x = (100*(fieldLoc-12)+field_border);
        y = (top_buffer+field_border+200);
    }else {
        x = (100*(fieldLoc-18)+field_border);
        y = (top_buffer+field_border+300);
    }
    
    SDL_Rect spriteRect = { .x = 70, .y = 0, .w = 70, .h = 70};
    Enemy enemy = {
        .created = true,
        .hp = 1,
        .last_shot = 0.0f,
        .reloading = false,
        .shoot_reload_interval_seconds = 1.0f,
        .spriteRect = spriteRect,
        .type = ENEMY_TYPE_DAGGER,
        .state = ENEMY_STATE_RECHARGING,
        .moveType = moveType,
        .fieldReservation = fieldLoc,
        .spawn_time = SDL_GetTicks64(),
        .fieldLocation = startLocation,
        .x_loc = (float)x,
        .y_loc = (float)y,
        .x = (float)x,
        .x_vel = 0.0f,
        .y = (float)y,
        .y_vel = 0.0f,
        .speed = 0.20f
    };
    enemy.reloading = true;
    enemy.last_shot = ((float)SDL_GetTicks64()/1000.0f);

    switch(enemy.fieldLocation){
    case 1:
        enemy.x += + 100.0f;
        if(enemy.moveType == MOVE_TYPE_LEFTRIGHT){
            enemy.x_vel = -1.0f;
        }else{
            enemy.y_vel = 1.0f;
        }
        break;
    case 2:
        enemy.y += 100.0f;
        enemy.y_vel = -1.0f;
        break;
    case 3:
        enemy.x += 100.0f;
        enemy.y += 100.0f;
        enemy.x_vel = -1.0f;
        break;
    default:
        enemy.x_vel = 1.0f;
        break;
    }

    int next_element = redistribute_enemy_array();
    enemies[next_element] = enemy;
    total_enemies++;
}

void create_enemy(EnemyType type, MoveType moveType, int startLocation){
    switch (type){
    case ENEMY_TYPE_BEADER:
        create_beader(moveType, startLocation);
        break;
    case ENEMY_TYPE_DAGGER:
        create_dagger(moveType, startLocation);
        break;
    case ENEMY_TYPE_TSHOT:
        create_tshot(moveType, startLocation);
        break;
    default:
        break;
    }
}
void clear_enemies(){
    for(int i = 0; i < (POSSIBLE_ENEMIES); i++) {
        Enemy enemy = {
            .created = false,
        };
        enemies[i] = enemy;
    }
    total_enemies = 0;
    total_asteroids = 0;
    clear_field();
}

void destroy_enemy(int index){
    int fieldSpace = enemies[index].fieldReservation;
    MoveType type = enemies[index].moveType;
    switch(enemies[index].type){
    case ENEMY_TYPE_ASTEROID:
        total_asteroids--;
        break;
    default:
        free_field_space(fieldSpace, type);
        total_enemies--;
    }
    Enemy zero = {
        .created = false,
    };
    enemies[index] = zero;
    
}

void move_type_circle(int enemyIndex){
    if(enemies[enemyIndex].x == enemies[enemyIndex].x_loc && enemies[enemyIndex].y < enemies[enemyIndex].y_loc){
        enemies[enemyIndex].y = enemies[enemyIndex].y_loc;
        enemies[enemyIndex].y_vel = 0.0f;
        enemies[enemyIndex].x_vel = 1.0f;
    }
    if(enemies[enemyIndex].x == enemies[enemyIndex].x_loc+100.0f && enemies[enemyIndex].y > enemies[enemyIndex].y_loc+100.0f){
        enemies[enemyIndex].y = enemies[enemyIndex].y_loc+100.0f;
        enemies[enemyIndex].y_vel = 0.0f;
        enemies[enemyIndex].x_vel = -1.0f;
    }
    if(enemies[enemyIndex].y == enemies[enemyIndex].y_loc && enemies[enemyIndex].x > enemies[enemyIndex].x_loc+100.0f){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc+100.0f;
        enemies[enemyIndex].y_vel = 1.0f;
        enemies[enemyIndex].x_vel = 0.0f;
    }
    if(enemies[enemyIndex].y == enemies[enemyIndex].y_loc+100.0f && enemies[enemyIndex].x < enemies[enemyIndex].x_loc){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc;
        enemies[enemyIndex].y_vel = -1.0f;
        enemies[enemyIndex].x_vel = 0.0f;
    }
    // enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
    // enemies[enemyIndex].y += enemies[enemyIndex].y_vel*enemies[enemyIndex].speed;
}

void move_type_leftright(int enemyIndex){
    if(enemies[enemyIndex].x < enemies[enemyIndex].x_loc){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc;
        enemies[enemyIndex].x_vel = 1.0f;
    }
    if(enemies[enemyIndex].x > enemies[enemyIndex].x_loc+100.0f){
        enemies[enemyIndex].x = enemies[enemyIndex].x_loc+100.0f;
        enemies[enemyIndex].x_vel = -1.0f;
    }
    // enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
}

void turn_enemy(int enemyIndex){
    enemies[enemyIndex].target_slope_x = (get_player_x()+35.0f) - (enemies[enemyIndex].x+35.0f);
    enemies[enemyIndex].target_slope_y = (675.0f+35.0f) - (enemies[enemyIndex].y+35.0f);
    double angle = atan(enemies[enemyIndex].target_slope_x/enemies[enemyIndex].target_slope_y)*-50.0;

    enemies[enemyIndex].target_dir = angle;
}

void move_enemies(int enemyIndex){
    enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
    enemies[enemyIndex].y += enemies[enemyIndex].y_vel*enemies[enemyIndex].speed;
    if(enemies[enemyIndex].type == ENEMY_TYPE_ASTEROID){return;};
    if(enemies[enemyIndex].state != ENEMY_STATE_RECHARGING){return;}
    switch(enemies[enemyIndex].moveType){
    case MOVE_TYPE_CIRCLE:
        move_type_circle(enemyIndex);
        break;
    case MOVE_TYPE_LEFTRIGHT:
        move_type_leftright(enemyIndex);
        break;
    default:
        break;
    }
    if(enemies[enemyIndex].state == ENEMY_STATE_RECHARGING){turn_enemy(enemyIndex);}
}

void shoot_beader_and_tshot(int enemyIndex){
    float lastShotDiff = ((float)SDL_GetTicks64()/1000.0f) - enemies[enemyIndex].last_shot;
    if(lastShotDiff >= enemies[enemyIndex].shoot_reload_interval_seconds){enemies[enemyIndex].reloading = false;}
    if(enemies[enemyIndex].reloading){return;}

    SDL_Rect dstRect = {
        .x = 0,
        .y = 0,
        .w = 17,
        .h = 20
    };
    SDL_Rect srcRect = {
        .w = 17,
        .h = 20,
    };
    Projectile temp = {
        .speed = 1.5f,
        .dstRect = dstRect,
        .srcRect = srcRect,
        .angle = enemies[enemyIndex].target_dir,
        .x = enemies[enemyIndex].x+35.0f,
        .y = enemies[enemyIndex].y+35.0f,
        .x_vel = 0.0f,
        .y_vel = 0.0f,
    };
    switch(enemies[enemyIndex].type){
    case ENEMY_TYPE_BEADER:
        temp.srcRect.x = 17;
        temp.srcRect.y = 0;
        temp.speed = 1.5f;
        temp.type = PROJ_BEADER;
        break;
    case ENEMY_TYPE_TSHOT:
        temp.srcRect.x = 34;
        temp.srcRect.y = 0;
        temp.speed = 1.5f;
        temp.type = PROJ_TSHOT;
        break;
    default:
        break;
    }

    // temp.speed
    float total_movement = (float)(fabs(enemies[enemyIndex].target_slope_x) + fabs(enemies[enemyIndex].target_slope_y));
    temp.x_vel = temp.speed * (enemies[enemyIndex].target_slope_x/total_movement);
    temp.y_vel = temp.speed * (enemies[enemyIndex].target_slope_y/total_movement);
    temp.x += (enemies[enemyIndex].target_slope_x/enemies[enemyIndex].target_slope_y)*20.0f;
    temp.y += 20.0f;
    enemies[enemyIndex].last_shot = (float)SDL_GetTicks64()/1000.0f;
    enemies[enemyIndex].reloading = true;
    play_sound(ENEMY_SHOOT_SOUND);
    create_projectile(temp);
}

void shoot_dagger(int enemyIndex){
    if(enemies[enemyIndex].state == ENEMY_STATE_RECHARGING){
        float lastShotDiff = ((float)SDL_GetTicks64()/1000.0f) - enemies[enemyIndex].last_shot;
        if(lastShotDiff >= enemies[enemyIndex].shoot_reload_interval_seconds){enemies[enemyIndex].reloading = false;}
        if(enemies[enemyIndex].reloading){return;}
        //printf("made it past recharging\n");
    }
    
    // float total_movement = (float)(fabs(enemies[enemyIndex].target_slope_x) + fabs(enemies[enemyIndex].target_slope_y));
    // temp.x_vel = temp.speed * (enemies[enemyIndex].target_slope_x/total_movement);
    // temp.y_vel = temp.speed * (enemies[enemyIndex].target_slope_y/total_movement);

    if(!enemies[enemyIndex].reloading){
        enemies[enemyIndex].last_shot = (float)SDL_GetTicks64()/1000.0f;
        enemies[enemyIndex].reloading = true;
        enemies[enemyIndex].state = ENEMY_STATE_SHOOTING;
        play_sound(WINDUP_SOUND);
        //printf("made it past shooting\n");
    }

    float total_movement = (float)(fabs(enemies[enemyIndex].target_slope_x) + fabs(enemies[enemyIndex].target_slope_y));
    float lastShotDiff = ((float)SDL_GetTicks64()/1000.0f) - enemies[enemyIndex].last_shot;
    switch(enemies[enemyIndex].state){
    case ENEMY_STATE_SHOOTING:
        enemies[enemyIndex].x_vel = -5.0f * (enemies[enemyIndex].target_slope_x/total_movement);
        enemies[enemyIndex].y_vel = -5.0f * (enemies[enemyIndex].target_slope_y/total_movement);
        if(lastShotDiff >= 0.5f){
            enemies[enemyIndex].state = ENEMY_STATE_DEAD;
            play_sound(WOOSH_SOUND);
        }
        //printf("shooting...\n");
        break;
    case ENEMY_STATE_DEAD:
        enemies[enemyIndex].x_vel = 60.0f * (enemies[enemyIndex].target_slope_x/total_movement);
        enemies[enemyIndex].y_vel = 60.0f * (enemies[enemyIndex].target_slope_y/total_movement);
        //printf("GOGOGO\n");
        break;
    default:
        printf("ENEMY: Whats going on here..??..,??\n");
        break;
    }
}

void shoot_enemies(int enemyIndex){
    switch(enemies[enemyIndex].type){
    case ENEMY_TYPE_DAGGER:
        shoot_dagger(enemyIndex);
        break;
    case ENEMY_TYPE_ASTEROID:
        break;
    default:
        shoot_beader_and_tshot(enemyIndex);
    }
}

void kill_enemy(int enemyIndex, int projectileIndex){
    float difficulty = get_difficulty_scale();
    ScoreText temp = {
        .x = enemies[enemyIndex].x+15.0f,
        .y = enemies[enemyIndex].y+5.0f
    };
    switch(enemies[enemyIndex].type){
    case ENEMY_TYPE_BEADER:
        increase_score((int)round(difficulty*50), temp);
        break;
    case ENEMY_TYPE_DAGGER:
        stop_chargup_sound();
        increase_score((int)round(difficulty*125), temp);
        break;
    case ENEMY_TYPE_TSHOT:
        increase_score((int)round(difficulty*200), temp);
        break;
    default:
        increase_score((int)round(difficulty*25), temp);
        break;
    }
    destroy_projectile(projectileIndex);
    destroy_enemy(enemyIndex); 
}

void check_asteroids(int enemyIndex){
    Enemy enemy = enemies[enemyIndex];
    Projectile proj;
    Projectile *p = get_projectiles();
    for(int i = 0; i < possible_projectiles; i++){
        proj = *(p + i);

        if(proj.x < (enemy.x+enemy.spriteRect.w) && 
        proj.y < (enemy.y+enemy.spriteRect.h-15.0f) &&
        proj.x > enemy.x &&
        proj.y > enemy.y
        ){
            play_sound(HIT_SOUND);
            destroy_projectile(i);
        }else if(proj.x+proj.srcRect.w > enemy.x && 
        proj.y < (enemy.y+enemy.spriteRect.h-15.0f) &&
        proj.x < enemy.x+enemy.spriteRect.w &&
        proj.y > enemy.y
        ){
            play_sound(HIT_SOUND);
            destroy_projectile(i);
        }
    }
    // Check if enemy is outside bounds and then kill it if so - or just remove it
    bool ENEMY_OUTSIDE_LEFT_BOUND = (enemy.x + (float)enemy.spriteRect.w) < 0.0f;
    bool ENEMY_OUTSIDE_BOTTOM_BOUND = enemy.y > GAME_HEIGHT;
    bool ENEMY_OUTSIDE_RIGHT_BOUND = enemy.x > GAME_WIDTH;
    if(ENEMY_OUTSIDE_LEFT_BOUND || ENEMY_OUTSIDE_BOTTOM_BOUND || ENEMY_OUTSIDE_RIGHT_BOUND){
        //printf("ENEMY: Destroyed Asteroid. y: %.2f\n", enemies[enemyIndex].y);
        destroy_enemy(enemyIndex);
        
    }
}

void check_enemies(int enemyIndex){
    if(SDL_GetTicks64()-enemies[enemyIndex].spawn_time < 333){return;}
    Enemy enemy = enemies[enemyIndex];
    Projectile proj;
    Projectile *p = get_projectiles();
    for(int i = 0; i < possible_projectiles; i++){
        proj = *(p + i);
        if(proj.created == false || proj.type != PROJ_PLAYER){continue;}

        if(proj.x < (enemy.x+enemy.spriteRect.w) && 
        proj.y < (enemy.y+enemy.spriteRect.h-15.0f) &&
        proj.x > enemy.x &&
        proj.y > enemy.y
        ){
            //printf("killed enemy: %d\n", enemies[enemyIndex].type);
            play_sound(DEAD_SOUND);
            kill_enemy(enemyIndex, i);
            
        }else if(proj.x+proj.srcRect.w > enemy.x && 
        proj.y < (enemy.y+enemy.spriteRect.h-15.0f) &&
        proj.x < enemy.x+enemy.spriteRect.w &&
        proj.y > enemy.y
        ){
            //printf("killed enemy: %d\n", enemies[enemyIndex].type);
            play_sound(DEAD_SOUND);
            kill_enemy(enemyIndex, i);
        }
    }
    // Check if enemy is outside bounds and then kill it if so - or just remove it
    bool ENEMY_OUTSIDE_LEFT_BOUND = (enemy.x + (float)enemy.spriteRect.w) < 0.0f;
    bool ENEMY_OUTSIDE_BOTTOM_BOUND = enemy.y > GAME_HEIGHT;
    bool ENEMY_OUTSIDE_RIGHT_BOUND = enemy.x > GAME_WIDTH;
    if(ENEMY_OUTSIDE_LEFT_BOUND || ENEMY_OUTSIDE_BOTTOM_BOUND || ENEMY_OUTSIDE_RIGHT_BOUND){
        destroy_enemy(enemyIndex);
    }
}

void tick_enemies(){
    for(int enemy = 0; enemy < POSSIBLE_ENEMIES; enemy++){
        if(!enemies[enemy].created){continue;}
        move_enemies(enemy);
        shoot_enemies(enemy);
        switch(enemies[enemy].type){
        case ENEMY_TYPE_ASTEROID:
            check_asteroids(enemy);
            break;
        default:
            check_enemies(enemy);
            break;
        }
        
    }
}

void draw_enemies(SDL_Renderer *renderer){
    // printf("ENEMY: Drawing Enemies...\n");
    if(!enemyTexturesLoaded){init_enemies(renderer);}
    for(int enemy = 0; enemy < POSSIBLE_ENEMIES; enemy++){
        if(!enemies[enemy].created){continue;}
        int x = (int)round(enemies[enemy].x);
        int y = (int)round(enemies[enemy].y);
        SDL_Rect renderRect = {
            .x = x,
            .y = y,
            .w = 70,
            .h = 70
        };
        int ret;
        switch(enemies[enemy].type){
        case ENEMY_TYPE_ASTEROID:
            ret = SDL_RenderCopy(renderer, enemiesTexture, &enemies[enemy].spriteRect, &renderRect);
            if(ret != 0){printf("SDL: Error Rendering Image - %s\n", SDL_GetError());}
            break;
        default:
            ret = SDL_RenderCopyEx(renderer, enemiesTexture, &enemies[enemy].spriteRect, &renderRect, enemies[enemy].target_dir, NULL, SDL_FLIP_NONE);
            if(ret != 0){printf("SDL: Error Rendering Image - %s\n", SDL_GetError());}
            break;
        } 
    }
    // printf("ENEMY: Finished Drawing Enemies...\n");
}