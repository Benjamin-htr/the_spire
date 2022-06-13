#ifndef ENTITY
#define ENTITY

#include "../misc/effect/effect.h"
#include "../misc/stat/stat.h"
#include "../misc/board/board.h"

typedef enum
{
    PETER,
    CARACTER_ID_SIZE,
} CARACTER_ID;

typedef enum
{
    JAWURM,
    BLOUNI,
    KELIKO,
    ENEMY_PHASE_1_ID_SIZE,
} ENEMY_PHASE_1_ID;

typedef enum
{
    JAWURM2,
    REDONI,
    MANGOUSTINE,
    ENEMY_PHASE_2_ID_SIZE,
} ENEMY_PHASE_2_ID;

typedef enum
{
    ELDAN,
    PYROX,
    MINIBOSS_ID_SIZE,
} MINIBOSS_ID;

typedef enum
{
    GARDIAN_PLUME,
    BOSS_ID_SIZE,
} BOSS_ID;

typedef struct
{
    char *name;
    deck_t *cardDeck;
    effect_t *effects;
    stat_t *stats;
    board_t *board;
} entity_t;

typedef struct
{
    char *name;
    int cardDeck[4][2];
    int diffCardSize;
    int stats[4][2];
} entity_import;

typedef struct
{
    char *name;
    int hpRange[2];
    int cardDeck[4][2];
    int diffCardSize;
} enemy_import;

// CONSTRUCTORS
entity_t *initEntity(char *name, int stats[][2], int cards[][2], int diffCardSize);

entity_t *importCaracter(entity_import entitySkel);
entity_t *importCaracterFromId(CARACTER_ID id);
entity_t *importEnemy(enemy_import enemySkel);
entity_t *importEnemyPhase1FromId(ENEMY_PHASE_1_ID id);
entity_t *getRandomEnemyPhase1();
entity_t *importEnemyPhase2FromId(ENEMY_PHASE_2_ID id);
entity_t *getRandomEnemyPhase2();
entity_t *importMiniBossFromId(MINIBOSS_ID id);
entity_t *getRandomMiniBoss();
entity_t *importBOSSFromId(BOSS_ID id);

// DISPLAY FUNCTION
void displayEntity(entity_t *entity);

// METHOD

void applyCardEffect(card_t *card, entity_t *launcher, entity_t *receiver);

// GETTER / SETTER
stat_t *getStat(entity_t *entity, stat_ID statId);
effect_t *getEffect(entity_t *entity, effect_ID id);

//      SETTER
void mergeEffect(entity_t *entity, effect_t effect);

// TEST FUNCTION
void testEntity();

extern entity_import CARATER_ENCYCLOPEDIA[];
extern enemy_import ENEMY_PHASE_1_ENCYCLOPEDIA[];
extern enemy_import ENEMY_PHASE_2_ENCYCLOPEDIA[];
extern enemy_import MINIBOSS_ENCYCLOPEDIA[];
extern enemy_import BOSS_ENCYCLOPEDIA[];

#endif