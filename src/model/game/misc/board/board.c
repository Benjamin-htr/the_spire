#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include "time.h"

const int NB_CARDS_TO_DRAW = 5;
board_t *createBoard(deck_t *playerDeck) // fonction qui créee un board à partir d'un deck, en lui allouant de la mémoire.
{
    board_t *gameBoard = malloc(sizeof(board_t));
    // WARNING: si tu fais ca et que tu pioche dans le deck tu vide le deck d'information stocker dans le player
    // il serait preferable de faire une copy du deck
    gameBoard->cardDeck = copyDeck(playerDeck);
    shuffleDeck(&gameBoard->cardDeck);
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

void tranferOneCardBetweenDeck(deck_t **firstPlace, deck_t **secondPlace, int cardIdx) // transfere une carte à un indice donné d'un deck à un autre
{
    card_t *moved = removeCardatIndex(firstPlace, cardIdx);
    if (moved != NULL)
    {
        addCard(*secondPlace, moved);
    }
}

void putCardsFromOnePlaceToAnother(deck_t **firstPlace, deck_t **secondPlace) // transfere toutes les cartes d'un deck vers un autre deck 
                                                                             // utilisé notamment pour déplacer toutes les cartes de la discard vers la main
{ 
    shuffleDeck(firstPlace);
    while ((*firstPlace) != NULL && (*firstPlace)->data != NULL)
    {
        tranferOneCardBetweenDeck(firstPlace, secondPlace, 0);
    }
}

int drawCardsFromDeck(board_t *myBoard, int nbCardsDrew) // pioche des cartes du deck, prend en paramètres le nombre de cartes piochées ce tour
{ 
    while (nbCardsDrew < NB_CARDS_TO_DRAW && myBoard->cardDeck != NULL)
    {
        tranferOneCardBetweenDeck(&myBoard->cardDeck, &myBoard->hand, 0);
        nbCardsDrew++;
    }
    return nbCardsDrew;
}

void drawCardsFromDeckWithRefillFromDiscard(board_t *myBoard) // pioche des cartes du deck et rempli la main dans le cas où il n'ya pas assez de cartes dans la main pour piocher
{ 
    int nbCardsDrawed = drawCardsFromDeck(myBoard, 0);
    if (nbCardsDrawed < NB_CARDS_TO_DRAW && getDeckSize(myBoard->discardPile) != 0)
    { // means no more cards are in the deck, lets try to fill the draw with discard
        myBoard->cardDeck = createDeck(NULL);
        putCardsFromOnePlaceToAnother(&myBoard->discardPile, &myBoard->cardDeck);
        drawCardsFromDeck(myBoard, nbCardsDrawed);
    }
}

void moveCardsFromHand(board_t *myBoard, boolean goToAbyss) // déplace les cartes de la main vers le cimetière/l'abysse
{ 
    if (myBoard->abyss == NULL)
    {
        myBoard->abyss = createDeck(NULL);
    }
    if (myBoard->discardPile == NULL)
    {
        myBoard->discardPile = createDeck(NULL);
    }
    // CRITICAL: Use pointer to myBoard->hand, not local copy
    while (myBoard->hand != NULL && myBoard->hand->data != NULL)
    {
        card_t *cardToRemove = removeFirstCard(&(myBoard->hand));
        if (cardToRemove == NULL)
        {
            break; // Safety: stop if removeFirstCard returns NULL
        }
        if (cardToRemove->isAbyssal && goToAbyss)
        {
            addCard(myBoard->abyss, cardToRemove);
        }
        else
        {
            addCard(myBoard->discardPile, cardToRemove);
        }
    }
    if (myBoard->hand == NULL)
    {
        myBoard->hand = createDeck(NULL); // initiate hand, because it is NULL at this point and can cause issues afterwards.
    }
}

deck_t *getRandomCardFromHand(board_t *board) // récupère une carte aléatoire  de la main
{
    int listSize = getDeckSize(board->hand);
    return getElementFromDeckAtIndex(rand() % listSize, board->hand);
}

void moveOneCardFromHand(board_t *board, card_t *cardToRemove) // retire une carte de la main et la déplace vers l'abysse/cimetière
{
    if (cardToRemove == NULL || cardToRemove->name == NULL)
    {
        return;
    }
    if (board->abyss == NULL)
    {
        board->abyss = createDeck(NULL);
    }
    if (board->discardPile == NULL)
    {
        board->discardPile = createDeck(NULL);
    }
    // Remove the card first, then route it based on isAbyssal
    card_t *removed = removeCard(&board->hand, cardToRemove->name);
    if (removed != NULL)
    {
        addCard((removed->isAbyssal) ? board->abyss : board->discardPile, removed);
    }
}

void moveOneCardFromHandByIdx(board_t *board, int idx) // déplace la carte de la main à l'indice idx vers le cimetière/ l'abysse 
{
    card_t *cardToRemove = getElementFromDeckAtIndex(idx, board->hand)->data;
    moveOneCardFromHand(board, cardToRemove);
}

void displayBoard(board_t *board) // affiche le board dans le terminal ( utilisé pour les tests)
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

void testBoard() // fonction pour tester différentes méthodes du board dans un terminal
{
    printf("\n==============================\n\tTEST BOARDY\n==============================\n");
    deck_t *playerDeck = createDeckFromArray((int[][2]){
                                                 {DEFENSE, 1},
                                                 {JAWURM_BACKSTAB, 1},
                                                 {JAWURM_CROUCH, 1},
                                             },
                                             3);
    deck_t *discard = createDeckFromArray((int[][2]){
                                              {PULVERIZE, 1},
                                              {FIREBALL, 1},
                                              {FOCUS, 1},
                                              {DODGE_A, 1},
                                          },
                                          4);

    printf("\nTEST INIT BOARD:\n________________\n");
    board_t *gameBoard = createBoard(playerDeck);
    // on doit free ici parce qu'on change le prointeur de deck de la discard pile
    freeDeckListAndCard(gameBoard->discardPile);
    gameBoard->discardPile = discard;
    displayBoard(gameBoard);
    printf("On a ajouté a la discard 4 cartes , la voici :  \n");
    displayDeck(gameBoard->discardPile);
    printf("Voici le deck, qui doit contenir 3 cartes  : \n ");
    displayDeck(gameBoard->cardDeck);

    // TEST DRAW CARD
    drawCardsFromDeckWithRefillFromDiscard(gameBoard);
    printf("Voici la discard, qui doit se retrouver vide puisqu'il n'y'a pas assez de cartes dans le deck pour piocher. \n");
    displayDeck(gameBoard->discardPile);
    printf("Voici notre deck, avec deux cartes de la discard : \n ");
    displayDeck(gameBoard->cardDeck);
    printf("\n Voici la main, qui doit contenir 5 cartes \n");
    displayDeck(gameBoard->hand);
    moveCardsFromHand(gameBoard, true);
    printf("\n La main après avoir jouée : ");
    displayDeck(gameBoard->hand);
    printf("\n La discard après après avoir vidé toute la main : \n");
    displayDeck(gameBoard->discardPile);
    addCard(gameBoard->hand, importCardFromId(SPECTRUM));
    addCard(gameBoard->hand, importCardFromId(PULVERIZE));
    addCard(gameBoard->hand, importCardFromId(CLAW_COMBO));
    addCard(gameBoard->hand, importCardFromId(DODGE_A));
    printf("On a rajouté des cartes à la main \n");
    displayDeck(gameBoard->hand);
    printf("Retirons ces cartes pour les mettre dans la défausse ou l'abysse \n");
    moveCardsFromHand(gameBoard, true);
    printf("Voici la main après avoir retiré les cartes \n");
    displayDeck(gameBoard->hand);
    printf("Voici l'abysse (il doit y avoir uniquement PULVERIZE) : \n");
    displayDeck(gameBoard->abyss);
    printf("Voici la discard, qui doit contenir 8 cartes : \n");
    displayDeck(gameBoard->discardPile);
    addCard(gameBoard->hand, importCardFromId(SPECTRUM));
    addCard(gameBoard->hand, importCardFromId(PULVERIZE));
    addCard(gameBoard->hand, importCardFromId(CLAW_COMBO));
    addCard(gameBoard->hand, importCardFromId(DODGE_A));
    printf("Nous avons rajouté 4 cartes à la main, retirons la 2eme ( pulverize ) \n");
    displayDeck(gameBoard->hand);
    moveOneCardFromHandByIdx(gameBoard, 1);
    displayDeck(gameBoard->hand);
    printf("Pulverize devrait être ajouté à l'abysse \n");
    printf("Voici l'abysse (il doit y avoir 2 PULVERIZE) : \n");
    displayDeck(gameBoard->abyss);
    card_t *randomCardFromHand = getRandomCardFromHand(gameBoard)->data;
    printf("Voici une random card de la main : \n");
    displayCard(randomCardFromHand);

    // FREE ACTION
    freeDeckListAndCard(playerDeck);
    freeBoard(gameBoard);
}
