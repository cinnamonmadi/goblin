#pragma once

#include "global.h"

typedef struct MapParams{
    int max_rooms;
    int min_rooms;
    int room_min_size;
    int room_max_size;
} MapParams;

typedef struct RoomData{
    SDL_Rect* rooms;
    SDL_Rect* hallways;
    int room_count;
} RoomData;

typedef struct Map{
    Vector** tiles;
    bool** walls;
    int width;
    int height;

    Vector player_spawn;
} Map;

Map* map_init(int width, int height, MapParams params);
void map_free(Map* map);
void map_generate_tiles(Map* map, int width, int height, RoomData* data);

RoomData* map_room_data_generate(int width, int height, MapParams params);
void map_room_data_free(RoomData* data);
bool map_room_data_validate(RoomData* data, MapParams params);
