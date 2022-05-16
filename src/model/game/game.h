#ifndef GAME
#define GAME

#include "entity/caracter/caracter.h"
#include "map/map.h"

typedef struct
{
    caracter* caracterData;
    map* mapData;
}game;


#endif