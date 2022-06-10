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

position player_position(map* ma){
    return ma->position_player;
}

int* playable_move(map* m){
    if(m->position_player.x <= MAP_HEIGHT && m->position_player.x >= 0){
        
    }
}

map* map_init()
{
    map *m = malloc(sizeof(map));
    m->position_player = position_init(0, 0);
    m->places = calloc(MAP_HEIGHT, sizeof(place*));
    int i,j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        if (i == 0 || i == MAP_HEIGHT - 1){
            m->places[i] = calloc(1, sizeof(place));
        }else{
            m->places[i] = calloc(MAP_WIDTH, sizeof(place));
        }
        for (j = 0; j < ((i==0||i==MAP_HEIGHT-1)?1:MAP_WIDTH); j++)
        {
            printf("taille %d \n", (int) sizeof(m->places[i]));
            m->places[i][j] = place_init(0, NULL, NULL);
        }
    }
    return m;
}

boolean check_map()
{   
    return true;
}

void move_player(map* m, int x, int y)
{
    if(check_map()){
        m->position_player = position_init(x,y);
        printf("position %d , %d", m->position_player.x, m->position_player.y);
    }
}

void map_print(map* m)
{
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
    map *m = map_init();
    map_print(m);
    move_player(m,1,4);
}
