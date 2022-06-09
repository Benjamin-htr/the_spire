#include <string.h>
#include <stdio.h>
#include "./place/event/event.h"
#include "./place/place.h"
#include "map.h"

int MAP_HEIGHT = 10;
int MAP_WIDTH = 4;

position position_init(int x, int y)
{
    position p;
    p.x = x;
    p.y = y;
    return p;
}

void printtamere(char * chaine){
    printf("---------------------------------\n TA MERE %s\n---------------------------------\n", chaine);
    fflush(stdout);
}
map* map_init()
{
    printtamere("1");
    map *m = malloc(sizeof(map));
    printtamere("2");
    m->position_player = position_init(0, 0);
    m->places = calloc(MAP_HEIGHT, sizeof(place*));
    printtamere("3");
    int i,j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        if (i == 0 || i == MAP_HEIGHT - 1){
            printtamere("6");
            m->places[i] = calloc(1, sizeof(place));
        }else{
            printtamere("7");
            m->places[i] = calloc(MAP_WIDTH, sizeof(place));
        }
        for (j = 0; j < ((i==0||i==MAP_HEIGHT-1)?1:MAP_WIDTH); j++)
        {
            printtamere("4 ");
            printf("taille %d \n", (int) sizeof(m->places[i]));
            m->places[i][j] = place_init(0, NULL, NULL);
        }
    }
    printtamere("9");
    return m;
}

void check_map(map* m, int x, int y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
    {
        m->position_player = position_init(0, 0);
        printf("out of map\n");
    }
    else
    {
        printf("in map\n");
    }
}


void map_print(map* m)
{
    printtamere("test");
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < ((i==0||i==MAP_HEIGHT-1)?1:MAP_WIDTH); j++)
        {
            printf("%d", m->places[i][j].isEvent);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
}

//returns the event of the given tile
place map_get(map* map,int x, int y)
{
    return map->places[x][y];
}

void testMap()
{
    printf("-------------------------t-----\n");
    map *m = map_init();
    printf("-------------------------a-----\n");
    map_print(m);
}
