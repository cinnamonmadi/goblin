#include "engine.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdint.h>

// Logical screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

// SDL objects
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool is_fullscreen = false;

// Art variables
SDL_Texture* spritesheet = NULL;
int spritesheet_width = 0;
int spritesheet_height = 0;

// Font variables
TTF_Font* font_small;

// Color constants
const SDL_Color COLOR_WHITE = (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 };

// Timing variables
const unsigned long SECOND = 1000;
const float FRAME_TIME = SECOND / 60.0;
const float UPDATE_TIME = SECOND / 60.0;
unsigned long second_before_time;
unsigned long frame_before_time;
unsigned long last_update_time;
float deltas = 0;
int frames = 0;
int fps = 0;
int ups = 0;


// Engine initializiation functions

bool engine_init(const char* title){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){

        printf("Unable to initialize SDL! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int img_flags = IMG_INIT_PNG;

    if(!(IMG_Init(img_flags) & img_flags)){

        printf("Unable to initialize SDL_image! SDL Error: %s\n", IMG_GetError());
        return false;
    }

    if(TTF_Init() == -1){

        printf("Unable to initialize SDL_ttf! SDL Error: %s\n", TTF_GetError());
        return false;
    }

    if(!window || !renderer){

        printf("Unable to initialize engine!\n");
        return false;
    }

    font_small = TTF_OpenFont("./res/hack.ttf", 10);
    if(font_small == NULL){

        printf("Unable to initialize font_small! SDL Error: %s\n", TTF_GetError());
        return false;
    }

    // Load tileset
    SDL_Surface* loaded_surface = IMG_Load("./res/spritesheet.png");
    if(loaded_surface == NULL){

        printf("Unable to load spritesheet image! SDL Error: %s\n", IMG_GetError());
        return NULL;
    }

    spritesheet = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if(spritesheet == NULL){

        printf("Unable to create spritesheet texture! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }

    spritesheet_width = (int)((loaded_surface->w - 1) / 13);
    spritesheet_height = (int)((loaded_surface->h - 1) / 13);

    engine_set_resolution(1280, 720);

    return true;
}

void engine_quit(){

    SDL_DestroyTexture(spritesheet);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void engine_set_resolution(int width, int height){

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void engine_toggle_fullscreen(){

    if(is_fullscreen){

        SDL_SetWindowFullscreen(window, 0);

    }else{

        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    is_fullscreen = !is_fullscreen;
}

// Timing functions

void engine_clock_init(){

    second_before_time = SDL_GetTicks();
    frame_before_time = second_before_time;
    last_update_time = second_before_time;
}

float engine_clock_tick(){

    frames++;
    unsigned long current_time = SDL_GetTicks();

    if(current_time - second_before_time >= SECOND){

        fps = frames;
        ups = (int)deltas;
        frames = 0;
        deltas -= ups;
        second_before_time += SECOND;
    }

    float delta = (current_time - last_update_time) / UPDATE_TIME;
    deltas += delta;
    last_update_time = current_time;

    if(current_time - frame_before_time < FRAME_TIME){

        unsigned long delay_time = FRAME_TIME - (current_time - frame_before_time);
        SDL_Delay(delay_time);
    }

    return delta;
}

// Rendering functions

void engine_render_text(const char* text, SDL_Color color, int x, int y){

    SDL_Surface* text_surface = TTF_RenderText_Solid(font_small, text, color);
    if(text_surface == NULL){

        printf("Unable to render text to surface! SDL Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    if(text_texture == NULL){

        printf("Unable to reate texture! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect source_rect = (SDL_Rect){ .x = 0, .y = 0, .w = text_surface->w, .h = text_surface->h };
    SDL_Rect dest_rect = (SDL_Rect){ .x = x, .y = y, .w = text_surface->w, .h = text_surface->h };
    SDL_RenderCopy(renderer, text_texture, &source_rect, &dest_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

void engine_render_clear(){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void engine_render_present(){

    SDL_RenderPresent(renderer);
}

void engine_render_fps(){

    char fps_text[10];
    sprintf(fps_text, "FPS: %i", fps);
    engine_render_text(fps_text, COLOR_WHITE, 0, 0);
}

void engine_render_sprite(const int tx, const int ty, const int x, const int y){

    if(tx < 0 || tx >= spritesheet_width || ty < 0 || ty >= spritesheet_height){

        printf("Error rendering sprite! Coordinate of %i, %i is out of bounds.\n", tx, ty);
        return;
    }

    SDL_Rect src_rect = (SDL_Rect){

        .x = 1 + (tx * 13),
        .y = 1 + (ty * 13),
        .w = 12,
        .h = 12
    };
    SDL_Rect dst_rect = (SDL_Rect){

        .x = x,
        .y = y,
        .w = 36,
        .h = 36
    };

    SDL_RenderCopy(renderer, spritesheet, &src_rect, &dst_rect);
}
