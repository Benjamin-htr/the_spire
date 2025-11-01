#include "./game.h"
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

        printf("free game \n");
        isLaunched = false;
    }
}