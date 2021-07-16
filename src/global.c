#include "global.h"

#include <stdlib.h>

const Vector SPRITE_VECTORS[SPRITE_COUNT] = {
    (Vector){ .x = 40, .y = 2 },
    (Vector){ .x = 3, .y = 14 },
    (Vector){ .x = 3, .y = 3 },
    (Vector){ .x = 1, .y = 2 }
};

bool vector_equal(Vector a, Vector b){

    return a.x == b.x && a.y == b.y;
}

Vector vector_sum(Vector a, Vector b){

    return (Vector){ .x = a.x + b.x, .y = a.y + b.y };
}

Vector vector_minus(Vector a, Vector b){

    return (Vector){ .x = a.x - b.x, .y = a.y - b.y };
}

Vector vector_increment(Vector a, int direction){

    return (Vector){ .x = a.x + DIRECTION_VECTORS[direction].x, .y = a.y + DIRECTION_VECTORS[direction].y };
}

int min(int a, int b){

    if(a <= b){

        return a;

    }else{

        return b;
    }
}

int max(int a, int b){

    if(a >= b){

        return a;

    }else{

        return b;
    }
}

bool rect_intersects(SDL_Rect* a, SDL_Rect* b){

    return !(a->x + a->w <= b->x || b->x + b->w <= a->x || a->y + a->h <= b->y || b->y + b->h <= a->y);
}

int rand_range(int min, int max){

    int range = max - min;
    int num = (rand() % range) + 1 + min;

    return num;
}

