#pragma once

#include "global.h"
#include "map.h"
#include "log.h"
#include "creature.h"

extern const int SIDEBAR_INFO_LENGTH;

typedef enum Action{
    ACTION_NONE = -1,
    ACTION_WAIT = 0,
    ACTION_MOVE_UP = 1,
    ACTION_MOVE_RIGHT = 2,
    ACTION_MOVE_DOWN = 3,
    ACTION_MOVE_LEFT = 4
} Action;

typedef struct State{
    char** sidebar_info;
    Log* log;
    Map* map;

    Vector camera;

    Creature player;
    Creature* enemies;
    int enemy_count;
} State;

State* state_init();
void state_free(State* state);

bool state_is_square_empty(State* state, Vector position);

Sprite state_map_at(State* state, int x, int y);

void state_update(State* state, Action action);
void state_update_camera(State* state);

void state_creature_attempt_move(State* state, Creature* creature, int direction);

void state_update_sidebar_info(State* state);
