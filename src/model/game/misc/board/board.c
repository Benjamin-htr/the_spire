#include <stdio.h>
#include "board.h"
#include <stdlib.h>

board_t * createBoard(deck_t * playerDeck){
    board_t * gameBoard = malloc(sizeof(board_t));
    gameBoard->cardDeck = playerDeck;
    gameBoard->abyss = createDeck(NULL);
    gameBoard->discardPile = createDeck(NULL);
    gameBoard->hand = createDeck(NULL);
    return gameBoard;
}

int drawCardsFromDeck(board_t * myBoard,int nbCardsDrew){ // returns number of cards drawed,
                                            //second parameter is the number of cards drew this turn
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
    firstPlace=  shuffleDeck(firstPlace);
    while(firstPlace!=NULL){
        card_t * cardToDraw = firstPlace->data;
        firstPlace = removeFirstCard(firstPlace);
       // firstPlace = firstPlace->next;
        addCard(secondPlace,cardToDraw);
    }
    return firstPlace;
}

board_t *  drawCards(board_t * myBoard){
    int nbCardsDrawed =   drawCardsFromDeck(myBoard,0);
    if(nbCardsDrawed<5){ // means no more cards are in the deck, lets try to fill the draw with discard
       myBoard->cardDeck=createDeck(NULL);
       myBoard->discardPile = putCardsFromOnePlaceToAnother(myBoard->discardPile,myBoard->cardDeck);
       nbCardsDrawed += drawCardsFromDeck(myBoard,nbCardsDrawed);
    }  
    return myBoard;
}


void testBoard(){
    deck_t * playerDeck = createDeck(importCardFromId(DEFENSE));
    deck_t * discard = createDeck(importCardFromId(PULVERIZE));
    addCard(discard,importCardFromId(FIREBALL));
    addCard(discard,importCardFromId(FOCUS));
    addCard(discard,importCardFromId(DODGE_A));
    addCard(playerDeck,importCardFromId(JAWURM_BACKSTAB));
    addCard(playerDeck,importCardFromId(JAWURM_CROUCH));
   /* addCard(playerDeck,importCardFromId(JAWURM_FIST));
    addCard(playerDeck,importCardFromId(ACCELERATION));
    addCard(playerDeck,importCardFromId(BLOUNI_JAB));
    addCard(playerDeck,importCardFromId(BLOUNI_KICK));   */
    board_t * gameBoard = createBoard(playerDeck);
    gameBoard->discardPile = discard;
    printf("Discard before draw:  \n");
    displayDeck(gameBoard->discardPile);
    printf("Deck before draw : ");
    displayDeck(gameBoard->cardDeck);
    gameBoard = drawCards(gameBoard);
    printf("Discard after draw \n");
    displayDeck(gameBoard->discardPile);
    printf("Deck after draw : ");
    displayDeck(gameBoard->cardDeck);
    printf("\n hand : \n");
    displayDeck(gameBoard->hand); 
}







