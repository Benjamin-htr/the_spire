#ifndef GAME
#define GAME

#include "entity/caracter/caracter.h"
#include "map/map.h"

typedef struct
{
    caracter *caracterData;
    map *mapData;
} Game;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern Game *game;

//----------------------------------------------------------------------------------
// Functions declarations
//----------------------------------------------------------------------------------
void initGame();

#endif