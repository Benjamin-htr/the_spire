#ifndef ITEM
#define ITEM

#include "../../effect/effect.h"

enum rarityE{
    COMMON,
    BASIC,
    ATYPIC,
    RARE,
};

typedef struct
{
    char* name;
    enum rarityE rarity;
    int manaCost;
    int energyCost;
    effect* effects;
    char* technic;
    char* description;
} card;

card * createCard(char* name,enum rarityE rarity, int manaCost, int energyCost , effect* effects, char* technic,char* description);


#endif