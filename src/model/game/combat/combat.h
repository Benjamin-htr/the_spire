#ifndef COMBAT
#define COMBAT

#include "../misc/board/board.h"
#include "../entity/entity.h"

typedef struct
{
    entity_t* caracter;
    entity_t* enemy;
    int caracterTurn;
}combat_t;


#endif