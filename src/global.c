#include "global.h"

#include <stdlib.h>

Vector vector_sum(Vector a, Vector b){

    return (Vector){ .x = a.x + b.x, .y = a.y + b.y };
}

Vector vector_minus(Vector a, Vector b){

    return (Vector){ .x = a.x - b.x, .y = a.y - b.y };
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

