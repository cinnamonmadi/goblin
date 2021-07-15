#pragma once

#include "global.h"
#include "map.h"

extern const int SIDEBAR_INFO_LENGTH;

static const int ACTION_NONE = -1;
static const int ACTION_WAIT = 0;
static const int ACTION_MOVE_UP = 1;
static const int ACTION_MOVE_RIGHT = 2;
static const int ACTION_MOVE_DOWN = 3;
static const int ACTION_MOVE_LEFT = 4;

typedef struct State{
    char** sidebar_info;

    Map* map;

    Vector camera;

    Vector player_position;
    Vector player_sprite;
} State;

State* state_init();
void state_free(State* state);

Vector state_map_at(State* state, int x, int y);

void state_update(State* state, int action);
void state_update_camera(State* state);

void state_update_sidebar_info(State* state);
