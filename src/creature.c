#include "creature.h"

void creature_attempt_move(Creature* creature, int direction, bool** walls){

    Vector attempt_position = vector_sum(creature->position, DIRECTION_VECTORS[direction]);
    if(!walls[attempt_position.x][attempt_position.y]){

        creature->position = attempt_position;
    }
}
