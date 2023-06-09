#include "item.h"
#include <stdio.h>

// INIT FUNCTION
item_t *createItem(
    char *name,
    int launcherEffectsSize,
    int launcherEffects[launcherEffectsSize][2],
    int receiverEffectsSize,
    int receiverEffects[receiverEffectsSize][2],
    char *technic,
    char *description,
    char *imageName)
{
    item_t *res = malloc(sizeof(item_t));
    res->name = name;
    res->launcherEffects = initEffectFromArray(launcherEffectsSize, launcherEffects);
    res->launcherEffectsSize = launcherEffectsSize;
    res->receiverEffects = initEffectFromArray(receiverEffectsSize, receiverEffects);
    res->receiverEffectsSize = receiverEffectsSize;
    res->technic = technic;
    res->description = description;
    res->imageName = imageName;

    return res;
}

// free memory of item
void freeItem(item_t *item)
{
    freeEffectArray(item->launcherEffects, item->launcherEffectsSize);
    freeEffectArray(item->receiverEffects, item->receiverEffectsSize);
    free(item);
}

// free memory from item array of entity
void freeEntityItem(item_t **items)
{
    for (int itemsIdx = 0; itemsIdx < 5; itemsIdx++)
    {
        freeItem(items[itemsIdx]);
    }
    free(items);
}

// import an item from global array
item_t *importItem(item_import itemImport)
{
    return createItem(
        itemImport.name,
        itemImport.launcherEffectsSize,
        itemImport.launcherEffects,
        itemImport.receiverEffectsSize,
        itemImport.receiverEffects,
        itemImport.technic,
        itemImport.description,
        itemImport.imageName);
}

// import an item from enum id
item_t *importItemFromId(ITEM_ENCYCLOPEDIA_ID itemId)
{
    return importItem(ITEM_ENCYCLOPEDIA[itemId]);
}

// create an empty item list (used for entity)
item_t **createEmptyEntityItemList()
{
    item_t **res = malloc(5 * sizeof(item_t *));
    int item_ID;
    for (item_ID = 0; item_ID < 5; item_ID++)
    {
        res[item_ID] = importItemFromId(NONE_ITEM);
    };
    return res;
}

// import item from array enum id
item_t **importItemFromIdArray(int itemLength, int itemsId[itemLength])
{
    item_t **res = malloc(itemLength * sizeof(item_t *));
    int item_ID;
    for (item_ID = 0; item_ID < itemLength; item_ID++)
    {
        res[itemsId[item_ID] - 1] = importItemFromId(itemsId[item_ID]);
    };
    return res;
}

// DISPLAY FUNCTION

void displayItem(item_t *item)
{
    printf("\nNAME:\n_____\n%s\n\nLAUNCHER EFFECT:\n________________\n", item->name);
    displayEffectArray(item->launcherEffects, item->launcherEffectsSize);
    printf("\nRECEIVER EFFECT:\n________________\n");
    displayEffectArray(item->receiverEffects, item->receiverEffectsSize);
}
// display array function
void displayEntityItems(item_t **items)
{
    printf("ITEMS: \n______\n");

    for (int itemsIdx = 0; itemsIdx < 5; itemsIdx++)
    {
        if (items[itemsIdx]->description != NULL)
        {
            displayItem(items[itemsIdx]);
        }
    }
}

// TEST FUNCTION

void testItem()
{

    printf("\n==============================\n\tTEST DE ITEM\n==============================\n");
    item_t **testItem = importItemFromIdArray(2, (int[]){WEAPON, ARMOR});
    displayEntityItems(testItem);
    freeEntityItem(testItem);
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
        .description = "Recupère un petit peut de vie a chaque début de combat",
        .technic = "PV +6",
        .launcherEffects = {{HP_E, 6}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
        .imageName = "casse-croute.png",

    },
    {
        .name = "Arme",
        .description = "Augmente la force pour toute la durée du combat",
        .technic = "FORCE +4",
        .launcherEffects = {{STR_E, 4}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
        .imageName = "weapon.png",
    },
    {
        .name = "Casque",
        .description = "Augmente la dexterité pour toute la durée du combat afin d'obtenir plus de point d'esquive",
        .technic = "DEXTERITE +4",
        .launcherEffects = {{DEX_E, 4}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
        .imageName = "helmet.png",
    },
    // TODO: Refacto
    {
        .name = "Armure",
        .description = "Augmente les points de vie maximum",
        .technic = "PV MAX +20",
        .launcherEffects = {{HP_MAX_E, 20}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
        .imageName = "armor.png",
    },
    // TODO: Refacto
    {
        .name = "Chaussures",
        .description = "Permet de faire plus d'action à chaque tour",
        .technic = "ENERGY MAX +1",
        .launcherEffects = {{ENERGY_MAX_E, 1}},
        .launcherEffectsSize = 1,
        .receiverEffectsSize = 0,
        .imageName = "boots.png",
    },
};