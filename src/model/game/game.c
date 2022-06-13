#include "./game.h"
#include "map/map.h"

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------
void initGame()
{
    game = malloc(sizeof(Game));
    game->mapData = map_init();
    map_print(game->mapData);
}