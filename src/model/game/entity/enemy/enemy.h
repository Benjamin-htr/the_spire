#ifndef ENEMY
#define ENEMY

#include "../entity.h"

typedef struct  {
    entity* core;
    int* habitatRange;
    int* hpRange;
}enemy;

#endif