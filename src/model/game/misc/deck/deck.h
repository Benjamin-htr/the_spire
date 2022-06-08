#ifndef DECK
#define DECK

#include "../../../misc/list/linkedList/simple_list.h"
#include "./card/card.h"

typedef simple_list deck;


deck * createDeck(card_t* card);

void addCard(deck * deck, card_t* card);

deck * removeFirstCard(deck* myDeck);

card_t * draw(deck * deck);

deck* getElementFromDeckAtIndex(int idx, deck * myDeck);

int size(deck * myDeck);

void  swapElements(deck * myDeck, int n, int m);

void displayDeck(deck * myDeck);

#endif