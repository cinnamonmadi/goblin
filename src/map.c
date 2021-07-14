#include "map.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

Map* map_init(int width, int height, MapParams params){

    Map* map = malloc(sizeof(Map));
    map->width = width;
    map->height = height;

    bool success = false;
    while(!success){

        map->tiles = map_generate_tiles(width, height, params, &success);
    }

    return map;
}

void map_free(Map* map){

    for(int x = 0; x < map->width; x++){

        free(map->tiles[x]);
    }
    free(map->tiles);
    free(map);
}

void create_room(Vector** tiles, SDL_Rect* room){

    for(int x = room->x; x < room->x + room->w; x++){

        for(int y = room->y; y < room->y + room->h; y++){

            tiles[x][y] = SPRITE_TILE_WALL;
        }
    }
}

Vector** map_generate_tiles(int width, int height, MapParams params, bool* success){

    Vector** tiles = malloc(sizeof(Vector*) * width);
    for(int x = 0; x < width; x++){

        tiles[x] = malloc(sizeof(Vector) * height);
        for(int y = 0; y < height; y++){

            tiles[x][y] = SPRITE_NONE;
        }
    }

    // Generate rooms
    SDL_Rect rooms[params.max_rooms];
    int room_count = 0;
    for(int r = 0; r < params.max_rooms; r++){

        int w = rand_range(params.room_min_size, params.room_max_size);
        int h = rand_range(params.room_min_size, params.room_max_size);
        int x = rand_range(0, width - w - 1);
        int y = rand_range(0, height - h - 1);

        SDL_Rect new_room = { .x = x, .y = y, .w = w, .h = h };

        // If the room overlaps with a previous room, don't create it
        bool overlaps_previous = false;
        for(int other_r = 0; other_r < room_count; other_r++){

            if(rect_intersects(&new_room, &rooms[other_r])){

                overlaps_previous = true;
                break;
            }
        }
        if(overlaps_previous){

            continue;
        }

        // Add the room to the map
        rooms[room_count] = new_room;
        create_room(tiles, &new_room);
        room_count++;

        // If the room is the first room we've created, don't do anything further
        if(room_count == 1){

            continue;
        }

        // Otherwise, create a tunnel connecting the new room to the previous room
        int room_index = room_count - 1;
        int previous_room = room_count - 2;

        Vector new_center = (Vector){
            .x = rooms[room_index].x + (rooms[room_index].w / 2),
            .y = rooms[room_index].y + (rooms[room_index].h / 2)
        };
        Vector previous_center = (Vector){
            .x = rooms[previous_room].x + (rooms[previous_room].w / 2),
            .y = rooms[previous_room].y + (rooms[previous_room].h / 2)
        };

        int min_x = min(new_center.x, previous_center.x);
        int max_x = max(new_center.x, previous_center.x);
        int min_y = min(new_center.y, previous_center.y);
        int max_y = max(new_center.y, previous_center.y);

        bool tunnel_horizontally = rand() % 2 == 0;
        if(tunnel_horizontally){

            create_room(tiles, &(SDL_Rect){

                .x = min_x,
                .y = previous_center.y,
                .w = max_x - min_x + 1,
                .h = 1
            });
            create_room(tiles, &(SDL_Rect){

                .x = new_center.x,
                .y = min_y,
                .w = 1,
                .h = max_y - min_y + 1
            });

        }else{

            create_room(tiles, &(SDL_Rect){

                .x = previous_center.x,
                .y = min_y,
                .w = 1,
                .h = max_y - min_y + 1
            });
            create_room(tiles, &(SDL_Rect){

                .x = min_x,
                .y = new_center.y,
                .w = max_x - min_x + 1,
                .h = 1
            });
        }
    }

    *success = room_count < params.min_rooms;

    return tiles;
}
