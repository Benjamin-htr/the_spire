#ifndef ITEM
#define ITEM

#include "../effect/effect.h"

typedef struct
{
    char* name;
    int manaCoast;
    int energyCost;
    effect* effects;
    char* technic;
    char* description;
} card;


#endif