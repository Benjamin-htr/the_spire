#ifndef MAP
#define MAP

#include <stdlib.h>
#include "place/place.h"

typedef struct _map
{
    place *place;
    struct _map *up;
    struct _map *middle;
    struct _map *down;
} map;

map *newTree(double value, map *left, map *right);
map *putToLeft(map *arbre, double value);
map *putToMiddle(map *arbre, double value);
map *putToRight(map *arbre, double value);
void _localSort(map *arbre, double value);
void _sortTree(map *arbre, map *sorted_arbre);
map *initTreeWithRandom(size_t randRange);
#endif