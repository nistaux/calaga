#include <stdio.h>

#include <SDL_mixer.h>

#include "audio.h"

#define TITLE_BACKGROUND_MUSIC_DIR "./res/audio/music/Final Quest - Adventurer's End Loop.mp3"
#define DEAD_SOUND_DIR "./res/audio/sfx/dead.wav"
#define ENTER_SOUND_DIR "./res/audio/sfx/enter.wav"
#define HIT_SOUND_DIR "./res/audio/sfx/hit.wav"
#define SELECT_SOUND_DIR "./res/audio/sfx/select.wav"
#define WOOSH_SOUND_DIR "./res/audio/sfx/woosh.wav"

Mix_Music *title_bgm;
Mix_Chunk *deadSound;
Mix_Chunk *enterSound;
Mix_Chunk *hitSound;
Mix_Chunk *selectSound;
Mix_Chunk *wooshSound;
MixController mixController;

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
    Mix_VolumeMusic(25);

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

    Mix_Volume(-1, 25);
}

void start_title_music() {
    if(Mix_PlayMusic(title_bgm, 300) == -1) {
        printf(".OGG sound could not be played!\n"
                "SDL_Error: %s\n", SDL_GetError());
    }
}

void start_play_music() {

}

void pause_music() {

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
        default:
            break;
    }
}

void free_mixer() {
    Mix_FreeMusic(title_bgm);
}