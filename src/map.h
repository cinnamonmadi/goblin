#pragma once

#include "global.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Map{
    SDL_Rect* rooms;
    int room_count;
    int width;
    int height;
} Map;

// Map* map_init(int width, int height);
bool** map_generate(int width, int height);
