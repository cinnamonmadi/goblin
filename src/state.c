#include "state.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int SIDEBAR_INFO_LENGTH = 3;

State* state_init(){

    State* state = malloc(sizeof(State));

    state->sidebar_info = malloc(sizeof(char**) * SIDEBAR_INFO_LENGTH);
    for(int i = 0; i < SIDEBAR_INFO_LENGTH; i++){

        state->sidebar_info[i] = malloc(sizeof(char) * 64);
    }

    state->player_position = (Vector){
        .x = 2,
        .y = 2
    };
    state->player_sprite = SPRITE_GOBLIN;

    return state;
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
