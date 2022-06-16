#include "item.h"
#include <stdio.h>

item_t *createItem(
    char *name,
    int launcherEffectsSize,
    int launcherEffects[launcherEffectsSize][2],
    int receiverEffectsSize,
    int receiverEffects[receiverEffectsSize][2],
    char *technic,
    char *description)
{
    printf("\n test init item 1");
    fflush(stdout);
    item_t *res = malloc(sizeof(item_t));
    printf("\n test init item 2");
    fflush(stdout);
    res->name = name;
    printf("\n test init item 3");
    fflush(stdout);
    res->launcherEffects = initEffectFromArray(launcherEffectsSize, launcherEffects);
    printf("\n test init item 4");
    fflush(stdout);
    res->launcherEffectsSize = launcherEffectsSize;
    printf("\n test init item 5");
    fflush(stdout);
    res->receiverEffects = initEffectFromArray(receiverEffectsSize, receiverEffects);
    printf("\n test init item 6");
    fflush(stdout);
    res->receiverEffectsSize = receiverEffectsSize;
    printf("\n test init item 7");
    fflush(stdout);
    res->technic = technic;
    printf("\n test init item 8");
    fflush(stdout);
    res->description = description;
    printf("\n test init item 9");
    fflush(stdout);

    return res;
}

item_t *importItem(item_import itemImport)
{
    return createItem(
        itemImport.name,
        itemImport.launcherEffectsSize,
        itemImport.launcherEffects,
        itemImport.receiverEffectsSize,
        itemImport.receiverEffects,
        itemImport.technic,
        itemImport.description);
}

item_t *importItemFromId(ITEM_ENCYCLOPEDIA_ID itemId)
{
    return importItem(ITEM_ENCYCLOPEDIA[itemId]);
}

item_t *importItemFromIdArray(int itemsId[], int itemLength)
{
    printf("\n test");
    fflush(stdout);
    item_t *res = calloc(5, sizeof(item_t *));
    printf("\n test");
    fflush(stdout);
    int item_ID;
    printf("\n test");
    fflush(stdout);
    for (item_ID = 0; item_ID < itemLength; item_ID++)
    {
        printf("\n test loop 1");
        fflush(stdout);
        res[item_ID] = *importItemFromId(itemsId[item_ID]);
    };
    for (; item_ID < 5; item_ID++)
    {
        printf("\n test loop 1");
        fflush(stdout);
        res[item_ID] = *importItemFromId(NONE_ITEM);
    };
    return res;
}

// DISPLAY FUNCTION

void displayItem(item_t item)
{
    printf("\nNAME:\n_____\n%s\n\nLAUNCHER EFFECT:\n________________\n", item.name);
    for (int launcherEffectID = 0; launcherEffectID < item.launcherEffectsSize; launcherEffectID++)
    {
        displayEffect(item.launcherEffects[launcherEffectID]);
    }
    printf("\nRECEIVER EFFECT:\n________________\n");
    for (int receiverEffectID = 0; receiverEffectID < item.receiverEffectsSize; receiverEffectID++)
    {
        displayEffect(item.receiverEffects[receiverEffectID]);
    }
}

// TEST FUNCTION

void testItem()
{
    item_t *testItem = importItemFromIdArray((int[]){NONE_ITEM, WEAPON, ARMOR}, 3);
    for (int i = 0; i < 5; i++)
    {
        displayItem(testItem[i]);
    }
    // testItem = importItemFromId(WEAPON);
    // displayItem(*testItem);
    // testItem = importItemFromId(HELMET);
    // displayItem(*testItem);
    // testItem = importItemFromId(ARMOR);
    // displayItem(*testItem);
    // testItem = importItemFromId(SHOES);
    // displayItem(*testItem);
}

item_import ITEM_ENCYCLOPEDIA[] = {
    {
        .name = "NONE",
        .launcherEffectsSize = 0,
        .receiverEffectsSize = 0,
    },
    {
        .name = "Casse-Croute",
        .description = "Recupère un petit peut de vie a chaque debut de combat",
        .technic = "PV +6",
        .launcherEffects = {{HP_E, 6}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
    },
    {
        .name = "Arme",
        .description = "Augmente la force pour toute la durée du combat",
        .technic = "FORCE +4",
        .launcherEffects = {{STR_E, 4}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
    },
    {
        .name = "Casque",
        .description = "Augmente la dexterité pour toute la durée du combat afin d'obtenir plus de point d'esquive",
        .technic = "DEXTERITE +4",
        .launcherEffects = {{DEX_E, 4}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
    },
    // TODO: Refacto
    {
        .name = "Armure",
        .description = "Augmente les points de vie maximum",
        .technic = "PV MAX +20",
        .launcherEffects = {{HP_MAX_E, 20}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
    },
    // TODO: Refacto
    {
        .name = "Chaussures",
        .description = "Permet de faire plus d'action à chaque tour",
        .technic = "ENERGY MAX +1",
        .launcherEffects = {{ENERGY_MAX_E, 1}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
    },
};