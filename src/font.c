#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "font.h"
#include "defs.h"
#include "game.h"
#include "ui.h"
#include "scores.h"

bool is_ttf_init = false;
SDL_Color SELECTED_COLOR = { 180,75,255 };
SDL_Color NOT_SELECTED_COLOR = { 180,255,75 };
SDL_Color MENU_TITLE_COLOR = { 255,100,100 };
SDL_Color SCORE_TEXT_COLOR = {255, 255, 0};

void init_ttf() {
    TTF_Init();
    is_ttf_init = true;
}

void create_text(SDL_Renderer *renderer, FontType fontType, int size, const char *string, Text *text){
    if(!is_ttf_init){
        init_ttf();
    }
    SDL_Color selected_color;
    SDL_Color not_selected_color;
    switch(fontType){
    case MENU_NAME_FONT:
        selected_color = MENU_TITLE_COLOR;
        not_selected_color = MENU_TITLE_COLOR;
        break;
    case REGULAR_FONT:
        selected_color = SCORE_TEXT_COLOR;
        not_selected_color = SCORE_TEXT_COLOR;
        break;
    default:
        selected_color = SELECTED_COLOR;
        not_selected_color = NOT_SELECTED_COLOR;
        break;
    }

    TTF_Font *font;
    switch (fontType)
    {
    case MENU_NAME_FONT:
        font = TTF_OpenFont(TITLE_FONT_DIR, size);
        break;
    case TITLE_FONT:
        font = TTF_OpenFont(TITLE_FONT_DIR, size);
        break;
    case THIN_FONT:
        font = TTF_OpenFont(THIN_FONT_DIR, size);
        break;
    case REGULAR_FONT:
        font = TTF_OpenFont(TITLE_FONT_DIR, size);
        break;
    default:
        font = NULL;
        break;
    }

    if(!font) {
        printf("Unable to create font!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }

    SDL_Surface *selected_surface = TTF_RenderText_Blended(font, string, selected_color);
    if(!selected_surface) {
        printf("Unable to render text surface!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, string, not_selected_color);
    if(!text_surface) {
        printf("Unable to render text surface!\nSDL2_ttf Error: %s\n\n", TTF_GetError());
        return;
    }

    SDL_Texture *selected_texture = SDL_CreateTextureFromSurface(renderer, selected_surface);
    if(!selected_texture) {
        printf("Unable to create texture from rendered text!\n"
                "SDL2 Error: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if(!texture) {
        printf("Unable to create texture from rendered text!\n"
                "SDL2 Error: %s\n", SDL_GetError());
        return;
    }
    Text temp_text = {
        .texture = texture,
        .selected_texture = selected_texture,
        .w = text_surface->w,
        .h = text_surface->h,
        .id = text->id
    };

    *text = temp_text;
    SDL_FreeSurface(selected_surface);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(font);
}

// -------------- TITLE SECTION --------------

bool scores_loaded = false;
Score *scores;
Text scoreTexts[10];
void free_scores(){
    free(scores);
    scores_loaded = false;
}
void load_scores(SDL_Renderer *renderer){
    printf("test loaded scores\n");
    scores = get_scores();
    char tempScoreStr[20];
    const char *pTempScoreStr = tempScoreStr;
    
    
    Text *pScoreTexts = scoreTexts;
    
    for(int i = 0; i < 10; i++, pScoreTexts++){
        if(!scores[i].exists) pTempScoreStr = "test";
        else sprintf(pTempScoreStr, "%d", scores[i].score);
        printf(pTempScoreStr);
        printf("\n");
        create_text(renderer, REGULAR_FONT, 20, pTempScoreStr, pScoreTexts);
    }
    scores_loaded = true;
}
void draw_title_scores_selections(SDL_Renderer *renderer){
    if(!scores_loaded) load_scores(renderer);
    SDL_Rect src;
    SDL_Rect dst;
    int ret = 0;
    for(int i = 0; i < 10; i++){
        //if(!scores[i].exists) continue;
        src.h = scoreTexts[i].h;
        src.w = scoreTexts[i].w;
        src.x = 0;
        src.y = 0;

        dst.h = src.h;
        dst.w = src.w;
        dst.x = 200;
        dst.y = 200+(50*(i+1));

        ret = SDL_RenderCopy(renderer, scoreTexts[i].selected_texture, &src, &dst);
        if(ret != 0) printf("BIG BAD BOI! %d\n", i);
    }
}

Text versionText = {
    .id = -1
};
SDL_Rect versionRect;
Text startText = {
    .id = 0
};
SDL_Rect startRect;
Text scoreText = {
    .id = 1
};
SDL_Rect scoreRect;
Text quitText = {
    .id = 2
};
SDL_Rect quitRect;
bool title_selections_loaded = false;
void load_title_main_selections(SDL_Renderer *renderer){
    create_text(renderer, REGULAR_FONT, 15, GAME_VERSION, &versionText);
    create_text(renderer, TITLE_FONT, 95, "Start", &startText);
    create_text(renderer, TITLE_FONT, 95, "Scores", &scoreText);
    create_text(renderer, TITLE_FONT, 95, "Quit", &quitText);
    title_selections_loaded = true;
}

void draw_title_main_selections(SDL_Renderer *renderer){
    if(!title_selections_loaded){
        load_title_main_selections(renderer);
    }

    int return_code;
    //x107 y140
    versionRect.x = GAME_WIDTH-(versionText.w+7);
    versionRect.y = GAME_HEIGHT-(versionText.h+7);
    versionRect.w = versionText.w;
    versionRect.h = versionText.h;

    if(get_game()->title.selection == startText.id){
        return_code = SDL_RenderCopy(renderer, versionText.selected_texture, NULL, &versionRect);
    }else {
        return_code = SDL_RenderCopy(renderer, versionText.texture, NULL, &versionRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    int start_x = 280;
    int interval = 115;
    
    startRect.x = (GAME_WIDTH-startText.w)/2;
    startRect.y = start_x;
    startRect.w = startText.w;
    startRect.h = startText.h;

    if(get_game()->title.selection == startText.id){
        return_code = SDL_RenderCopy(renderer, startText.selected_texture, NULL, &startRect);
    }else {
        return_code = SDL_RenderCopy(renderer, startText.texture, NULL, &startRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    scoreRect.x = (GAME_WIDTH-scoreText.w)/2;
    scoreRect.y = start_x+(interval*1);
    scoreRect.w = scoreText.w;
    scoreRect.h = scoreText.h;

    if(get_game()->title.selection == scoreText.id){
        return_code = SDL_RenderCopy(renderer, scoreText.selected_texture, NULL, &scoreRect);
    }else {
        return_code = SDL_RenderCopy(renderer, scoreText.texture, NULL, &scoreRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    quitRect.x = (GAME_WIDTH-quitText.w)/2;
    quitRect.y = start_x+(interval*2);
    quitRect.w = quitText.w;
    quitRect.h = quitText.h;

    if(get_game()->title.selection == quitText.id){
        return_code = SDL_RenderCopy(renderer, quitText.selected_texture, NULL, &quitRect);
    }else {
        return_code = SDL_RenderCopy(renderer, quitText.texture, NULL, &quitRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}

// -------------- PLAY SECTION --------------
Text pausedPlayText = {
    .id = -1
};
SDL_Rect pausedPlayRect;
Text resumePlayText = {
    .id = 0
};
SDL_Rect resumePlayRect;
Text mainMenuPlayText = {
    .id = 1
};
SDL_Rect mainMenuPlayRect;
bool play_paused_selections_loaded = false;
void load_play_paused_main_selections(SDL_Renderer *renderer) {
    create_text(renderer, MENU_NAME_FONT, 175, "Paused", &pausedPlayText);
    create_text(renderer, TITLE_FONT, 95, "Resume", &resumePlayText);
    create_text(renderer, TITLE_FONT, 95, "Main Menu", &mainMenuPlayText);
    play_paused_selections_loaded = true;
}
void draw_play_paused_main_selections(SDL_Renderer *renderer) {
    if(!play_paused_selections_loaded){
        load_play_paused_main_selections(renderer);
    }

    int return_code;

    pausedPlayRect.x = (GAME_WIDTH-pausedPlayText.w)/2;
    pausedPlayRect.y = 80;
    pausedPlayRect.w = pausedPlayText.w;
    pausedPlayRect.h = pausedPlayText.h;

    if(get_game()->play.paused_selection == pausedPlayText.id){
        return_code = SDL_RenderCopy(renderer, pausedPlayText.selected_texture, NULL, &pausedPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, pausedPlayText.texture, NULL, &pausedPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    int start_x = 400;
    int interval = 115;

    resumePlayRect.x = (GAME_WIDTH-resumePlayText.w)/2;
    resumePlayRect.y = start_x;
    resumePlayRect.w = resumePlayText.w;
    resumePlayRect.h = resumePlayText.h;

    if(get_game()->play.paused_selection == resumePlayText.id){
        return_code = SDL_RenderCopy(renderer, resumePlayText.selected_texture, NULL, &resumePlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, resumePlayText.texture, NULL, &resumePlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    mainMenuPlayRect.x = (GAME_WIDTH-mainMenuPlayText.w)/2;
    mainMenuPlayRect.y = start_x + (interval*1);
    mainMenuPlayRect.w = mainMenuPlayText.w;
    mainMenuPlayRect.h = mainMenuPlayText.h;

    if(get_game()->play.paused_selection == mainMenuPlayText.id){
        return_code = SDL_RenderCopy(renderer, mainMenuPlayText.selected_texture, NULL, &mainMenuPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, mainMenuPlayText.texture, NULL, &mainMenuPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}

Text gameoverPlayText = {
    .id = -1
};
SDL_Rect gameoverPlayRect;
Text scoreTextOverPlayText = {
    .id = -1
};
SDL_Rect scoreTextOverPlayRect;
Text scoreOverPlayText = {
    .id = -1
};
SDL_Rect scoreOverPlayRect;
Text goagainPlayText = {
    .id = 0
};
SDL_Rect goagainPlayRect;
Text mainMenuOverPlayText = {
    .id = 1
};
SDL_Rect mainMenuOverPlayRect;
bool play_over_selections_loaded = false;
void load_play_over_main_selections(SDL_Renderer *renderer) {
    create_text(renderer, MENU_NAME_FONT, 160, "Game Over", &gameoverPlayText);
    create_text(renderer, REGULAR_FONT, 70, "Score:", &scoreTextOverPlayText);
    create_text(renderer, TITLE_FONT, 95, "Go Again", &goagainPlayText);
    create_text(renderer, TITLE_FONT, 95, "Main Menu", &mainMenuOverPlayText);
    play_over_selections_loaded = true;
}
bool play_over_score_loaded = false;
void load_play_over_score(SDL_Renderer *renderer){
    create_text(renderer, REGULAR_FONT, 105, get_p_score_string(), &scoreOverPlayText);
    play_over_score_loaded = true;
}
void destroy_game_over_score(){
    SDL_DestroyTexture(scoreOverPlayText.texture);
    SDL_DestroyTexture(scoreOverPlayText.selected_texture);
    play_over_score_loaded = false;
}
void draw_play_over_main_selections(SDL_Renderer *renderer) {
    if(!play_over_selections_loaded){
        load_play_over_main_selections(renderer);
    }
    if(!play_over_score_loaded){
        load_play_over_score(renderer);
    }

    int return_code;

    gameoverPlayRect.x = (GAME_WIDTH-gameoverPlayText.w)/2;
    gameoverPlayRect.y = 80;
    gameoverPlayRect.w = gameoverPlayText.w;
    gameoverPlayRect.h = gameoverPlayText.h;

    if(get_game()->play.over_selection == gameoverPlayText.id){
        return_code = SDL_RenderCopy(renderer, gameoverPlayText.selected_texture, NULL, &gameoverPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, gameoverPlayText.texture, NULL, &gameoverPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    int scoreTextX = 275;
    int scoreActualX = scoreTextX+80;
    scoreTextOverPlayRect.x = (GAME_WIDTH-scoreTextOverPlayText.w)/2;
    scoreTextOverPlayRect.y = scoreTextX;
    scoreTextOverPlayRect.w = scoreTextOverPlayText.w;
    scoreTextOverPlayRect.h = scoreTextOverPlayText.h;

    if(get_game()->play.over_selection == scoreTextOverPlayText.id){
        return_code = SDL_RenderCopy(renderer, scoreTextOverPlayText.selected_texture, NULL, &scoreTextOverPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, scoreTextOverPlayText.texture, NULL, &scoreTextOverPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    scoreOverPlayRect.x = (GAME_WIDTH-scoreOverPlayText.w)/2;
    scoreOverPlayRect.y = scoreActualX;
    scoreOverPlayRect.w = scoreOverPlayText.w;
    scoreOverPlayRect.h = scoreOverPlayText.h;

    if(get_game()->play.over_selection == scoreOverPlayText.id){
        return_code = SDL_RenderCopy(renderer, scoreOverPlayText.selected_texture, NULL, &scoreOverPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, scoreOverPlayText.texture, NULL, &scoreOverPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    int start_x = 500;
    int interval = 115;

    goagainPlayRect.x = (GAME_WIDTH-goagainPlayText.w)/2;
    goagainPlayRect.y = start_x;
    goagainPlayRect.w = goagainPlayText.w;
    goagainPlayRect.h = goagainPlayText.h;

    if(get_game()->play.over_selection == goagainPlayText.id){
        return_code = SDL_RenderCopy(renderer, goagainPlayText.selected_texture, NULL, &goagainPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, goagainPlayText.texture, NULL, &goagainPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }

    mainMenuOverPlayRect.x = (GAME_WIDTH-mainMenuOverPlayText.w)/2;
    mainMenuOverPlayRect.y = start_x + interval;
    mainMenuOverPlayRect.w = mainMenuOverPlayText.w;
    mainMenuOverPlayRect.h = mainMenuOverPlayText.h;

    if(get_game()->play.over_selection == mainMenuOverPlayText.id){
        return_code = SDL_RenderCopy(renderer, mainMenuOverPlayText.selected_texture, NULL, &mainMenuOverPlayRect);
    }else {
        return_code = SDL_RenderCopy(renderer, mainMenuOverPlayText.texture, NULL, &mainMenuOverPlayRect);
    }
    if(return_code != 0){
        printf("SDL: Error Rendering Image - %s\n", SDL_GetError());
    }
}