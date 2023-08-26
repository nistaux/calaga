#include <stdio.h>
#include <math.h>

#include <SDL_mixer.h>

#include "audio.h"

#define TITLE_BACKGROUND_MUSIC_DIR "./assets/audio/music/Final Quest - Adventurer's End Loop.mp3"
#define PLAY_BACKGROUND_MUSIC_DIR "./assets/audio/music/Final Quest - Battle.mp3"
#define DEAD_SOUND_DIR "./assets/audio/sfx/dead.wav"
#define ENTER_SOUND_DIR "./assets/audio/sfx/enter.wav"
#define HIT_SOUND_DIR "./assets/audio/sfx/hit.wav"
#define SELECT_SOUND_DIR "./assets/audio/sfx/select.wav"
#define WOOSH_SOUND_DIR "./assets/audio/sfx/woosh.wav"
#define PLAYER_SHOOT_SOUND_DIR "./assets/audio/sfx/blaster_06.wav"

Mix_Music *title_bgm;
Mix_Music *play_bgm;
Mix_Chunk *deadSound;
Mix_Chunk *enterSound;
Mix_Chunk *hitSound;
Mix_Chunk *selectSound;
Mix_Chunk *wooshSound;
Mix_Chunk *playerShootSound;
MixController mixController;
int musicVolume = 25;
int soundVolume = 35;

void init_mixer() {
    if(Mix_OpenAudio(88200*2, MIX_DEFAULT_FORMAT, 5, 1024) == -1){
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
    selectSound = Mix_LoadWAV(SELECT_SOUND_DIR);
    if(!selectSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", SELECT_SOUND_DIR, SDL_GetError());
    }
    wooshSound = Mix_LoadWAV(WOOSH_SOUND_DIR);
    if(!wooshSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", WOOSH_SOUND_DIR, SDL_GetError());
    }
    playerShootSound = Mix_LoadWAV(PLAYER_SHOOT_SOUND_DIR);
    if(!wooshSound){
        printf(".WAV sound '%s' could not be loaded!\n"
                "SDL_Error: %s\n", PLAYER_SHOOT_SOUND_DIR, SDL_GetError());
    }

    // SELECT CHANNEL
    Mix_Volume(1, soundVolume);
    // ENTER CHANNEL
    Mix_Volume(2, soundVolume);
    // SHOOT CHANNEL
    Mix_Volume(3, round(soundVolume*0.25f));
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

void pause_music() {

}

void set_music_volume_play(){
    Mix_VolumeMusic(musicVolume);
}
void set_music_volume_paused(){
    Mix_VolumeMusic(Mix_VolumeMusic(-1)/3);
}

void play_sound(Sound sound) {
    switch(sound) {
        case DEAD_SOUND:
            break;
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
            break;
        case WOOSH_SOUND:
            break;
        case PLAYER_SHOOT_SOUND:
            if(Mix_PlayChannel(3, playerShootSound, 0) == -1){
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(playerShootSound);
            }
            break;
        default:
            break;
    }
}

void free_mixer() {
    Mix_FreeMusic(title_bgm);
}