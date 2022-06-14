#ifndef COMBAT
#define COMBAT

#include "../misc/board/board.h"
#include "../entity/entity.h"

typedef struct
{
    entity_t* caracter;
    entity_t* enemy;
    int caracterTurn;
}combat_t;

combat_t * startCombat(entity_t* caracter, entity_t* enemy);

void playTurn(combat_t* combat);

void playEnemyTurn(combat_t* combat);

int getChoosenCardId(deck_t* hand);

deck_t * pickCardFromHand(deck_t * hand);

void playPlayerTurn(combat_t* combat);

void playCaracterCards(combat_t* combat);

void playOnePlayerCard(combat_t* combat, card_t* cardToPlay);

void testCombat();

int getChoosenCardId(deck_t* hand);
#endif