#ifndef COMBAT
#define COMBAT

#include "../entity/entity.h"
typedef struct
{
    entity_t *caracter;
    entity_t *enemy;
    int caracterTurn;
} combat_t;

#include "../misc/board/board.h"
#include "../../../view/combat/combat.h"
combat_t *startCombat(entity_t *caracter, entity_t *enemy);

// to remove when GUI in place
int getChoosenCardId(board_t *board);
int getChoosenRewardCardId(deck_t *deck);
int chooseRandomCardId(board_t *board);
boolean checkEndCombat(combat_t *combat);
boolean checkVictory(combat_t *combat);

card_t *pickCardFromHand(entity_t *caracter, int (*cardChoosingFunc)(board_t *));

card_t *playEnemyCards(combat_t *combat);
void playTurn(entity_t *entity);

void playOneCard(entity_t *launcher, entity_t *receiver, card_t *cardToPlay);

void testCombat();

void startFight(combat_t *combat);

void freeCombat(combat_t *combat);

#endif