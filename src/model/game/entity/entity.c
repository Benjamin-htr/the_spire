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
    return &entity->effects[id - 2];
}

//      SETTER

void mergeEffect(entity_t *entity, effect_t effect)
{
    if (effect.id < 2)
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

void testEntity()
{
    int testStat[5][2] = {
        {10, 1},
        {100, 1},
        {30, 1},
        {10, 1},
        {10, 1},
    };
    entity_t *test = initEntity("martino", testStat);
    displayEntity(test);

    // GETTER TESTS
    stat_t *testStatGetter = getStat(test, DODGE);
    displayStat(*testStatGetter);

    effect_t *testEffectGetter = getEffect(test, FIRE_E);
    displayEffect(*testEffectGetter);

    effect_t *testEffect = initEffect(STR_E, 10);
    mergeEffect(test, *testEffect);

    displayEntity(test);
}
