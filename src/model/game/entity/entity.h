#ifndef ENTITY
#define ENTITY

#include "../misc/deck/deck.h"
#include "../misc/effect/effect.h"
#include "../misc/stat/stat.h"
#include "../misc/board/board.h"

typedef struct
{
    char *name;
    deck *cardDeck;
    effect_t *effects;
    stat_t *stats; /* HP,DODGE(max 999),STR(max 999),Mana,Energy */
    board *board;
} entity_t;

// CONSTRUCTORS
entity_t *initEntity(char *name, int stats[][2]);
stat_t *initEntityStatBars(int stats[][2]);
effect_t *initEffectBar();

// DISPLAY FUNCTION
void displayEntity(entity_t *entity);

// GETTER / SETTER
stat_t *getStat(entity_t *entity, stat_ID statId);
effect_t *getEffect(entity_t *entity, effect_ID id);

// TEST FUNCTION
void testEntity();

#endif