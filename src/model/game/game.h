#ifndef GAME
#define GAME

#include "map/map.h"

typedef struct
{
    entity_t *caracterData;
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