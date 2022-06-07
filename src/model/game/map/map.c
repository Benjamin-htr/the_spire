// #include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./place/event/event.h"
#include "./place/place.h"
#include "map.h"


extern int MAP_HEIGHT;
extern int MAP_WIDTH;

position position_init(int x, int y)
{
    position p;
    p.x = x;
    p.y = y;
    return p;
}

void map_init(int height, int width)
{
    printf("---------------------------------\n");
    map* m = malloc(sizeof(map));
    m->position_player = position_init(0, 0);
    m->places = calloc(height, sizeof(place*));
    printf("---------------------------------\n");
    int i,j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                printf("--------------------oe-------------\n");
                m->places[i][j] = place_init(0, NULL, NULL);
                printf("---------------------super------------\n");
            }
            else
            {
                // m->places[i][j] = (place) {.isEvent = 0, .eventData = NULL, .enemyData = NULL};
            }
        }
    }
}

void check_map(map* m, int x, int y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
    {
        printf("out of map\n");
    }
    else
    {
        printf("in map\n");
    }
}


void map_print(map* m)
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            printf("%d", m->places[i][j].isEvent);
        }
        printf("\n");
    }
}

//returns the event of the given tile
place map_get(map* map,int x, int y)
{
    return map->places[x][y];
}

int main(int argc, char const *argv[])
{
    printf("-------------------------t-----\n");
    map_init(10,10);
    printf("-------------------------a-----\n");
    return 0;
}
