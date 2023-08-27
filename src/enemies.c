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

SDL_Texture *enemiesTexture;
int total_enemies = 0;
Enemy enemies[POSSIBLE_ENEMIES];
bool enemyTexturesLoaded = false;

void init_enemies(SDL_Renderer *renderer){
    // create sprite sheet
    enemiesTexture = IMG_LoadTexture(renderer, ENEMY_SPRITE_DIR);
    enemyTexturesLoaded = true;
}

int redistribute_enemy_array(){
    if(total_enemies == 0){return 0;}
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
void create_enemy(EnemyType type, MoveType moveType, int startLocation){
    switch (type){
    case ENEMY_TYPE_BEADER:
        create_beader(moveType, startLocation);
        break;
    default:
        break;
    }
}

void destroy_enemy(int index){
    Enemy zero = {
        .created = false,
    };
    int fieldSpace = enemies[index].fieldReservation;
    MoveType type = enemies[index].moveType;
    enemies[index] = zero;
    total_enemies--;
    free_field_space(fieldSpace, type);
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
    enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
    enemies[enemyIndex].y += enemies[enemyIndex].y_vel*enemies[enemyIndex].speed;
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
    enemies[enemyIndex].x += enemies[enemyIndex].x_vel*enemies[enemyIndex].speed;
}

void turn_enemy(int enemyIndex){
    enemies[enemyIndex].target_slope_x = (get_player_x()+35.0f) - (enemies[enemyIndex].x+35.0f);
    enemies[enemyIndex].target_slope_y = (675.0f+35.0f) - (enemies[enemyIndex].y+35.0f);
    double angle = atan(enemies[enemyIndex].target_slope_x/enemies[enemyIndex].target_slope_y)*-50.0;

    enemies[enemyIndex].target_dir = angle;
}

void move_enemies(int enemyIndex){
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
    turn_enemy(enemyIndex);
}

void shoot_enemies(int enemyIndex){
    float lastShotDiff = ((float)SDL_GetTicks64()/1000.0f) - enemies[enemyIndex].last_shot;
    if(lastShotDiff >= enemies[enemyIndex].shoot_reload_interval_seconds){enemies[enemyIndex].reloading = false;}
    if(enemies[enemyIndex].reloading){return;}

    SDL_Rect dstRect = {
        .x = 0,
        .y = 0,
        .w = 17,
        .h = 20
    };
    SDL_Rect srcRect;
    switch(enemies[enemyIndex].type){
    case ENEMY_TYPE_BEADER:
        srcRect.x = 17;
        srcRect.y = 0;
        srcRect.w = 17;
        srcRect.h = 20;
        break;
    case ENEMY_TYPE_TSHOT:
        break;
    default:
        break;
    }
    
    Projectile temp = {
        .speed = 1.1f,
        .dstRect = dstRect,
        .srcRect = srcRect,
        .type = PROJ_BEADER,
        .angle = enemies[enemyIndex].target_dir,
        .x = enemies[enemyIndex].x+35.0f,
        .y = enemies[enemyIndex].y+35.0f,
        .x_vel = 0.0f,
        .y_vel = 0.0f,
    };
    temp.x_vel = temp.speed * (enemies[enemyIndex].target_slope_x/enemies[enemyIndex].target_slope_y);
    temp.y_vel = temp.speed;
    temp.x += (enemies[enemyIndex].target_slope_x/enemies[enemyIndex].target_slope_y)*20.0f;
    temp.y += 20.0f;
    enemies[enemyIndex].last_shot = (float)SDL_GetTicks64()/1000.0f;
    enemies[enemyIndex].reloading = true;
    create_projectile(temp);
}

void kill_enemy(int enemyIndex, int projectileIndex){
    ScoreText temp = {
        .x = enemies[enemyIndex].x+15.0f,
        .y = enemies[enemyIndex].y+5.0f
    };
    switch(enemies[enemyIndex].type){
    default:
        increase_score(100, temp);
        break;
    }
    destroy_projectile(projectileIndex);
    destroy_enemy(enemyIndex); 
}

void check_enemies(int enemyIndex){
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
            kill_enemy(enemyIndex, i);
        }else if(proj.x+proj.srcRect.w > enemy.x && 
        proj.y < (enemy.y+enemy.spriteRect.h-15.0f) &&
        proj.x < enemy.x+enemy.spriteRect.w &&
        proj.y > enemy.y
        ){
            kill_enemy(enemyIndex, i);
        }
    }
}

void tick_enemies(){
    for(int enemy = 0; enemy < POSSIBLE_ENEMIES; enemy++){
        if(enemies[enemy].created == true){
            move_enemies(enemy);
            check_enemies(enemy);
            shoot_enemies(enemy);
        }
    }
}

void draw_enemies(SDL_Renderer *renderer){
    if(!enemyTexturesLoaded){init_enemies(renderer);}
    for(int enemy = 0; enemy < POSSIBLE_ENEMIES; enemy++){
        if(enemies[enemy].created == true){
            int x = (int)round(enemies[enemy].x);
            int y = (int)round(enemies[enemy].y);
            SDL_Rect renderRect = {
                .x = x,
                .y = y,
                .w = 70,
                .h = 70
            };
            switch(enemies[enemy].type){
            case ENEMY_TYPE_BEADER:
                if(SDL_RenderCopyEx(renderer, enemiesTexture, &enemies[enemy].spriteRect, &renderRect, enemies[enemy].target_dir, NULL, SDL_FLIP_NONE) != 0){
                    printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
                }
                break;
            default:
                break;
            }
        }
    }
}