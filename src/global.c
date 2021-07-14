#include "global.h"

#include <stdlib.h>

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

