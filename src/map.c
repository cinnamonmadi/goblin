#include "map.h"

#include <stdlib.h>

int rand_range(int min, int max){

    int range = max - min;
    int num = (rand() % range) + 1 + min;

    return num;
}

bool rect_intersects(SDL_Rect* a, SDL_Rect* b){
    return !(a->x + a->w <= b->x || b->x + b->w <= a->x || a->y + a->h <= b->y || b->y + b->h <= a->y);
}

int min(int a, int b){

    if(a <= b){

        return a;

    }else{

        return b;
    }
}

int max(int a, int b){

    if(a >= b){

        return a;

    }else{

        return b;
    }
}

void create_room(bool** map, SDL_Rect* room){

    for(int x = room->x; x < room->x + room->w; x++){

        for(int y = room->y; y < room->y + room->h; y++){

            map[x][y] = false;
        }
    }
}

bool** map_generate(int width, int height){

    bool** map = malloc(sizeof(bool*) * width);
    for(int x = 0; x < width; x++){

        map[x] = malloc(sizeof(bool) * height);
        for(int y = 0; y < height; y++){

            map[x][y] = true;
        }
    }

    // Generate rooms
    int max_rooms = 30;
    int room_min_size = 6;
    int room_max_size = 15;

    SDL_Rect rooms[max_rooms];
    int room_count = 0;
    for(int r = 0; r < max_rooms; r++){

        int w = rand_range(room_min_size, room_max_size);
        int h = rand_range(room_min_size, room_max_size);
        int x = rand_range(0, width - w - 1);
        int y = rand_range(0, height - h - 1);

        SDL_Rect new_room = { .x = x, .y = y, .w = w, .h = h };

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

        rooms[room_count] = new_room;
        create_room(map, &new_room);
        room_count++;

        if(room_count == 1){

            continue;
        }

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

            create_room(map, &(SDL_Rect){

                .x = min_x,
                .y = previous_center.y,
                // .w = rand_range(1, max_x - min_x),
                .w = max_x - min_x + 1,
                .h = 1
            });
            create_room(map, &(SDL_Rect){

                .x = new_center.x,
                .y = min_y,
                .w = 1,
                // .h = rand_range(1, max_y - min_y)
                .h = max_y - min_y + 1
            });

        }else{

            create_room(map, &(SDL_Rect){

                .x = previous_center.x,
                .y = min_y,
                .w = 1,
                // .h = rand_range(1, max_y - min_y)
                .h = max_y - min_y + 1
            });
            create_room(map, &(SDL_Rect){

                .x = min_x,
                .y = new_center.y,
                // .w = rand_range(1, max_x - min_x),
                .w = max_x - min_x + 1,
                .h = 1
            });
        }
    }

    return map;
}
