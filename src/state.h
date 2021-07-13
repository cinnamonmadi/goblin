#pragma once

#include "global.h"

typedef struct State{
    Vector player_position;
    Vector player_sprite;
} State;

State* state_init();
