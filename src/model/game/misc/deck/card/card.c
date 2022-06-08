#include "card.h"
#include "card-global.h"
#include "../../../entity/entity.h"
#include <stdlib.h>
#include <stdio.h>

// CONSTRUCTOR

card_t *createCard(
    char *name,
    enum rarityE rarity,
    int manaCost,
    int energyCost,
    boolean isAbyssal,
    int launcherEffects[][2],
    size_t launcherEffectsSize,
    int receiverEffects[][2],
    size_t receiverEffectsSize,
    char *technic,
    char *description)
{
    card_t *res = malloc(sizeof(card_t));
    res->name = name;
    res->rarity = rarity;
    res->manaCost = manaCost;
    res->energyCost = energyCost;
    res->isAbyssal = isAbyssal;
    res->launcherEffects = initEffectFromArray(launcherEffects, launcherEffectsSize);
    res->launcherEffectsSize = launcherEffectsSize;
    res->receiverEffects = initEffectFromArray(receiverEffects, receiverEffectsSize);
    res->receiverEffectsSize = receiverEffectsSize;
    res->technic = technic;
    res->description = description;
    return res;
}

// DISPLAY FUNCTION

void displayCard(card_t card)
{
    printf("\nNAME:\n_____\n%s\n\nLAUNCHER EFFECT:\n________________\n", card.name);
    for (size_t launcherEffectID = 0; launcherEffectID < card.launcherEffectsSize; launcherEffectID++)
    {
        displayEffect(card.launcherEffects[launcherEffectID]);
    }
    printf("\nRECEIVER EFFECT:\n________________\n");
    for (size_t receiverEffectID = 0; receiverEffectID < card.receiverEffectsSize; receiverEffectID++)
    {
        displayEffect(card.receiverEffects[receiverEffectID]);
    }
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

// TEST FUNCTION

void testCard()
{
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
    displayCard(*test);

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
    displayEntity(testEnemy);
    applyCardEffect(test, testCar, testEnemy);
    printf("\napply effect\n");
    displayEntity(testCar);
    displayEntity(testEnemy);
}
