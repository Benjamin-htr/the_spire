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

int drawCardsFromDeck(board_t *myBoard, int nbCardsDrew); // returns number of cards drawed,
                                                          // second parameter is the number of cards drew this turn
void testBoard();

deck_t *putCardsFromOnePlaceToAnother(deck_t *firstPlace, deck_t *secondPlace); // returns the firstPlace, which is empty.
                                                                                // don't forget to use this method this way : 'firstPlace = putCardsFromOnePlaceToAnother(firstPlace,secondPlace)'

board_t *drawCards(board_t *myBoard); // returns the board, please use "board = drawCards(board)

void moveOneCardFromHandByIdx(board_t *board, int idx);

void moveOneCardFromHand(board_t *board, card_t *cardToRemove);

void moveCardsFromHand(board_t *myBoard); // move all cards from hand to abyss/discard

deck_t *getRandomCardFromHand(board_t *board);
#endif