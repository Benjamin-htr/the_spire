#include "entity.h"
#include "../misc/stat/stat.h"
#include <stdio.h>

// CONSTRUCTORS
entity_t *initEntity(char *name, int stats[][2])
{
    entity_t *res = malloc(sizeof(entity_t));
    res->name = name;
    res->stats = initEntityStatFromArray(stats);
    res->effects = initEffectBar();
    return res;
}

// DISPLAY FUNCTION

void displayEntity(entity_t *entity)
{
    printf("\nNAME:\n_____\n%s\n", entity->name);
    printf("\nSTATS: \n______\n");
    for (size_t stats_ID = 0; stats_ID < 4; stats_ID++)
    {
        displayStat(entity->stats[stats_ID]);
    };
    printf("\nEFFECTS: \n________\n");
    for (size_t effects_ID = 0; effects_ID < 5; effects_ID++)
    {
        displayEffect(entity->effects[effects_ID]);
    };

    printf("============================================\n");
}

// METHODS

void applyCardEffect(card_t *card, entity_t *launcher, entity_t *receiver)
{
    for (size_t launcherEffectID = 0; launcherEffectID < card->launcherEffectsSize; launcherEffectID++)
    {
        mergeEffect(launcher, card->launcherEffects[launcherEffectID]);
    }
    for (size_t receiverEffectID = 0; receiverEffectID < card->receiverEffectsSize; receiverEffectID++)
    {
        mergeEffect(receiver, card->receiverEffects[receiverEffectID]);
    }
}

// GETTER / SETTER

//      GETTER

stat_t *getStat(entity_t *entity, stat_ID statId)
{
    return &entity->stats[statId - 1];
}

effect_t *getEffect(entity_t *entity, effect_ID id)
{
    if (id < 3)
    {
        return NULL;
    }
    return &entity->effects[id - 4];
}

//      SETTER

void mergeEffect(entity_t *entity, effect_t effect)
{
    if (effect.id < 4)
    {
        stat_t *currentStat = getStat(entity, effect.id + 1);
        updateStat(currentStat, effect.value, CURR, PERCISTANT);
    }
    else
    {
        effect_t *currentEffect = getEffect(entity, effect.id);
        currentEffect->value += effect.value;
    }
}

// TEST

void testGetterSetter(entity_t *testCar)
{
    // GETTER TESTS

    printf("TEST GETTER/SETTER\n==================\n");
    printf("TEST getStat\n____________");
    stat_t *testStatGetter = getStat(testCar, DODGE);
    displayStat(*testStatGetter);

    printf("TEST getEffect\n______________");
    effect_t *testEffectGetter = getEffect(testCar, FIRE_E);
    displayEffect(*testEffectGetter);

    effect_t *testEffect = initEffect(STR_E, 10);
    mergeEffect(testCar, *testEffect);
}

void testApplyCardEffect(entity_t *testCar, entity_t *testEnemy)
{
    printf("\nTEST applyCard\n==============");

    card_t *test = createCard(
        "Strike",
        BASIC,
        0,
        1,
        false,
        (int[][2]){{STR_E, 6}, {DODGE_E, 10}},
        2,
        (int[][2]){{HP_E, -6}},
        1,
        "Inflige 6 dégats",
        "Attaque de base");
    displayEntity(testCar);
    displayEntity(testEnemy);
    applyCardEffect(test, testCar, testEnemy);
    printf("\napply effect\n");
    displayEntity(testCar);
    displayEntity(testEnemy);
}

void testEntity()
{

    int testStat[5][2] = {
        {10, 1},
        {100, 1},
        {30, 1},
        {10, 1},
        {10, 1},
    };

    entity_t *testCar = initEntity("peter", testStat);
    entity_t *testEnemy = initEntity("enemy", testStat);
    displayEntity(testCar);

    testGetterSetter(testCar);
    testApplyCardEffect(testCar, testEnemy);
}
