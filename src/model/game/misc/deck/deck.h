#ifndef DECK
#define DECK

#include "../../../misc/list/linkedList/simple_list.h"
#include "./card/card.h"

typedef simple_list deck;


deck * createDeck(card* card);

void addCard(deck * deck, card* card);

deck * removeFirstCard(deck* myDeck);

card * draw(deck * deck);

deck* getElementFromDeckAtIndex(int idx, deck * myDeck);

int size(deck * myDeck);

void  swapElements(deck * myDeck, int n, int m);

void displayDeck(deck * myDeck);

#endif