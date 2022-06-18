#include "./game.h"
#include "map/map.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------
void initGame()
{
    game = malloc(sizeof(Game));
    game->mapData = map_init();
    game->caracterData = importCaracterFromId(PETER);
    map_print(game->mapData);
}

void freeGame()
{
    if (game != NULL)
    {
        freeEntity(game->caracterData);
        free(game);
        game = {0};
        printf("free game \n");
    }
}