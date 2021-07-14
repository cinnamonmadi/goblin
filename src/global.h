#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

static const int VIEWPORT_WIDTH = 20;
static const int VIEWPORT_HEIGHT = 10;
static const int TILE_SIZE = 24;

typedef struct Vector{
    int x;
    int y;
} Vector;

static const Vector SPRITE_NONE = (Vector){ .x = -1, .y = -1 };
static const Vector SPRITE_GOBLIN = (Vector){ .x = 40, .y = 2 };
static const Vector SPRITE_TILE_WALL = (Vector){ .x = 3, .y = 3 };

int min(int a, int b);
int max(int a, int b);
bool rect_intersects(SDL_Rect* a, SDL_Rect* b);
int rand_range(int min, int max);
