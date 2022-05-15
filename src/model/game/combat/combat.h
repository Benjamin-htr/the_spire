#ifndef COMBAT
#define COMBAT

#include "../entity/caracter/caracter.h"
#include "../entity/enemy/enemy.h"

typedef struct
{
    caracter* caracterSlot;
    enemy* enemySlot;
}combat;


#endif