#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include "time.h"

const int NB_CARDS_TO_DRAW = 5;
board_t * createBoard(deck_t * playerDeck){
    board_t * gameBoard = malloc(sizeof(board_t));
    gameBoard->cardDeck = playerDeck;
    gameBoard->abyss = createDeck(NULL);
    gameBoard->discardPile = createDeck(NULL);
    gameBoard->hand = createDeck(NULL);
    return gameBoard;
}

int drawCardsFromDeck(board_t * myBoard,int nbCardsDrew){ // returns number of cards drawed,
                                            //second parameter is the number of cards drew this turn*
    while(nbCardsDrew <5 && myBoard->cardDeck != NULL){
        card_t * cardToDraw = myBoard->cardDeck->data;
        removeFirstCard(myBoard->cardDeck);
        myBoard->cardDeck = myBoard->cardDeck->next;
        addCard(myBoard->hand,cardToDraw);
        nbCardsDrew++;
   } 
    return nbCardsDrew;
}

deck_t * putCardsFromOnePlaceToAnother(deck_t* firstPlace, deck_t* secondPlace){  // returns the firstPlace, which is empty.
    // don't forget to use this method this way : 'firstPlace = putCardsFromOnePlaceToAnother(firstPlace,secondPlace)'
    firstPlace=  shuffleDeck(firstPlace);
    while(firstPlace!=NULL && firstPlace->data!=NULL){
        card_t * cardToDraw = firstPlace->data;
        firstPlace = removeFirstCard(firstPlace);
        addCard(secondPlace,cardToDraw);
    }
    return firstPlace;
}

board_t *  drawCards(board_t * myBoard){ // returns the board, please use "board = drawCards(board) 
    int nbCardsDrawed =   drawCardsFromDeck(myBoard,0);
    if(nbCardsDrawed<NB_CARDS_TO_DRAW){ // means no more cards are in the deck, lets try to fill the draw with discard
        myBoard->cardDeck = createDeck(NULL);
        myBoard->discardPile = putCardsFromOnePlaceToAnother(myBoard->discardPile,myBoard->cardDeck);
        nbCardsDrawed += drawCardsFromDeck(myBoard,nbCardsDrawed);
    }  
    return myBoard;
}

void moveCardsFromHand(board_t * myBoard){ // move all cards from hand to abyss/discard 
    deck_t * hand = myBoard->hand;
    if(myBoard->abyss==NULL){
        myBoard->abyss = createDeck(NULL);
    }
    if(myBoard->discardPile==NULL){
        myBoard->discardPile = createDeck(NULL);
    }
    while(hand!=NULL && hand->data !=NULL){
        card_t * cardToRemove = hand->data;
        hand = removeFirstCard(hand);
        if(cardToRemove->isAbyssal){
           addCard(myBoard->abyss,cardToRemove);
        }
        else {
           addCard(myBoard->discardPile,cardToRemove);
        }
    }
    myBoard ->hand = createDeck(NULL); // initiate hand, because it is NULL at this point and can cause issues afterwards.
}

deck_t * getRandomCardFromHand(board_t* board){
    srand(time(NULL));
    int listSize = size(board->hand);
    return getElementFromDeckAtIndex(rand() %listSize, board->hand);
}

void moveOneCardFromHand(board_t * board, card_t * cardToRemove){
     if(cardToRemove != NULL){
        if(board->abyss==NULL){
            board->abyss = createDeck(NULL);
        }
        if(board->discardPile==NULL){
            board->discardPile = createDeck(NULL);
        }
        removeCard(&board->hand,cardToRemove->name);
        if(cardToRemove->isAbyssal){
           addCard(board->abyss,cardToRemove);
        }
        else {
           addCard(board->discardPile,cardToRemove);
        }
    }
}

void moveOneCardFromHandByIdx(board_t * board,int idx){ 
    card_t* cardToRemove =  getElementFromDeckAtIndex(idx,board->hand)->data;
    moveOneCardFromHand(board,cardToRemove);
}


void testBoard(){
    deck_t * playerDeck = createDeck(importCardFromId(DEFENSE));
    deck_t * discard = createDeck(importCardFromId(PULVERIZE));
    addCard(discard,importCardFromId(FIREBALL));
    addCard(discard,importCardFromId(FOCUS));
    addCard(discard,importCardFromId(DODGE_A));
    addCard(playerDeck,importCardFromId(JAWURM_BACKSTAB));
    addCard(playerDeck,importCardFromId(JAWURM_CROUCH));
    board_t * gameBoard = createBoard(playerDeck);
    gameBoard->discardPile = discard;
    printf("On a ajouté a la discard 4 cartes , la voici :  \n");
    displayDeck(gameBoard->discardPile);
    printf("Voici le deck, qui doit contenir 3 cartes  : \n ");
    displayDeck(gameBoard->cardDeck);
    gameBoard = drawCards(gameBoard);
    printf("Voici la discard, qui doit se retrouver vide puisqu'il n'y'a pas assez de cartes dans le deck pour piocher. \n");
    displayDeck(gameBoard->discardPile);
    printf("Voici notre deck, avec deux cartes de la discard : \n ");
    displayDeck(gameBoard->cardDeck);
    printf("\n Voici la main, qui doit contenir 5 cartes \n");
    displayDeck(gameBoard->hand); 
    moveCardsFromHand(gameBoard);
    printf("\n La main après avoir jouée : ");
    displayDeck(gameBoard->hand);
    printf("\n La discard après après avoir vidé toute la main : \n");
    displayDeck(gameBoard->discardPile);
    addCard(gameBoard->hand,importCardFromId(SPECTRUM));
    addCard(gameBoard->hand,importCardFromId(PULVERIZE));
    addCard(gameBoard->hand,importCardFromId(CLAW_COMBO));
    addCard(gameBoard->hand,importCardFromId(DODGE_A)); 
    printf("On a rajouté des cartes à la main \n");
    displayDeck(gameBoard->hand);
    printf("Retirons ces cartes pour les mettre dans la défausse ou l'abysse \n");
    moveCardsFromHand(gameBoard);
    printf("Voici la main après avoir retiré les cartes \n");
    displayDeck(gameBoard->hand);
    printf("Voici l'abysse (il doit y avoir uniquement PULVERIZE) : \n");
    displayDeck(gameBoard->abyss);
    printf("Voici la discard, qui doit contenir 8 cartes : \n");
    displayDeck(gameBoard->discardPile);
    addCard(gameBoard->hand,importCardFromId(SPECTRUM));
    addCard(gameBoard->hand,importCardFromId(PULVERIZE));
    addCard(gameBoard->hand,importCardFromId(CLAW_COMBO));
    addCard(gameBoard->hand,importCardFromId(DODGE_A)); 
    printf("Nous avons rajouté 4 cartes à la main, retirons la 2eme ( pulverize ) \n");
    moveOneCardFromHandByIdx(gameBoard,1);
    displayDeck(gameBoard->hand);
    printf("Pulverize devrait être ajouté à l'abysse \n");
    printf("Voici l'abysse (il doit y avoir 2 PULVERIZE) : \n");
    displayDeck(gameBoard->abyss);
    card_t* randomCardFromHand = getRandomCardFromHand(gameBoard)->data;
    printf("Voici une random card de la main : \n");
    displayCard(*randomCardFromHand);
}







