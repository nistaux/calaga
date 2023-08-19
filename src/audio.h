#pragma once

typedef enum {
    TITLE_BGM,
    PLAY_BGM
} Music;

typedef enum {
    DEAD_SOUND,
    ENTER_SOUND,
    SELECT_SOUND,
    HIT_SOUND,
    WOOSH_SOUND
} Sound;

typedef struct {
    Music currentMusic;
} MixController;


void init_mixer();

void start_title_music();
void start_play_music();
void pause_music();
void set_music_volume_play();
void set_music_volume_paused();

void play_sound(Sound sound);

void free_mixer();