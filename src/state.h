#pragma once

#include "global.h"

extern const int SIDEBAR_INFO_LENGTH;

typedef struct State{
    char** sidebar_info;

    Vector player_position;
    Vector player_sprite;
} State;

State* state_init();
void state_update_sidebar_info(State* state);
