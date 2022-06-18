#include "entity.h"
#include "../misc/stat/stat.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    entity_t *res = malloc(sizeof(entity_t));
    res->name = name;
    res->stats = initEntityStatFromArray(stats);
    res->effects = initEntityEffectArray();
    res->cardDeck = createDeckFromArray(cards, diffCardSize);
    res->items = importEntityItemFromIdArray(itemslength, items);
    return res;
}

void freeEntity(entity_t *entity)
{
    freeEntityStatArray(entity->stats);
    freeEntityEffectArray(entity->effects);
    freeDeckListAndCard(entity->cardDeck);
    freeEntityItem(entity->items);
    free(entity);
}

entity_t *importCaracter(entity_import entitySkel)
{
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
    displayEntityStatArray(entity->stats);
    displayEntityEffectArray(entity->effects);
    displayEntityItems(entity->items);
    printf("DECK: \n_____\n");
    displayDeck(entity->cardDeck);

    printf("============================================\n");
}

// METHODS

void takeDamage(entity_t *entity, int value)
{
    stat_t *entityDodge = getEntityStat(entity, DODGE);
    stat_t *entityHealth = getEntityStat(entity, HP);
    if (-value >= entityDodge->current)
    {
        value += entityDodge->current;
        entityDodge->current = 0;
        updateStat(entityHealth, value, false);
    }
    else
    {
        updateStat(entityDodge, value, false);
    }
}

void applyCardEffect(card_t *card, entity_t *launcher, entity_t *receiver)
{
    printf("la carte %s est joué\n", card->name);
    for (int launcherEffectID = 0; launcherEffectID < card->launcherEffectsSize; launcherEffectID++)
    {
        mergeEffect(launcher, card->launcherEffects[launcherEffectID]);
    }
    for (int receiverEffectID = 0; receiverEffectID < card->receiverEffectsSize; receiverEffectID++)
    {
        mergeEffect(receiver, card->receiverEffects[receiverEffectID]);
    }
}

// dupplication de code pas ouf :/
void applyAllItemsEffect(entity_t *entity)
{
    for (int itemsIdx = 0; itemsIdx < 5 && entity->items[itemsIdx]->description != NULL; itemsIdx++)
    {
        for (int itemEffectIdx = 0; itemEffectIdx < entity->items[itemsIdx]->launcherEffectsSize; itemEffectIdx++)
        {
            mergeEffect(entity, entity->items[itemsIdx]->launcherEffects[itemEffectIdx]);
        }
        for (int itemEffectIdx = 0; itemEffectIdx < entity->items[itemsIdx]->receiverEffectsSize; itemEffectIdx++)
        {
            mergeEffect(entity, entity->items[itemsIdx]->receiverEffects[itemEffectIdx]);
        }
    }
}

void wipeAllEffect(entity_t *entity)
{
    for (int effectIdx = 0; effectIdx < 5; effectIdx++)
    {
        entity->effects[effectIdx]->value = 0;
    }
}

void turnBeginEffectUpdate(entity_t *entity)
{
    effect_t *entityFIRE = getEntityEffect(entity, FIRE_E);
    effect_t *entityWEAK = getEntityEffect(entity, WEAK_E);
    effect_t *entitySLOW = getEntityEffect(entity, SLOW_E);
    takeDamage(entity, entityFIRE->value);
    entityFIRE->value = floor(entityFIRE->value / 2.0);
    if (entitySLOW->value > 0)
    {
        (entitySLOW->value)--;
    }
    if (entityWEAK->value > 0)
    {
        (entityWEAK->value)--;
    }
}

void updateCardEffectWithEntityEffect(entity_t *entity, effect_t *cardEffect)
{
    effect_t *entityStr = getEntityEffect(entity, STR_E);
    effect_t *entityDex = getEntityEffect(entity, DEX_E);
    effect_t *entityWeak = getEntityEffect(entity, WEAK_E);
    effect_t *entitySlow = getEntityEffect(entity, SLOW_E);
    if (cardEffect->value < 0)
    {
        if (cardEffect->id == DODGE_E)
        {
            cardEffect->value = ceil(((cardEffect->value + entityDex->value) / (entitySlow->value ? 2.0 : 1.0)));
        }
        else if (cardEffect->id == HP_E)
        {
            cardEffect->value = floor(((cardEffect->value + entityStr->value) * (entityWeak->value ? 0.75 : 1.0)));
        }
    }
}

void mergeEffect(entity_t *entity, effect_t *effect)
{
    fflush(stdout);
    if (effect->id == HP_E && effect->value < 0)
    {
        takeDamage(entity, effect->value);
    }
    else if (effect->id < 4)
    {
        stat_t *currentStat = getEntityStat(entity, effect->id + 1);
        updateStat(currentStat, effect->value, false);
    }
    else if (effect->id < 8)
    {
        stat_t *currentStat = getEntityStat(entity, effect->id - 3);
        updateStat(currentStat, effect->value, true);
    }
    else
    {
        effect_t *currentEffect = getEntityEffect(entity, effect->id);
        currentEffect->value += effect->value;
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
    return entity->effects[id - 8];
}

card_t *getTrueCardValue(entity_t *entity, card_t *card) // On oublie pas de free la carte retourné une fois l'utilisation fini
{
    card_t *res = copyCard(card);
    effect_t *cardEffect;
    for (int launcherEffectID = 0; launcherEffectID < card->launcherEffectsSize; launcherEffectID++)
    {
        cardEffect = card->launcherEffects[launcherEffectID];
        updateCardEffectWithEntityEffect(entity, cardEffect);
    }
    for (int receiverEffectID = 0; receiverEffectID < card->receiverEffectsSize; receiverEffectID++)
    {
        cardEffect = card->receiverEffects[receiverEffectID];
        updateCardEffectWithEntityEffect(entity, cardEffect);
    }
    return res;
}

//      SETTER

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
    printf("\n==============================\n\tTEST DE L'ENTITY\n==============================\n");
    entity_t *testCar = importCaracterFromId(TEST_CAR);
    // entity_t *testPeter = importCaracterFromId(PETER);
    getEntityStat(testCar, DODGE)->current = 3;
    displayEntity(testCar);
    takeDamage(testCar, -10);
    displayEntity(testCar);
    // displayEntity(testPeter);
    freeEntity(testCar);
    // freeEntity(testPeter);
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
            {75, false},
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
            {75, false},
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
