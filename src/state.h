#pragma once

#include "global.h"
#include "map.h"

extern const int SIDEBAR_INFO_LENGTH;

typedef struct State{
    char** sidebar_info;

    Map* map;

    Vector player_position;
    Vector player_sprite;
} State;

State* state_init();
void state_free(State* state);
void state_update_sidebar_info(State* state);
