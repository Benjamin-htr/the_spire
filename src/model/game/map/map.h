#ifndef MAP
#define MAP

#include <stdlib.h>
#include "./place/place.h"
#include "map.h"

static const int MAP_HEIGHT = 10;
static const int MAP_WIDTH = 4;

typedef struct position
{
    int x;
    int y;
} position;

typedef struct _map
{
    place **places;
    position position_player;
    // struct _map *up;
    // struct _map *middle;
    // struct _map *down;
} map;

position position_init(int x, int y);
place map_get(map* map,int x, int y);
void map_init(int height, int width);
// map *newTree(double value, map *left, map *right);
// map *putToLeft(map *arbre, double value);
// map *putToMiddle(map *arbre, double value);
// map *putToRight(map *arbre, double value);
// void _localSort(map *arbre, double value);
// void _sortTree(map *arbre, map *sorted_arbre);
// map *initTreeWithRandom(size_t randRange);
#endif