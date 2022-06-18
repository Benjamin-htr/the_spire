#ifndef COMBAT
#define COMBAT

#include "../misc/board/board.h"
#include "../entity/entity.h"

typedef struct
{
    entity_t *caracter;
    entity_t *enemy;
    int caracterTurn;
} combat_t;

combat_t *startCombat(entity_t *caracter, entity_t *enemy);

void playTurn(combat_t *combat, entity_t *entity);

int getChoosenCardId(deck_t *hand);
int chooseRandomCardId(deck_t *hand);

card_t *pickCardFromHand(entity_t *caracter, int (*cardChoosingFunc)(deck_t *));

void playCards(combat_t *combat);

void playOneCard(entity_t *launcher, entity_t *receiver, card_t *cardToPlay);

void testCombat();

void startFight(combat_t *combat);
#endif