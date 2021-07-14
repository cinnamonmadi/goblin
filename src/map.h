#pragma once

#include "global.h"

typedef struct Map{
    Vector** tiles;
    int width;
    int height;
} Map;

typedef struct MapParams{
    int max_rooms;
    int min_rooms;
    int room_min_size;
    int room_max_size;
} MapParams;

Map* map_init(int width, int height, MapParams params);
void map_free(Map* map);
Vector** map_generate_tiles(int width, int height, MapParams params, bool* success);
