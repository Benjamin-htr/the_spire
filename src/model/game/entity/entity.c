#include "entity.h"
#include "../misc/stat/stat.h"
#include <stdio.h>

// CONSTRUCTORS
entity_t *initEntity(char *name, int stats[][2])
{
    entity_t *res = malloc(sizeof(entity_t));
    res->name = name;
    res->stats = initEntityStatBars(stats);
    res->effects = initEffectBar();
    return res;
}

stat_t *initEntityStatBars(int stats[][2])
{
    stat_t *res = calloc(6, sizeof(stat_t *));
    for (size_t stats_ID = 0; stats_ID < 5; stats_ID++)
    {
        res[stats_ID] = *initStat(stats_ID + 1, stats[stats_ID][0], stats[stats_ID][1]);
    };
    return res;
}

effect_t *initEffectBar()
{
    effect_t *res = calloc(5, sizeof(effect_t *));
    for (size_t effects_ID = 0; effects_ID < 5; effects_ID++)
    {
        res[effects_ID] = *initEffect(effects_ID + 3, 0);
    };
    return res;
}

// DISPLAY FUNCTION

void displayEntity(entity_t *entity)
{
    printf("NAME:\n_____\n%s\n", entity->name);
    printf("\nSTATS: \n______\n");
    for (size_t stats_ID = 0; stats_ID < 5; stats_ID++)
    {
        displayStat(entity->stats[stats_ID]);
    };
    printf("\nEFFECTS: \n________\n");
    for (size_t effects_ID = 0; effects_ID < 5; effects_ID++)
    {
        displayEffect(entity->effects[effects_ID]);
    };

    printf("============================================");
}

// GETTER / SETTER

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
    return &entity->effects[id - 3];
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
}
