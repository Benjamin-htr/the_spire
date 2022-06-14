#include "combat.h"
#include <stdlib.h>;

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
    caracterBoard = drawCards();
    playCaracterCards(combat);
}

void playCaracterCards(combat_t* combat){
    int endTurn = 0;
    while(!endTurn){ // tant que le tour n'est pas fini
        // on montre la main au joueur, et il choisit une carte
        card_t* cardToPlay = pickCardFromHand(combat->caracter->board->hand);
        playOnePlayerCard(combat, cardToPlay);

    }
}

void playOnePlayerCard(combat_t* combat, card_t* cardToPlay){
    stat_t* caracterMana = getStat(combat->caracter,MANA);
    stat_t* caracterEnergy = getStat(combat->caracter,ENERGY);
    if(cardToPlay->energyCost < 100 && cardToplay->manaCost <100){ // TODO : faire une fonction pour récupérer la valeur d'une stat
        applyCardEffect(cardToPlay,combat->caracter,combat->enemy);
        moveOneCardFromHand(combat->caracter->board,cardToPlay);
    }
}

int getChoosenCardId(deck_t* hand){
    displayDeck();
    printf(" \n Quelle carte voulez vous ? saisir numero \n");
    return 0; // pour l'instant on prend juste la premiere blc
}

int getChoosenCardIdGUI(deck_t* hand){
    
}

card_t * pickCardFromHand(deck_t * hand){
    int (*choosenId)(deck_t) = &getChoosenCardId;
    return getElementFromDeckAtIndex(choosenId(hand),hand);

}

void testCombat(){
    
}

void playEnemyTurn(combat_t* combat){
    
}


void playTurn(combat_t* combat){
    if(combat->caracterTurn==true){
        playPlayerTurn(combat);
    }
    else {
        playEnemyTurn(combat);
    }
}