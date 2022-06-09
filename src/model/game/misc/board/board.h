#ifndef BOARD
#define BOARD

#include "../deck/deck.h"
#include "../deck/card/card.h"



typedef struct
{
    deck_t *cardDeck;
    deck_t *hand;
    deck_t *discardPile;
    deck_t *abyss;
} board_t;


void testBoard();

#endif