#include <stdio.h>
#include <math.h>

#include <SDL_mixer.h>

#include "audio.h"

Mix_Music *title_bgm;
Mix_Music *play_bgm;
Mix_Music *over_bgm;
Mix_Chunk *playerdeadSound;
Mix_Chunk *deadSound;
Mix_Chunk *enterSound;
Mix_Chunk *hitSound;
Mix_Chunk *selectSound;
Mix_Chunk *windupSound;
Mix_Chunk *wooshSound;
Mix_Chunk *playerShootSound;
Mix_Chunk *enemyShootSound;
MixController mixController;
int musicVolume = 70;
int soundVolume = 150;

void init_mixer() {
    if(Mix_OpenAudio(88200*2, MIX_DEFAULT_FORMAT, 8, 1024) == -1){
        printf("SDL2_mixer could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
    }
    // Load .mp3 sound
    title_bgm = Mix_LoadMUS(TITLE_BACKGROUND_MUSIC_DIR);
    if(!title_bgm){
        printf(".mp3 sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", TITLE_BACKGROUND_MUSIC_DIR, SDL_GetError());
    }
    play_bgm = Mix_LoadMUS(PLAY_BACKGROUND_MUSIC_DIR);
    if(!play_bgm){
        printf(".mp3 sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", PLAY_BACKGROUND_MUSIC_DIR, SDL_GetError());
    }
    over_bgm = Mix_LoadMUS(OVER_BACKGROUND_MUSIC_DIR);
    if(!over_bgm){
        printf(".mp3 sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", PLAY_BACKGROUND_MUSIC_DIR, SDL_GetError());
    }
    Mix_VolumeMusic(musicVolume);

    selectSound = Mix_LoadWAV(SELECT_SOUND_DIR);
    if(!selectSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", SELECT_SOUND_DIR, SDL_GetError());
    }
    enterSound = Mix_LoadWAV(ENTER_SOUND_DIR);
    if(!enterSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", ENTER_SOUND_DIR, SDL_GetError());
    }
    hitSound = Mix_LoadWAV(HIT_SOUND_DIR);
    if(!hitSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", HIT_SOUND_DIR, SDL_GetError());
    }
    deadSound = Mix_LoadWAV(DEAD_SOUND_DIR);
    if(!deadSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", SELECT_SOUND_DIR, SDL_GetError());
    }
    playerdeadSound = Mix_LoadWAV(PLAYERDEAD_SOUND_DIR);
    if(!playerdeadSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", PLAYERDEAD_SOUND_DIR, SDL_GetError());
    }
    windupSound = Mix_LoadWAV(WINDUP_SOUND_DIR);
    if(!windupSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", WOOSH_SOUND_DIR, SDL_GetError());
    }
    wooshSound = Mix_LoadWAV(WOOSH_SOUND_DIR);
    if(!wooshSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", WOOSH_SOUND_DIR, SDL_GetError());
    }
    playerShootSound = Mix_LoadWAV(PLAYER_SHOOT_SOUND_DIR);
    if(!playerShootSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", PLAYER_SHOOT_SOUND_DIR, SDL_GetError());
    }
    enemyShootSound = Mix_LoadWAV(ENEMY_SHOOT_SOUND_DIR);
    if(!enemyShootSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", ENEMY_SHOOT_SOUND_DIR, SDL_GetError());
    }

    // SELECT CHANNEL
    Mix_Volume(1, soundVolume);
    // ENTER CHANNEL
    Mix_Volume(2, soundVolume);
    // PLAYER SHOOT CHANNEL
    Mix_Volume(3, round(soundVolume*0.20f));
    // ENEMY SHOOT CHANNEL
    Mix_Volume(4, round(soundVolume*0.25f));
    // WOOSH CHANNEL
    Mix_Volume(5, round(soundVolume*0.70f));
    // HIT CHANNEL
    Mix_Volume(6, round(soundVolume*0.99f));
    // DEAD CHANNEL
    Mix_Volume(7, round(soundVolume*0.70f));
}

void start_title_music() {
    Mix_VolumeMusic(musicVolume*2);
    if(Mix_PlayMusic(title_bgm, -1) == -1) {
        printf(".OGG sound could not be played!\n"
                "SDL_Error: %s\n", SDL_GetError());
    }
}

void start_play_music() {
    Mix_VolumeMusic(musicVolume*0.7);
    if(Mix_PlayMusic(play_bgm, -1) == -1) {
        printf(".OGG sound could not be played!\n"
                "SDL_Error: %s\n", SDL_GetError());
    }
}

void start_over_music() {
    Mix_VolumeMusic(musicVolume*0.7);
    if(Mix_PlayMusic(over_bgm, -1) == -1) {
        printf(".OGG sound could not be played!\n"
                "SDL_Error: %s\n", SDL_GetError());
    }
}

void pause_music() {

}

void set_music_volume_play(){
    Mix_VolumeMusic(musicVolume);
}
void set_music_volume_paused(){
    Mix_VolumeMusic(Mix_VolumeMusic(-1)/3);
}

void stop_chargup_sound(){
    Mix_ExpireChannel(5,1);
}

void stop_all_sounds(){
    Mix_ExpireChannel(-1,1);
}

void play_sound(Sound sound) {
    switch(sound) {
        case ENTER_SOUND:
            if(Mix_PlayChannel(2, enterSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(enterSound);
            }
            break;
        case SELECT_SOUND:
            if(Mix_PlayChannel(1, selectSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(selectSound);
            }
            break;
        case HIT_SOUND:
            if(Mix_PlayChannel(6, hitSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(hitSound);
            }
            break;
        case DEAD_SOUND:
            if(Mix_PlayChannel(7, deadSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(deadSound);
            }
            break;
        case PLAYERDEAD_SOUND:
            if(Mix_PlayChannel(7, playerdeadSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(playerdeadSound);
            }
            break;
        case WINDUP_SOUND:
            if(Mix_PlayChannel(5, windupSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(windupSound);
            }
            break;
        case WOOSH_SOUND:
            if(Mix_PlayChannel(5, wooshSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(wooshSound);
            }
            break;
        case PLAYER_SHOOT_SOUND:
            if(Mix_PlayChannel(3, playerShootSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(playerShootSound);
            }
            break;
        case ENEMY_SHOOT_SOUND:
            if(Mix_PlayChannel(4, enemyShootSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(enemyShootSound);
            }
            break;
        default:
            break;
    }
}

void free_mixer() {
    Mix_FreeMusic(title_bgm);
}