#include "combat.h"
#include <stdlib.h>
#include <stdio.h>

combat_t * startCombat(entity_t* caracter, entity_t* enemy){
    combat_t* combat = malloc(sizeof(combat_t));
    combat->caracter = caracter;
    combat->enemy = enemy;
    combat->caracterTurn=1;
    combat->caracter->board = createBoard(combat->caracter->cardDeck);
    combat->enemy->board = createBoard(combat->enemy->cardDeck);
    return combat;
}


void playPlayerTurn(combat_t* combat){
    board_t * caracterBoard = combat->caracter->board;
    caracterBoard = drawCards(caracterBoard);
    printf("Le deck après avoir pioché :  \n");
    displayDeck(caracterBoard->cardDeck);
    printf("La main après avoir pioché : \n");
    displayDeck(caracterBoard->hand);
   // playCaracterCards(combat);
}

void playOnePlayerCard(combat_t* combat, card_t* cardToPlay){
    // stat_t* caracterMana = getStat(combat->caracter,MANA);
    // stat_t* caracterEnergy = getStat(combat->caracter,ENERGY);
    if(cardToPlay->energyCost < 100 && cardToPlay->manaCost <100){ // TODO : faire une fonction pour récupérer la valeur d'une stat
        applyCardEffect(cardToPlay,combat->caracter,combat->enemy);
        moveOneCardFromHand(combat->caracter->board,cardToPlay);
    }
}

void playCaracterCards(combat_t* combat){
   // int endTurn = 0;
    int i=0;
    while(i<5){ // tant que le tour n'est pas fini
        // on montre la main au joueur, et il choisit une carte
        printf("tour %d \n",i);
        card_t* cardToPlay = pickCardFromHand(combat->caracter->board->hand)->data;
        playOnePlayerCard(combat, cardToPlay);
        i++;

    }
}


int getChoosenCardId(deck_t* hand){
    displayDeck(hand);
   // char *nb =scanf(" \n Quelle carte voulez vous ? saisir numero \n");
    return 0;; // pour l'instant on prend juste la premiere blc
}

// int getChoosenCardIdGUI(deck_t* hand){
    
// }

deck_t* pickCardFromHand(deck_t * hand){
   // int (*choosenId)(deck_t) = &getChoosenCardId;
    return getElementFromDeckAtIndex(getChoosenCardId(hand),hand);
}

void testCombat(){
    entity_t* player = importCaracterFromId(PETER);
  
    displayEntity(player);
    //  entity_t* enemy = importBOSSFromId(GARDIAN_PLUME);
    //  displayEntity(enemy);
     combat_t* combat = startCombat(player,player);
    playPlayerTurn(combat);
}

// void playEnemyTurn(combat_t* combat){
    
// }


void playTurn(combat_t* combat){
    if(combat->caracterTurn==true){
        playPlayerTurn(combat);
    }
    else {
       // playEnemyTurn(combat);
    }
}