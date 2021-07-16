#pragma once

#include "global.h"

typedef struct Creature{
    char* name;
    Sprite sprite;
    Vector position;
    int health;
    int max_health;
    int low_attack;
    int high_attack;
    int defense;
    int speed;
} Creature;

static const Creature NEW_PLAYER = {
    .name = "player",
    .sprite = SPRITE_GOBLIN,
    .position = (Vector){ .x = 0, .y = 0 },
    .health = 100,
    .max_health = 100,
    .low_attack = 5,
    .high_attack = 7,
    .defense = 5,
    .speed = 5
};

static const Creature NEW_WASP = {
    .name = "wasp",
    .sprite = SPRITE_WASP,
    .position = (Vector){ .x = 0, .y = 0 },
    .health = 20,
    .max_health = 20,
    .low_attack = 5,
    .high_attack = 7,
    .defense = 5,
    .speed = 5
};

void creature_attempt_move(Creature* creature, int direction, bool** walls);
