#ifndef ITEM
#define ITEM

#include "../effect/effect.h"

typedef struct
{
    char *name;
    effect_t *effects;
    char *description;
} item;

#endif