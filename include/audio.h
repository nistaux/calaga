#pragma once

#define TITLE_BACKGROUND_MUSIC_DIR "./assets/audio/music/Final Quest - Adventurer's End Loop.mp3"
#define PLAY_BACKGROUND_MUSIC_DIR "./assets/audio/music/Final Quest - Battle.mp3"
#define OVER_BACKGROUND_MUSIC_DIR "./assets/audio/music/Final Quest - Battle Triumph.mp3"
#define DEAD_SOUND_DIR "./assets/audio/sfx/dead.wav"
#define PLAYERDEAD_SOUND_DIR "./assets/audio/sfx/player_dead.wav"
#define ENTER_SOUND_DIR "./assets/audio/sfx/enter.wav"
#define HIT_SOUND_DIR "./assets/audio/sfx/hit.wav"
#define SELECT_SOUND_DIR "./assets/audio/sfx/select.wav"
#define WINDUP_SOUND_DIR "./assets/audio/sfx/wind_up.wav"
#define WOOSH_SOUND_DIR "./assets/audio/sfx/woosh.wav"
#define PLAYER_SHOOT_SOUND_DIR "./assets/audio/sfx/blaster_06.wav"
#define ENEMY_SHOOT_SOUND_DIR "./assets/audio/sfx/blaster_03.wav"

typedef enum {
    TITLE_BGM,
    PLAY_BGM
} Music;

typedef enum {
    DEAD_SOUND,
    PLAYERDEAD_SOUND,
    ENTER_SOUND,
    SELECT_SOUND,
    HIT_SOUND,
    WINDUP_SOUND,
    WOOSH_SOUND,
    PLAYER_SHOOT_SOUND,
    ENEMY_SHOOT_SOUND,
} Sound;

typedef struct {
    Music currentMusic;
} MixController;


void init_mixer();

void start_title_music();
void start_play_music();
void start_over_music();
void pause_music();
void set_music_volume_play();
void set_music_volume_paused();
void stop_chargup_sound();
void stop_all_sounds();

void play_sound(Sound sound);

void free_mixer();