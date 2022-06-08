#include "card.h"
#include "card-global.h"
#include "stdlib.h"
#include "stdio.h"

card * createCard(char* name,enum rarityE rarity, int manaCost, int energyCost , effect* effects, char* technic,char* description){
    card* res = malloc(sizeof(card));
    res->name = name;
    res->rarity=rarity;
    res->manaCost=manaCost;
    res->energyCost =energyCost;
    res->effects=effects;
    res->technic=technic;
    res->description=description;
    return res;
}


