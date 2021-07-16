#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

static const int VIEWPORT_WIDTH = 20;
static const int VIEWPORT_HEIGHT = 10;
static const int TILE_SIZE = 24;

typedef enum Sprite{
    SPRITE_NONE = -1,
    SPRITE_GOBLIN = 0,
    SPRITE_WASP = 1,
    SPRITE_TILE_WALL = 2,
    SPRITE_TILE_FLOOR = 3,
    SPRITE_COUNT = 4
} Sprite;

typedef struct Vector{
    int x;
    int y;
} Vector;

bool vector_equal(Vector a, Vector b);
Vector vector_sum(Vector a, Vector b);
Vector vector_minus(Vector a, Vector b);
Vector vector_increment(Vector a, int direction);

static const Vector DIRECTION_VECTORS[4] = {
    (Vector){ .x = 0, .y = -1 },
    (Vector){ .x = 1, .y = 0 },
    (Vector){ .x = 0, .y = 1 },
    (Vector){ .x = -1, .y = 0 }
};
extern const Vector SPRITE_VECTORS[SPRITE_COUNT];

int min(int a, int b);
int max(int a, int b);
bool rect_intersects(SDL_Rect* a, SDL_Rect* b);
int rand_range(int min, int max);
