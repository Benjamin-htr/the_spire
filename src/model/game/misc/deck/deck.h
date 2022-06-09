#ifndef DECK
#define DECK

#include "../../../misc/list/linkedList/simple_list.h"
#include "card/card.h"
typedef simple_list deck_t;

deck_t *createDeck(card_t *card);

void addCard(deck_t *deck, card_t *card);

deck_t *removeFirstCard(deck_t *myDeck);

card_t *draw(deck_t *deck);

deck_t *getElementFromDeckAtIndex(int idx, deck_t *myDeck);

int size(deck_t *myDeck);

void swapElements(deck_t *myDeck, int n, int m);

void displayDeck(deck_t *myDeck);

#endif