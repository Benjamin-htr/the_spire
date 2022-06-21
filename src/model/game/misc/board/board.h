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

board_t *createBoard(deck_t *playerDeck);

void freeBoard(board_t *board);

void tranferOneCardBetweenDeck(deck_t **firstPlace, deck_t **secondPlace, int cardIdx);
void putCardsFromOnePlaceToAnother(deck_t **firstPlace, deck_t **secondPlace); // returns the firstPlace, which is empty.
                                                                               // don't forget to use this method this way : 'firstPlace = putCardsFromOnePlaceToAnother(firstPlace,secondPlace)'

void drawCardsFromDeckWithRefillFromDiscard(board_t *myBoard);
int drawCardsFromDeck(board_t *myBoard, int nbCardsDrew); // returns number of cards drawed,
                                                          // second parameter is the number of cards drew this turn
void moveOneCardFromHandByIdx(board_t *board, int idx);

void moveOneCardFromHand(board_t *board, card_t *cardToRemove);

void moveCardsFromHand(board_t *myBoard, boolean goToAbyss); // move all cards from hand to abyss/discard

deck_t *getRandomCardFromHand(board_t *board);

void displayBoard(board_t *board);

void testBoard();
#endif