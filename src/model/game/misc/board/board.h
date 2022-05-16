#ifndef BOARD
#define BOARD

#include "../deck/deck.h"

typedef struct
{
    deck* cardDeck;
    deck* hand;
    deck* discardPile;
    deck* abyss;
}board;


#endif