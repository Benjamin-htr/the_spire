#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include "time.h"

const int NB_CARDS_TO_DRAW = 5;
board_t *createBoard(deck_t *playerDeck)
{
    board_t *gameBoard = malloc(sizeof(board_t));
    // WARNING: si tu fais ca et que tu pioche dans le deck tu vide le deck d'information stocker dans le player
    // il serait preferable de faire une copy du deck
    gameBoard->cardDeck = playerDeck;
    gameBoard->abyss = createDeck(NULL);
    gameBoard->discardPile = createDeck(NULL);
    gameBoard->hand = createDeck(NULL);
    return gameBoard;
}

// Cette fonction libere la memoire du board passé en parametre ainsi que tout les decks qu'il contient et toutes les cartes qu'ils contiennent
void freeBoard(board_t *board)
{
    freeDeckListAndCard(board->abyss);
    freeDeckListAndCard(board->cardDeck);
    freeDeckListAndCard(board->discardPile);
    freeDeckListAndCard(board->hand);
    free(board);
}

int drawCardsFromDeck(board_t *myBoard, int nbCardsDrew)
{ // returns number of cards drawed,
    // second parameter is the number of cards drew this turn*
    while (nbCardsDrew < NB_CARDS_TO_DRAW && myBoard->cardDeck != NULL)
    {
        addCard(myBoard->hand, removeFirstCard(&(myBoard->cardDeck)));
        nbCardsDrew++;
    }
    return nbCardsDrew;
}

void putCardsFromOnePlaceToAnother(deck_t *firstPlace, deck_t *secondPlace)
{ // returns the firstPlace, which is empty.
    // don't forget to use this method this way : 'firstPlace = putCardsFromOnePlaceToAnother(firstPlace,secondPlace)'
    shuffleDeck(&firstPlace);
    while (firstPlace != NULL && firstPlace->data != NULL)
    {
        addCard(secondPlace, removeFirstCard(&(firstPlace)));
    }
}

board_t *drawCards(board_t *myBoard)
{ // returns the board, please use "board = drawCards(board)
    int nbCardsDrawed = drawCardsFromDeck(myBoard, 0);
    if (nbCardsDrawed < NB_CARDS_TO_DRAW)
    { // means no more cards are in the deck, lets try to fill the draw with discard
        myBoard->cardDeck = createDeck(NULL);
        putCardsFromOnePlaceToAnother(myBoard->discardPile, myBoard->cardDeck);
        nbCardsDrawed += drawCardsFromDeck(myBoard, nbCardsDrawed);
    }
    return myBoard;
}

void moveCardsFromHand(board_t *myBoard)
{ // move all cards from hand to abyss/discard
    deck_t *hand = myBoard->hand;
    if (myBoard->abyss == NULL)
    {
        myBoard->abyss = createDeck(NULL);
    }
    if (myBoard->discardPile == NULL)
    {
        myBoard->discardPile = createDeck(NULL);
    }
    while (hand != NULL && hand->data != NULL)
    {
        // Pourquoi ne pas appeler moveOneCardFromHand tu dupplique ton code
        card_t *cardToRemove = removeFirstCard(&(hand));
        if (cardToRemove->isAbyssal)
        {
            addCard(myBoard->abyss, cardToRemove);
        }
        else
        {
            addCard(myBoard->discardPile, cardToRemove);
        }
    }
    myBoard->hand = createDeck(NULL); // initiate hand, because it is NULL at this point and can cause issues afterwards.
}

deck_t *getRandomCardFromHand(board_t *board)
{
    // ne serait-il pas préférable de retourner directement la carte (à voir les utilisations faites)
    int listSize = getDeckSize(board->hand);
    return getElementFromDeckAtIndex(rand() % listSize, board->hand);
}

void moveOneCardFromHand(board_t *board, card_t *cardToRemove)
{
    if (cardToRemove != NULL)
    {
        if (board->abyss == NULL)
        {
            board->abyss = createDeck(NULL);
        }
        if (board->discardPile == NULL)
        {
            board->discardPile = createDeck(NULL);
        }
        removeCard(&board->hand, cardToRemove->name);
        if (cardToRemove->isAbyssal)
        {
            addCard(board->abyss, cardToRemove);
        }
        else
        {
            addCard(board->discardPile, cardToRemove);
        }
    }
}

void moveOneCardFromHandByIdx(board_t *board, int idx)
{
    card_t *cardToRemove = getElementFromDeckAtIndex(idx, board->hand)->data;
    moveOneCardFromHand(board, cardToRemove);
}

void displayBoard(board_t *board)
{
    printf("\nBOARD:\n______\n");
    printf("\nDECK:");
    displayDeck(board->cardDeck);
    printf("\nHAND:");
    displayDeck(board->hand);
    printf("\nDISCARD:");
    displayDeck(board->discardPile);
    printf("\nABYSS:");
    displayDeck(board->abyss);
}

void testBoard()
{
    printf("\n==============================\n\tTEST BOARDY\n==============================\n");
    // TO FREE
    deck_t *playerDeck = createDeck(importCardFromId(DEFENSE));
    // TO FREE
    deck_t *discard = createDeckFromArray((int[][2]){
                                              {PULVERIZE, 1},
                                              {FIREBALL, 1},
                                              {FOCUS, 1},
                                              {DODGE_A, 1},
                                              {JAWURM_BACKSTAB, 1},
                                              {JAWURM_CROUCH, 1},
                                          },
                                          6);
    board_t *testBoard = createBoard(playerDeck);
    freeDeckListAndCard(testBoard->discardPile);
    testBoard->discardPile = discard;
    displayBoard(testBoard);
    // printf("On a ajouté a la discard 4 cartes , la voici :  \n");
    // displayDeck(testBoard->discardPile);
    // printf("Voici le deck, qui doit contenir 3 cartes  : \n ");
    // displayDeck(testBoard->cardDeck);
    // testBoard = drawCards(testBoard);
    // printf("Voici la discard, qui doit se retrouver vide puisqu'il n'y'a pas assez de cartes dans le deck pour piocher. \n");
    // displayDeck(testBoard->discardPile);
    // printf("Voici notre deck, avec deux cartes de la discard : \n ");
    // displayDeck(testBoard->cardDeck);
    // printf("\n Voici la main, qui doit contenir 5 cartes \n");
    // displayDeck(testBoard->hand);
    // moveCardsFromHand(testBoard);
    // printf("\n La main après avoir jouée : ");
    // displayDeck(testBoard->hand);
    // printf("\n La discard après après avoir vidé toute la main : \n");
    // displayDeck(testBoard->discardPile);
    // addCard(testBoard->hand, importCardFromId(SPECTRUM));
    // addCard(testBoard->hand, importCardFromId(PULVERIZE));
    // addCard(testBoard->hand, importCardFromId(CLAW_COMBO));
    // addCard(testBoard->hand, importCardFromId(DODGE_A));
    // printf("On a rajouté des cartes à la main \n");
    // displayDeck(testBoard->hand);
    // printf("Retirons ces cartes pour les mettre dans la défausse ou l'abysse \n");
    // moveCardsFromHand(testBoard);
    // printf("Voici la main après avoir retiré les cartes \n");
    // displayDeck(testBoard->hand);
    // printf("Voici l'abysse (il doit y avoir uniquement PULVERIZE) : \n");
    // displayDeck(testBoard->abyss);
    // printf("Voici la discard, qui doit contenir 8 cartes : \n");
    // displayDeck(testBoard->discardPile);
    // addCard(testBoard->hand, importCardFromId(SPECTRUM));
    // addCard(testBoard->hand, importCardFromId(PULVERIZE));
    // addCard(testBoard->hand, importCardFromId(CLAW_COMBO));
    // addCard(testBoard->hand, importCardFromId(DODGE_A));
    // printf("Nous avons rajouté 4 cartes à la main, retirons la 2eme ( pulverize ) \n");
    // moveOneCardFromHandByIdx(testBoard, 1);
    // displayDeck(testBoard->hand);
    // printf("Pulverize devrait être ajouté à l'abysse \n");
    // printf("Voici l'abysse (il doit y avoir 2 PULVERIZE) : \n");
    // displayDeck(testBoard->abyss);
    // card_t *randomCardFromHand = getRandomCardFromHand(testBoard)->data;
    // printf("Voici une random card de la main : \n");
    // displayCard(*randomCardFromHand);
}
