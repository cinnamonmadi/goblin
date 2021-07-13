#include "engine.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdint.h>

// Logical screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

// UI constants
const int UI_MARGIN = 10;
SDL_Rect viewport_rect;
SDL_Rect sidebar_rect;
SDL_Rect chatlog_rect;

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
const SDL_Color COLOR_RED = (SDL_Color){ .r = 255, .g = 0, .b = 0, .a = 255 };
const SDL_Color COLOR_YELLOW = (SDL_Color){ .r = 255, .g = 255, .b = 0, .a = 255 };
const SDL_Color COLOR_GREEN = (SDL_Color){ .r = 0, .g = 255, .b = 0, .a = 255 };

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

void engine_init_ui_rects(){

    viewport_rect = (SDL_Rect){
        .x = UI_MARGIN,
        .y = UI_MARGIN,
        .w = VIEWPORT_WIDTH * TILE_SIZE,
        .h = VIEWPORT_HEIGHT * TILE_SIZE
    };
    sidebar_rect = (SDL_Rect){
        .x = viewport_rect.x + viewport_rect.w + UI_MARGIN,
        .y = UI_MARGIN,
        .w = SCREEN_WIDTH - (UI_MARGIN * 3) - viewport_rect.w,
        .h = viewport_rect.h
    };
    chatlog_rect = (SDL_Rect){
        .x = UI_MARGIN,
        .y = viewport_rect.y + viewport_rect.h + UI_MARGIN,
        .w = SCREEN_WIDTH - (UI_MARGIN * 2),
        .h = SCREEN_HEIGHT - (UI_MARGIN * 3) - viewport_rect.h
    };
}

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

    engine_init_ui_rects();

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

void engine_render_ui(){

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderDrawRect(renderer, &viewport_rect);
    SDL_RenderDrawRect(renderer, &sidebar_rect);
    SDL_RenderDrawRect(renderer, &chatlog_rect);
}

void engine_render_log(Log* log){

    for(int i = 0; i < log->length; i++){

        int log_index = log->position - i;
        if(log_index < 0){

            log_index += log->length;
        }
        int log_char_index = 0;

        char message[3][128];
        SDL_Color message_colors[3] = { COLOR_WHITE, COLOR_RED, COLOR_YELLOW };
        int current_color = 0;
        int message_index = 0;

        while(log->messages[log_index][log_char_index] != '\0'){

            if(current_color != 0 && log->messages[log_index][log_char_index] == ')'){

                current_color = 0;
                log_char_index++;
                continue;

            }else if(log->messages[log_index][log_char_index] == 'r' && log->messages[log_index][log_char_index + 1] == '('){

                current_color = 1;
                log_char_index += 2;
                continue;

            }else if(log->messages[log_index][log_char_index] == 'y' && log->messages[log_index][log_char_index + 1] == '('){

                current_color = 2;
                log_char_index += 2;
                continue;
            }

            for(int color = 0; color < 3; color++){

                if(color == current_color){

                    message[color][message_index] = log->messages[log_index][log_char_index];

                }else{

                    message[color][message_index] = ' ';
                }
            }

            message_index++;
            log_char_index++;
        }

        for(int color = 0; color < 3; color++){

            message[color][message_index] = '\0';
            engine_render_text(message[color], message_colors[color], chatlog_rect.x + 5, chatlog_rect.y + 2 + (12 * i));
        }
    }
}

void engine_render_fps(){

    char fps_text[10];
    sprintf(fps_text, "FPS: %i", fps);
    engine_render_text(fps_text, COLOR_WHITE, 0, 0);
}

void engine_render_sprite(const Vector position, const Vector sprite){

    if(position.x < 0 || position.x >= VIEWPORT_WIDTH || position.y < 0 || position.y >= VIEWPORT_HEIGHT){

        return;
    }

    if(sprite.x < 0 || sprite.x >= spritesheet_width || sprite.y < 0 || sprite.y >= spritesheet_height){

        printf("Error rendering sprite! Coordinate of %i, %i is out of bounds.\n", sprite.x, sprite.y);
        return;
    }

    SDL_Rect src_rect = (SDL_Rect){

        .x = 1 + (sprite.x * 13),
        .y = 1 + (sprite.y * 13),
        .w = 12,
        .h = 12
    };
    SDL_Rect dst_rect = (SDL_Rect){

        .x = viewport_rect.x + (position.x * TILE_SIZE),
        .y = viewport_rect.y + (position.y * TILE_SIZE),
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };

    SDL_RenderCopy(renderer, spritesheet, &src_rect, &dst_rect);
}
