#include "map.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

Map* map_init(int width, int height, MapParams params){

    Map* map = malloc(sizeof(Map));
    map->width = width;
    map->height = height;

    RoomData* data;
    bool data_successful = false;
    while(!data_successful){

        data = map_room_data_generate(width, height, params);
        data_successful = map_room_data_validate(data, params);
        if(!data_successful){

            map_room_data_free(data);
        }
    }
    map_generate_tiles(map, width, height, data);

    SDL_Rect player_spawn_room = data->rooms[rand_range(0, data->room_count - 1)];
    map->player_spawn = (Vector){
        .x = rand_range(player_spawn_room.x + 1, player_spawn_room.x + player_spawn_room.w - 1),
        .y = rand_range(player_spawn_room.y + 1, player_spawn_room.y + player_spawn_room.h - 1)
    };

    map->enemy_spawn = map->player_spawn;
    while(vector_equal(map->enemy_spawn, map->player_spawn)){

        SDL_Rect enemy_spawn_room = data->rooms[rand_range(0, data->room_count - 1)];
        map->enemy_spawn = (Vector){
            .x = rand_range(enemy_spawn_room.x + 1, enemy_spawn_room.x + enemy_spawn_room.w - 1),
            .y = rand_range(enemy_spawn_room.y + 1, enemy_spawn_room.y + enemy_spawn_room.h - 1)
        };
    }

    map_room_data_free(data);

    return map;
}

void map_free(Map* map){

    for(int x = 0; x < map->width; x++){

        free(map->tiles[x]);
        free(map->walls[x]);
    }
    free(map->tiles);
    free(map->walls);
    free(map);
}

RoomData* map_room_data_generate(int width, int height, MapParams params){

    // Generate rooms
    RoomData* data = malloc(sizeof(RoomData));
    *data = (RoomData){
        .rooms = malloc(sizeof(SDL_Rect) * params.max_rooms),
        .hallways = malloc(sizeof(SDL_Rect) * (params.max_rooms - 1) * 2),
        .room_count = 0
    };
    for(int r = 0; r < params.max_rooms; r++){

        int w = rand_range(params.room_min_size, params.room_max_size);
        int h = rand_range(params.room_min_size, params.room_max_size);
        int x = rand_range(1, width - w - 1);
        int y = rand_range(1, height - h - 1);

        SDL_Rect new_room = { .x = x, .y = y, .w = w, .h = h };

        // If the room overlaps with a previous room, don't create it
        bool overlaps_previous = false;
        for(int other_r = 0; other_r < data->room_count; other_r++){

            if(rect_intersects(&new_room, &(data->rooms[other_r]))){

                overlaps_previous = true;
                break;
            }
        }
        if(overlaps_previous){

            continue;
        }

        // Add the room to the map
        int room_index = data->room_count;
        data->rooms[room_index] = new_room;
        data->room_count++;

        // If the room is the first room we've created, don't do anything further
        if(room_index == 0){

            continue;
        }

        // Otherwise, create a tunnel connecting the new room to the previous room
        int previous_room = room_index - 1;

        Vector new_center = (Vector){
            .x = data->rooms[room_index].x + (data->rooms[room_index].w / 2),
            .y = data->rooms[room_index].y + (data->rooms[room_index].h / 2)
        };
        Vector previous_center = (Vector){
            .x = data->rooms[previous_room].x + (data->rooms[previous_room].w / 2),
            .y = data->rooms[previous_room].y + (data->rooms[previous_room].h / 2)
        };

        int min_x = min(new_center.x, previous_center.x);
        int max_x = max(new_center.x, previous_center.x);
        int min_y = min(new_center.y, previous_center.y);
        int max_y = max(new_center.y, previous_center.y);

        int hallway_index_a = (room_index - 1) * 2;
        int hallway_index_b = hallway_index_a + 1;

        bool tunnel_horizontally = rand() % 2 == 0;
        if(tunnel_horizontally){

            data->hallways[hallway_index_a] = (SDL_Rect){

                .x = min_x,
                .y = previous_center.y,
                .w = max_x - min_x + 1,
                .h = 1
            };
            data->hallways[hallway_index_b] = (SDL_Rect){

                .x = new_center.x,
                .y = min_y,
                .w = 1,
                .h = max_y - min_y + 1
            };

        }else{

            data->hallways[hallway_index_a] = (SDL_Rect){

                .x = previous_center.x,
                .y = min_y,
                .w = 1,
                .h = max_y - min_y + 1
            };
            data->hallways[hallway_index_b] = (SDL_Rect){

                .x = min_x,
                .y = new_center.y,
                .w = max_x - min_x + 1,
                .h = 1
            };
        }
    }

    return data;
}

void map_room_data_free(RoomData* data){

    free(data->rooms);
    free(data->hallways);
    free(data);
}

bool map_room_data_validate(RoomData* data, MapParams params){

    return data->room_count >= params.min_rooms && data->room_count <= params.max_rooms;
}

void map_generate_tiles(Map* map, int width, int height, RoomData* data){

    // Start with an empty floors array and an empty tile array
    bool floors[width][height];
    map->tiles = malloc(sizeof(Sprite*) * width);
    map->walls = malloc(sizeof(bool*) * width);
    for(int x = 0; x < width; x++){

        map->tiles[x] = malloc(sizeof(Vector) * height);
        map->walls[x] = malloc(sizeof(bool) * height);
        for(int y = 0; y < height; y++){

            floors[x][y] = false;
            map->tiles[x][y] = SPRITE_NONE;
            map->walls[x][y] = false;
        }
    }

    // Fill a 'true' value on the floors for each point on each room and hallway
    int num_rooms = data->room_count;
    int num_hallways = (data->room_count - 1) * 2;
    for(int r = 0; r < num_rooms + num_hallways; r++){

        SDL_Rect to_map;
        if(r < num_rooms){

            to_map = data->rooms[r];

        }else{

            to_map = data->hallways[r - num_rooms];
        }
        for(int x = to_map.x; x < to_map.x + to_map.w; x++){

            for(int y = to_map.y; y < to_map.y + to_map.h; y++){

                floors[x][y] = true;
            }
        }
    }

    // Fill a wall tile on every open space around the floors
    for(int x = 0; x < width; x++){

        for(int y = 0; y < height; y++){

            if(!floors[x][y]){

                continue;
            }

            map->tiles[x][y] = SPRITE_TILE_FLOOR;

            Vector adjacent_squares[8];
            int adjacent_index = 0;
            for(int ax = -1; ax < 2; ax++){

                for(int ay = -1; ay < 2; ay++){

                    if(ax == 0 && ay == 0){

                        continue;
                    }
                    adjacent_squares[adjacent_index] = (Vector){ .x = x + ax, .y = y + ay };
                    adjacent_index++;
                }
            }

            for(int a = 0; a < 8; a++){

                // This shouldn't be possible but it doesn't hurt to check
                Vector square = adjacent_squares[a];
                if(square.x < 0 || square.x >= width || square.y < 0 || square.y >= height){

                    continue;
                }

                if(!floors[square.x][square.y] && map->tiles[square.x][square.y] == SPRITE_NONE){

                    map->tiles[square.x][square.y] = SPRITE_TILE_WALL;
                    map->walls[square.x][square.y] = true;
                }
            }
        }
    }
}
