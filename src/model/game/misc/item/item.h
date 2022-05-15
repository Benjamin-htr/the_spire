#ifndef ITEM
#define ITEM

#include "../effect/effect.h"

typedef struct
{
    char* name;
    effect* effects;
    char* description;
} item;

#endif