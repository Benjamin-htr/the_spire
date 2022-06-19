#ifndef ENTITY
#define ENTITY

#include "../misc/effect/effect.h"
#include "../misc/stat/stat.h"
#include "../misc/board/board.h"
#include "../misc/item/item.h"

typedef enum
{
    PETER,
    TEST_CAR,
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
    item_t **items;
    effect_t **effects;
    stat_t **stats;
    board_t *board;
    char *spriteName;
} entity_t;

typedef struct
{
    char *name;
    int items[5];
    int itemslength;
    int cardDeck[4][2];
    int diffCardSize;
    int stats[4][2];
    char *spriteName;
} entity_import;

typedef struct
{
    char *name;
    int hpRange[2];
    int items[5];
    int itemslength;
    int cardDeck[4][2];
    int diffCardSize;
    char *spriteName;
} enemy_import;

// CONSTRUCTORS
entity_t *initEntity(
    char *name,
    int stats[][2],
    int items[],
    int itemslength,
    int cards[][2],
    int diffCardSize,
    char *spriteName);

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

// DEXCONSTRUTOR

void freeEntity(entity_t *entity);

// DISPLAY FUNCTION
void displayEntity(entity_t *entity);

// METHOD

void applyCardEffect(card_t *card, entity_t *launcher, entity_t *receiver);
void applyAllItemsEffect(entity_t *launcher, entity_t *receiver);
void applyItemMaxEffect(entity_t *launcher, item_t *item);
void removeItemMaxEffect(entity_t *launcher, item_t *item);
void addItemtoEntityItemList(entity_t *entity, ITEM_ENCYCLOPEDIA_ID id);
void turnBeginEffectUpdate(entity_t *entity);
void updateCardEffectWithEntityEffect(entity_t *entity, effect_t *cardEffect);
void importEntityItemFromIdArray(entity_t *entity, int itemLength, int itemsId[itemLength]);
void takeDamage(entity_t *entity, int value);

// GETTER / SETTER
stat_t *getEntityStat(entity_t *entity, stat_ID statId);
effect_t *getEntityEffect(entity_t *entity, effect_ID id);
card_t *getTrueCardValue(entity_t *entity, card_t *card); // On oublie pas de free la carte retourné une fois l'utilisation fini

//      SETTER
void mergeEffect(entity_t *entity, effect_t *effect);

// TEST FUNCTION
void testEntity();

void freeEntity(entity_t *entity);

extern entity_import CARATER_ENCYCLOPEDIA[];
extern enemy_import ENEMY_PHASE_1_ENCYCLOPEDIA[];
extern enemy_import ENEMY_PHASE_2_ENCYCLOPEDIA[];
extern enemy_import MINIBOSS_ENCYCLOPEDIA[];
extern enemy_import BOSS_ENCYCLOPEDIA[];

#endif