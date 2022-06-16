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


void playTurn(combat_t* combat, board_t* board);

int getChoosenCardId(deck_t* hand);

deck_t * pickCardFromHand(deck_t * hand);


void playCards(combat_t* combat);

void playOnePlayerCard(combat_t* combat, card_t* cardToPlay);

void playOneEnemyCard(combat_t* combat, card_t* cardToPlay);

void testCombat();

void startFight(combat_t* combat);
#endif