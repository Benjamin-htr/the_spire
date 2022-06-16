#include "entity.h"
#include "../misc/stat/stat.h"
#include <stdio.h>
#include <stdlib.h>

// CONSTRUCTORS
entity_t *initEntity(
    char *name,
    int stats[][2],
    int items[],
    int itemslength,
    int cards[][2],
    int diffCardSize)
// for cards param first int is the card id 2nd one is the number of card
// for diffCardSize param is just the size of the array
{
    printf("\n test 1");
    fflush(stdout);
    entity_t *res = malloc(sizeof(entity_t));
    printf("\n test2");
    fflush(stdout);
    res->name = name;
    printf("\n test3");
    fflush(stdout);
    res->stats = initEntityStatFromArray(stats);
    printf("\n test4");
    fflush(stdout);
    res->effects = initEntityEffectArray();
    printf("\n test5");
    fflush(stdout);
    res->cardDeck = createDeckFromArray(cards, diffCardSize);
    printf("\n test6");
    fflush(stdout);
    res->items = importItemFromIdArray(items, itemslength);
    printf("\n test7");
    fflush(stdout);
    return res;
}

entity_t *importCaracter(entity_import entitySkel)
{
    printf("\n test");
    fflush(stdout);
    return initEntity(
        entitySkel.name,
        entitySkel.stats,
        entitySkel.items,
        entitySkel.itemslength,
        entitySkel.cardDeck,
        entitySkel.diffCardSize);
}

entity_t *importCaracterFromId(CARACTER_ID id)
{
    printf("\n test");
    fflush(stdout);
    return importCaracter(CARATER_ENCYCLOPEDIA[id]);
}

entity_t *importEnemy(enemy_import enemySkel)
{
    int hp =
        enemySkel.hpRange[0] + (rand() %
                                (enemySkel.hpRange[1] -
                                 enemySkel.hpRange[0]));
    int stats[5][2] = {
        {hp, false},
        {999, true},
        {999, false},
        {999, false},
    };
    return initEntity(
        enemySkel.name,
        stats,
        enemySkel.items,
        enemySkel.itemslength,
        enemySkel.cardDeck,
        enemySkel.diffCardSize);
}

entity_t *importEnemyPhase1FromId(ENEMY_PHASE_1_ID id)
{
    return importEnemy(ENEMY_PHASE_1_ENCYCLOPEDIA[id]);
}

entity_t *getRandomEnemyPhase1()
{
    return importEnemyPhase1FromId(rand() % ENEMY_PHASE_1_ID_SIZE);
}

entity_t *importEnemyPhase2FromId(ENEMY_PHASE_2_ID id)
{
    return importEnemy(ENEMY_PHASE_2_ENCYCLOPEDIA[id]);
}

entity_t *getRandomEnemyPhase2()
{
    return importEnemyPhase2FromId(rand() % ENEMY_PHASE_2_ID_SIZE);
}

entity_t *importMiniBossFromId(MINIBOSS_ID id)
{
    return importEnemy(MINIBOSS_ENCYCLOPEDIA[id]);
}

entity_t *getRandomMiniBoss()
{
    return importMiniBossFromId(rand() % MINIBOSS_ID_SIZE);
}

entity_t *importBOSSFromId(BOSS_ID id)
{
    return importEnemy(BOSS_ENCYCLOPEDIA[id]);
}

// DISPLAY FUNCTION

void displayEntity(entity_t *entity)
{
    printf("NAME:\n_____\n%s\n", entity->name);
    printf("STATS: \n______\n");
    for (int stats_ID = 0; stats_ID < 4; stats_ID++)
    {
        displayStat(entity->stats[stats_ID]);
    }
    printf("EFFECTS: \n________\n");
    for (int effects_ID = 0; effects_ID < 9; effects_ID++)
    {
        displayEffect(entity->effects[effects_ID]);
    }
    printf("ITEMS: \n______\n");

    for (int itemsIdx = 0; itemsIdx < 5 && entity->items[itemsIdx].description != NULL; itemsIdx++)
    {
        displayItem(entity->items[itemsIdx]);
    }
    printf("DECK: \n_____\n");
    displayDeck(entity->cardDeck);

    printf("============================================\n");
}

// METHODS

void applyCardEffect(card_t *card, entity_t *launcher, entity_t *receiver)
{
    for (int launcherEffectID = 0; launcherEffectID < card->launcherEffectsSize; launcherEffectID++)
    {
        mergeEffect(launcher, card->launcherEffects[launcherEffectID]);
    }
    for (int receiverEffectID = 0; receiverEffectID < card->receiverEffectsSize; receiverEffectID++)
    {
        mergeEffect(receiver, card->receiverEffects[receiverEffectID]);
    }
}

void applyAllItemsEffect(entity_t *entity)
{
    for (int itemsIdx = 0; itemsIdx < 5 && entity->items[itemsIdx].description != NULL; itemsIdx++)
    {
        for (int itemEffectIdx = 0; itemEffectIdx < entity->items[itemsIdx].launcherEffectsSize; itemEffectIdx++)
        {
            mergeEffect(entity, entity->items[itemsIdx].launcherEffects[itemEffectIdx]);
        }
        for (int itemEffectIdx = 0; itemEffectIdx < entity->items[itemsIdx].receiverEffectsSize; itemEffectIdx++)
        {
            mergeEffect(entity, entity->items[itemsIdx].receiverEffects[itemEffectIdx]);
        }
    }
}

void wipeAllEffect(entity_t *entity)
{
    for (int effectIdx = 0; effectIdx < 9; effectIdx++)
    {
        entity->effects[effectIdx]->value = 0;
    }
}

// GETTER / SETTER

//      GETTER

stat_t *getEntityStat(entity_t *entity, stat_ID statId)
{
    return entity->stats[statId - 1];
}

effect_t *getEntityEffect(entity_t *entity, effect_ID id)
{
    if (id < 3)
    {
        return NULL;
    }
    return entity->effects[id - 4];
}
//      SETTER
void mergeEffect(entity_t *entity, effect_t *effect)
{
    if (effect->id < 4)
    {
        stat_t *currentStat = getEntityStat(entity, effect->id + 1);
        updateStat(currentStat, effect->value, false);
    }
    else
    {
        effect_t *currentEffect = getEntityEffect(entity, effect->id);
        currentEffect->value += effect->value;
    }
}

// TEST

void testGetterSetter(entity_t *testCar)
{
    // GETTER TESTS

    printf("TEST GETTER/SETTER\n==================\n");
    printf("TEST getEntityStat\n____________");
    stat_t *testStatGetter = getEntityStat(testCar, DODGE);
    displayStat(testStatGetter);

    printf("TEST getEntityEffect\n______________");
    effect_t *testEffectGetter = getEntityEffect(testCar, FIRE_E);
    displayEffect(testEffectGetter);

    effect_t *testEffect = initEffect(STR_E, 10);
    mergeEffect(testCar, testEffect);
}

void testApplyCardEffect(entity_t *testCar, entity_t *testEnemy)
{
    printf("\nTEST applyCard\n==============");

    card_t *testCard = importCardFromId(SPECTRUM);
    displayEntity(testCar);
    displayEntity(testEnemy);
    applyCardEffect(testCard, testCar, testEnemy);
    printf("\napply effect\n");
    displayEntity(testCar);
    displayEntity(testEnemy);
}

void testEntity()
{
    entity_t *testCar = importCaracterFromId(TEST_CAR);
    // entity_t *testEnemy = getRandomMiniBoss();
    displayEntity(testCar);
    // entity_t *testCar2 = importCaracterFromId(PETER);
    // // entity_t *testEnemy = getRandomMiniBoss();
    // displayEntity(testCar2);
    // applyAllItemsEffect(testCar);
    // displayEntity(testCar);
    // wipeAllEffect(testCar);
    // displayEntity(testCar);

    // testGetterSetter(testCar);
    // testApplyCardEffect(testCar, testCar);
}

entity_import CARATER_ENCYCLOPEDIA[] = {
    {
        .name = "Peter",
        .stats = {
            {75, true},
            {999, true},
            {3, true},
            {100, true},
        },
        .items = {
            LUNCH_BOX,
        },
        .itemslength = 1,
        .cardDeck = {
            {STRIKE, 5},
            {DODGE_A, 5},
            {SPECTRUM, 1},
        },
        .diffCardSize = 3,
    },
    {
        .name = "Tester",
        .stats = {
            {75, true},
            {999, true},
            {3, true},
            {100, true},
        },
        .items = {
            LUNCH_BOX,
            ARMOR,
            WEAPON,
            HELMET,
            SHOES,
        },
        .itemslength = 5,
        .cardDeck = {
            {STRIKE, 5},
            {DODGE_A, 5},
            {SPECTRUM, 1},
        },
        .diffCardSize = 3,
    },
};
enemy_import ENEMY_PHASE_1_ENCYCLOPEDIA[] = {
    {
        .name = "Jawurm",
        .hpRange = {40, 44},
        .cardDeck = {
            {JAWURM_BACKSTAB, 1},
            {JAWURM_FIST, 1},
            {JAWURM_CROUCH, 1},
        },
        .itemslength = 0,
        .diffCardSize = 3,
    },
    {
        .name = "Blouni",
        .hpRange = {46, 50},
        .cardDeck = {
            {BLOUNI_JAB, 1},
            {BLOUNI_KICK, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
    },
    {
        .name = "Keliko",
        .hpRange = {60, 70},
        .cardDeck = {
            {KELIKO_NUDGE, 1},
            {KELIKO_PINCH, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
    },
};
enemy_import ENEMY_PHASE_2_ENCYCLOPEDIA[] = {
    {
        .name = "Jawurm2",
        .hpRange = {55, 62},
        .cardDeck = {
            {JAWURM2_HAIRPULLING, 1},
            {JAWURM2_SPIT, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
    },
    {
        .name = "Redoni",
        .hpRange = {50, 54},
        .cardDeck = {
            {REDONI_SLAP, 1},
            {REDONI_SPANKING, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
    },
    {
        .name = "Mangoustine",
        .hpRange = {70, 80},
        .cardDeck = {
            {MANGOUSTINE_SNARE, 1},
        },
        .itemslength = 0,
        .diffCardSize = 1,
    },
};
enemy_import MINIBOSS_ENCYCLOPEDIA[] = {
    {
        .name = "Eldan",
        .hpRange = {80, 81},
        .cardDeck = {
            {ELDAN_BANANAPEEL, 1},
            {HEADBUTT, 1},
            {ELDAN_TOTAL, 1},
        },
        .itemslength = 0,
        .diffCardSize = 3,
    },
    {
        .name = "Pyrox",
        .hpRange = {120, 121},
        .cardDeck = {
            {PYROX_FIRESPIT, 1},
            {PYROX_FIRESTORM, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
    },
};
enemy_import BOSS_ENCYCLOPEDIA[] = {
    {
        .name = "Gardien de la plume",
        .hpRange = {150, 151},
        .cardDeck = {
            {SPECTRUM, 1},
            {PULVERIZE, 1},
            {CLAW_COMBO, 1},
            {SLEEP, 1},
        },
        .itemslength = 0,
        .diffCardSize = 4,
    },
};
