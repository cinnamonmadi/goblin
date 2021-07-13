#include "state.h"

#include <stdlib.h>

State* state_init(){

    State* state = malloc(sizeof(State));

    state->player_position = (Vector){
        .x = 2,
        .y = 2
    };
    state->player_sprite = SPRITE_GOBLIN;

    return state;
}
