#pragma once

static const int VIEWPORT_WIDTH = 20;
static const int VIEWPORT_HEIGHT = 10;
static const int TILE_SIZE = 24;

typedef struct Vector{
    int x;
    int y;
} Vector;

static const Vector SPRITE_GOBLIN = (Vector){ .x = 40, .y = 2 };
