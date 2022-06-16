#include "item.h"
#include <stdio.h>

item_t *createItem(
    char *name,
    int launcherEffects[6][2],
    size_t launcherEffectsSize,
    int receiverEffects[6][2],
    size_t receiverEffectsSize,
    char *technic,
    char *description)
{
    item_t *res = malloc(sizeof(item_t));
    res->name = name;
    res->launcherEffects = initEffectFromArray(launcherEffects, launcherEffectsSize);
    res->launcherEffectsSize = launcherEffectsSize;
    res->receiverEffects = initEffectFromArray(receiverEffects, receiverEffectsSize);
    res->receiverEffectsSize = receiverEffectsSize;
    res->technic = technic;
    res->description = description;

    return res;
}

item_t *importItem(item_import itemImport)
{
    return createItem(
        itemImport.name,
        itemImport.launcherEffects,
        itemImport.launcherEffectsSize,
        itemImport.receiverEffects,
        itemImport.receiverEffectsSize,
        itemImport.technic,
        itemImport.description);
}

item_t *importItemFromId(ITEM_ENCYCLOPEDIA_ID itemId)
{
    return importItem(ITEM_ENCYCLOPEDIA[itemId]);
}

item_t *importItemFromIdArray(int itemsId[], int itemLength)
{
    item_t *res = calloc(5, sizeof(item_t *));
    int item_ID;
    for (item_ID = 0; item_ID < itemLength; item_ID++)
    {
        res[item_ID] = *importItemFromId(itemsId[item_ID]);
    };
    for (; item_ID < 5; item_ID++)
    {
        res[item_ID] = *importItemFromId(NONE_ITEM);
    };
    return res;
}

// DISPLAY FUNCTION

void displayItem(item_t item)
{
    printf("\nNAME:\n_____\n%s\n\nLAUNCHER EFFECT:\n________________\n", item.name);
    for (size_t launcherEffectID = 0; launcherEffectID < item.launcherEffectsSize; launcherEffectID++)
    {
        displayEffect(item.launcherEffects[launcherEffectID]);
    }
    printf("\nRECEIVER EFFECT:\n________________\n");
    for (size_t receiverEffectID = 0; receiverEffectID < item.receiverEffectsSize; receiverEffectID++)
    {
        displayEffect(item.receiverEffects[receiverEffectID]);
    }
}

// TEST FUNCTION

void testItem()
{
    item_t *testItem = importItemFromIdArray((int[]){NONE_ITEM, WEAPON, ARMOR}, 3);
    for (size_t i = 0; i < 5; i++)
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