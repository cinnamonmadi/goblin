#include "state.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int SIDEBAR_INFO_LENGTH = 3;

const int CAMERA_BOX_TOP = (int)(VIEWPORT_HEIGHT * 0.3);
const int CAMERA_BOX_BOT = VIEWPORT_HEIGHT - CAMERA_BOX_TOP;
const int CAMERA_BOX_LEFT = (int)(VIEWPORT_WIDTH * 0.3);
const int CAMERA_BOX_RIGHT = VIEWPORT_WIDTH - CAMERA_BOX_LEFT;

Vector direction_vectors[4] = {
    (Vector){ .x = 0, .y = -1 },
    (Vector){ .x = 1, .y = 0 },
    (Vector){ .x = 0, .y = 1 },
    (Vector){ .x = -1, .y = 0 }
};

State* state_init(){

    State* state = malloc(sizeof(State));

    state->sidebar_info = malloc(sizeof(char**) * SIDEBAR_INFO_LENGTH);
    for(int i = 0; i < SIDEBAR_INFO_LENGTH; i++){

        state->sidebar_info[i] = malloc(sizeof(char) * 64);
    }

    state->map = map_init(60, 60, (MapParams){
        .max_rooms = 10,
        .min_rooms = 5,
        .room_min_size = 3,
        .room_max_size = 8
    });

    state->camera = (Vector){ .x = 0, .y = 0 };

    state->player_position = (Vector){
        .x = 2,
        .y = 2
    };
    state->player_sprite = SPRITE_GOBLIN;

    return state;
}

void state_free(State* state){

    map_free(state->map);
    free(state);
}

Vector state_map_at(State* state, int x, int y){

    Vector coord = vector_sum(state->camera, (Vector){ .x = x, .y = y });
    return state->map->tiles[coord.x][coord.y];
}

void state_update(State* state, int action){

    if(action == ACTION_NONE){

        return;
    }

    if(action >= ACTION_MOVE_UP && action <= ACTION_MOVE_LEFT){

        int direction = action - ACTION_MOVE_UP;
        state->camera = vector_sum(state->camera, direction_vectors[direction]);
    }

    state_update_camera(state);
}

void state_update_camera(State* state){

    /*
    if(state->player_position.y - state->camera.y < CAMERA_BOX_TOP){

        state->camera.y = state->player_position.y - CAMERA_BOX_TOP;

    }else if(state->player_position.y - state->camera.y > CAMERA_BOX_BOT){

        state->camera.y = state->player_position.y - CAMERA_BOX_BOT;
    }

    if(state->player_position.x - state->camera.x < CAMERA_BOX_LEFT){

        state->camera.x = state->player_position.x - CAMERA_BOX_LEFT;

    }else if(state->player_position.x - state->camera.x > CAMERA_BOX_RIGHT){

        state->camera.x = state->player_position.x - CAMERA_BOX_RIGHT;
    }
    */

    if(state->camera.x < 0){

        state->camera.x = 0;

    }else if(state->camera.x >= state->map->width - VIEWPORT_WIDTH){

        state->camera.x = state->map->width - VIEWPORT_WIDTH;
    }

    if(state->camera.y < 0){

        state->camera.y = 0;

    }else if(state->camera.y >= state->map->height - VIEWPORT_HEIGHT){

        state->camera.y = state->map->height - VIEWPORT_HEIGHT;
    }
}

void pad_string(char* dest, int length, char* left, char* right){

    int left_length = 0;
    int left_index = 0;
    while(left[left_index] != '\0'){

        if(left[left_index + 1] == '(' && (left[left_index] == 'r' || left[left_index] == 'y')){

            left_index += 2;
            continue;
        }
        left_index++;
        left_length++;
    }

    int right_length = 0;
    int right_index = 0;
    while(right[right_index] != '\0'){

        if(right[right_index + 1] == '(' && (right[right_index] == 'r' || right[right_index] == 'y')){

            right_index += 2;
            continue;
        }
        right_index++;
        right_length++;
    }

    int combined_length = left_length + right_length;
    if(combined_length > length){

        printf("Error! Tried to pad strings with too small a destination length. Left: %s Right: %s\n", left, right);
        return;
    }
    int padding_amount = length - combined_length;

    int dest_index = 0;
    left_index = 0;
    while(left[left_index] != '\0'){

        dest[dest_index] = left[left_index];
        left_index++;
        dest_index++;
    }
    for(int i = 0; i < padding_amount; i++){

        dest[dest_index] = ' ';
        dest_index++;
    }
    right_index = 0;
    while(right[right_index] != '\0'){

        dest[dest_index] = right[right_index];
        right_index++;
        dest_index++;
    }
    dest[dest_index] = '\0';
}

void state_update_sidebar_info(State* state){

    strcpy(state->sidebar_info[0], "Goblin Time");
    pad_string(state->sidebar_info[1], 21, "Health", "y(10/10)");
    strcpy(state->sidebar_info[2], "abcdefghijklmnopqrstuv");
}
