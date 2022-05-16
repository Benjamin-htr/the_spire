#ifndef ENTITY
#define ENTITY

#include "../misc/deck/deck.h"
#include "../misc/effect/effect.h"
#include "../misc/stat/stat.h"
#include "../misc/board/board.h"

typedef struct{
    char* name;
    deck* cardDeck;
    effect* effects;
    stat* stats; /* HP,DODGE(max 999),STR(max 999),Mana,Energy */
    board* board;
}entity;


#endif