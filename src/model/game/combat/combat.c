#include "combat.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"

combat_t * startCombat(entity_t* caracter, entity_t* enemy){
    combat_t* combat = malloc(sizeof(combat_t));
    combat->caracter = caracter;
    combat->enemy = enemy;
    combat->caracterTurn=1;
    combat->caracter->board = createBoard(combat->caracter->cardDeck);
    combat->enemy->board = createBoard(combat->enemy->cardDeck);
    combat->caracter->board->cardDeck=shuffleDeck(combat->caracter->board->cardDeck);
    combat->enemy->board->cardDeck=shuffleDeck(combat->enemy->board->cardDeck);
    return combat;
}


void playOnePlayerCard(combat_t* combat, card_t* cardToPlay){
    // stat_t* caracterMana = getStat(combat->caracter,MANA);
    // stat_t* caracterEnergy = getStat(combat->caracter,ENERGY);
    
    if(cardToPlay->energyCost < 1000 && cardToPlay->manaCost <1000){ // TODO : faire une fonction pour récupérer la valeur d'une stat
        applyCardEffect(cardToPlay,combat->caracter,combat->enemy);
        moveOneCardFromHand(combat->caracter->board,cardToPlay);
    }
    else {
        displayCard(*cardToPlay);
        printf("plus d'energie enculer \n");
    }
}

void playOneEnemyCard(combat_t * combat, card_t* cardToPlay){
      // stat_t* caracterMana = getStat(combat->enemy,MANA);
    // stat_t* caracterEnergy = getStat(combat->enemy,ENERGY);
    if(cardToPlay->energyCost < 1000 && cardToPlay->manaCost <1000){ // TODO : faire une fonction pour récupérer la valeur d'une stat
        applyCardEffect(cardToPlay,combat->enemy,combat->caracter);
        moveOneCardFromHand(combat->enemy->board,cardToPlay);
    }
    else {
         fflush(stdout);
         printf("plus d'energie enculer \n");
    }
}

void playCards(combat_t* combat){
    int i=0;
    board_t* boardToCheck ;
    if(combat->caracterTurn==1){
        boardToCheck=combat->caracter->board;
    }
    else {
        boardToCheck=combat->enemy->board;
    }
    card_t* card;
    while(size(boardToCheck->hand)>0) { // tant que le tour n'est pas fini 
    // (pour l'instant on considère qu'un tour est fini quand il n'y a plus de cartes)
        if(combat->caracterTurn==1){
            card = pickCardFromHand(combat->caracter->board->hand)->data;
            playOnePlayerCard(combat, card);
        }
        else {
            card = getRandomCardFromHand(combat->enemy->board)->data;
            playOneEnemyCard(combat, card);     
        }
        
        i++;
    }
    moveCardsFromHand(boardToCheck); // on déplace les cartes restantes de la main vers défausse/abysse
}


int getChoosenCardId(deck_t* hand){
    srand(time(NULL));
    hand->data = hand->data; // pour ce chien de warning 
    return rand()%size(hand); // pour l'instant on prend juste une random blc
}

// int getChoosenCardIdGUI(deck_t* hand){
    
// }

deck_t* pickCardFromHand(deck_t * hand){
    return getElementFromDeckAtIndex(getChoosenCardId(hand),hand);
}

void testCombat(){
    entity_t* player = importCaracterFromId(PETER);
    displayEntity(player);
     combat_t* combat = startCombat(player,player);
    startFight(combat);
}


void playTurn(combat_t* combat, board_t* board){
    board = drawCards(board);

    // POUR LES TESTS
    printf("Le deck après avoir pioché :  \n");
    displayDeck(board->cardDeck);
    fflush(stdout);
    printf("La main après avoir pioché : \n");
    displayDeck(board->hand);
    ///////////////


    playCards(combat);

  // POUR LES TESTS
    printf("Après avoir joué toutes les cartes, voici le deck : \n");
    displayDeck(board->cardDeck);
    printf("Voici la main : \n");
    displayDeck(board->hand);
    printf("Voici la défausse : \n");
    displayDeck(board->discardPile);
    printf("Voici l'abysse : \n");
    displayDeck(board->abyss);
    ///////////////////

    // FIN DE TOUR
    combat->caracterTurn= !combat->caracterTurn;
}

void startFight(combat_t* combat){
    int i =0;
    while(i<11){
        if(combat->caracterTurn==true){
            printf("------------------------TOUR %d ---------------- \n",i);
            playTurn(combat,combat->caracter->board);
            fflush(stdout);
        }
        else {
            printf("------------------------TOUR ennemi %d ---------------- \n",i);
            playTurn(combat,combat->enemy->board);
            fflush(stdout);
        }
        i++;
    }
}