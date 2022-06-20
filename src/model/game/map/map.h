#ifndef MAP
#define MAP

#include <stdlib.h>
#include "./place/place.h"
#include "map.h"

extern int MAP_HEIGHT;
extern int MAP_WIDTH;

typedef struct _position
{
    int x;
    int y;
} position_player;

typedef struct _map
{
    place **places;
    position_player position_player;
} map;

void printtamere(char *chaine);
position_player position_init(int x, int y);
int map_get(map *map);
map *map_init();
void testMap();
void map_event(map *m);
void move_player(map *m, int y, boolean isTP);
int *playable_move(map *m);
position_player player_position(map *ma);
boolean check_map(map *m, int y);
void map_print(map *m);
void *teleporter(map *m);

// map *newTree(double value, map *left, map *right);
// map *putToLeft(map *arbre, double value);
// map *putToMiddle(map *arbre, double value);
// map *putToRight(map *arbre, double value);
// void _localSort(map *arbre, double value);
// void _sortTree(map *arbre, map *sorted_arbre);
// map *initTreeWithRandom(int randRange);
#endif