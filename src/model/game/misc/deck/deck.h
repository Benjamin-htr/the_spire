#ifndef DECK
#define DECK

#include "card/card.h"
typedef struct deck_in
{
    card_t *data;
    struct deck_in *next;
} deck_t;

deck_t *createDeck(card_t *card);

deck_t *createDeckFromArray(int cardIds[][2], int diffCardNumber);

void freeDeck(deck_t *deck);
void freeDeckListAndCard(deck_t *deckList);

void addCard(deck_t *deck, card_t *card);

card_t *removeFirstCard(deck_t **deck);
card_t *removeCard(deck_t **deck, char *cardName);
card_t *removeCardatIndex(deck_t **deck, int cardIdx);

card_t *draw(deck_t *deck);

void shuffleDeck(deck_t **deck);

deck_t *getElementFromDeckAtIndex(int idx, deck_t *myDeck);

int getDeckSize(deck_t *myDeck);

void swapElements(deck_t *myDeck, int n, int m);

void displayDeck(deck_t *myDeck);

void testDeck();
#endif